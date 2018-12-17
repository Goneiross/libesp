#include "driver/uart.h"
#include "esp_console.h"
#include "esp_partition.h"
#include "linenoise/linenoise.h"
#include "esp_spi_flash.h"

#include "esp_log.h"

#include "serialAddon.h"
#include "parameters.h"
#include "settings.h"
#include "utility.h"
#include "UART.h"

#define PARAM_MAX 8

#define READ_WRITE_SIZE 4

void getTotalStorageSize(){
    size_t size = spi_flash_get_chip_size();
    ESP_LOGI("info","Chip size %d", size);
}

void partition_read(const esp_partition_t * partition, uint8_t * adress, int16_t *data){ //CHECK POINTER
    int size = READ_WRITE_SIZE ;
    if (partition == NULL){
        ESP_LOGE("Serial test ERROR","Pointer to partition is NULL !");
    }
    esp_err_t  err = esp_partition_read(partition, data, adress, size);
    if (err != ESP_OK){
        ESP_LOGE("Serial read ERROR","Could not read data");
    }
    if (err == ESP_ERR_INVALID_ARG){
        ESP_LOGE("Serial read ERROR","Offset exceed partition size !");
    }
    else if (err == ESP_ERR_INVALID_SIZE){
        ESP_LOGE("Serial read ERROR","Wrong size, exceed partition size !");
    }
}

void partition_write(const esp_partition_t * partition, uint8_t adress, int16_t data){
    int size = READ_WRITE_SIZE;
    if (partition == NULL){
        ESP_LOGE("Serial test ERROR","Pointer to partition is NULL !");
    }
    esp_err_t  err = esp_partition_erase_range(partition, (int)(adress / 4096), 4096);
    if (err != ESP_OK){
            ESP_LOGE("Serial erase ERROR","Could not erase data");
    }
    if (err == ESP_ERR_INVALID_ARG){
        ESP_LOGE("Serial erase ERROR","Pointer to partition is NULL !");
    }
    else if (err == ESP_ERR_INVALID_SIZE){
        ESP_LOGE("Serial erase ERROR","Exceed partition size !");
    }

    ESP_LOGE("test","size %d",(int)(size));
    ESP_LOGE("test","adress %d",(int)(adress));
    ESP_LOGE("test","data %d",data);
    if (partition == NULL){
        ESP_LOGE("Serial test ERROR","Pointer to partition is NULL !");
    }

    err = esp_partition_write(partition, adress, (void *)data, size);
    if (err != ESP_OK){
        ESP_LOGE("Serial write ERROR","Could not write data");
    }
    if (err == ESP_ERR_INVALID_ARG){
        ESP_LOGE("Serial write ERROR","Offset exceed partition size !");
    }
    else if (err == ESP_ERR_INVALID_SIZE){
        ESP_LOGE("Serial write ERROR","Wrong size, exceed partition size !");
    }
    ESP_LOGE("test","%d",(int)(adress));
}

void partition_test(const esp_partition_t * partition){
    char data [8] = {};
    partition_write(partition, 0x01, 0x2A);
    partition_read(partition, 0x01, data);
    ESP_LOGD("Parition test", "%s\n", data);
}

void spi_test(){
    char data[READ_WRITE_SIZE];
    ESP_LOGE("SPI TEST","Begin SPI");
    spi_flash_erase_range(0x00110001, READ_WRITE_SIZE);
    ESP_LOGE("SPI TEST","erase done");
    spi_flash_write(0x00110001, 0x2A, READ_WRITE_SIZE);
    ESP_LOGE("SPI TEST","write done");
    spi_flash_write(0x00110001, data, READ_WRITE_SIZE);
    ESP_LOGE("SPI TEST","read done : %s", data);
}

void partition_dump(){

}

void parameters_update(const esp_partition_t * var, uint16_t * parameters){
    for (uint8_t i = 0; i < PARAM_MAX; i++){
        partition_read(var, (uint8_t *) ADRESS_BEGIN + i, parameters[i]);
    }
}

void printHelp(){
    ESP_LOGI("Serial help","----- SERIAL HELP -----");
    ESP_LOGI("Serial help","(h)elp");
    ESP_LOGI("Serial help","(s)ettings");
    ESP_LOGI("Serial help","(u)tilities");
    ESP_LOGI("Serial help","(p)arameters");
    specificHelp();
}

void printParameters(const esp_partition_t * var, uint16_t *parameters){
    parameters_update(var, parameters);
    ESP_LOGI("Serial parameters","----- ALL DEVICE PARAMETERS -----");
    for (int i = 0; i < PARAM_MAX; i++){
        ESP_LOGI("Serial parameters","#%i : %i",i,parameters[i]);
    }
}

void partition_debug(const esp_partition_t * partition, uint16_t * parameters){
    printParameters(partition, parameters);
}

void main_serial(char* data){
    bool end = false;
    uint8_t i = 0;
    uint8_t adress = 0x0;
    uint16_t position = 0x00;
    int16_t value = 0; // could be negative !
    bool negative = false;
    uint16_t parameters[PARAM_MAX];

    getTotalStorageSize();

    const esp_partition_t* var = esp_partition_find_first(2, ESP_PARTITION_SUBTYPE_ANY, "var");
    if (var == NULL){
        ESP_LOGE("Serial ERROR","No partition named var found");
        ESP_LOGE("Serial ERROR","Restarting in 10 secs");
        vTaskDelay(10000/portTICK_RATE_MS);
        esp_restart();
    }
    const esp_partition_t* log = esp_partition_find_first(2, ESP_PARTITION_SUBTYPE_ANY, "log");
    if (log == NULL){
        ESP_LOGE("Serial ERROR","No partition named log found");
        ESP_LOGE("Serial ERROR","Restarting in 10 secs");
        vTaskDelay(10000/portTICK_RATE_MS);
        esp_restart();
    }
    ESP_LOGI("Serial partition","Initialized");

    spi_test();
    partition_test(var);

    if ((data[0] > 64) && (data[0] < 91)){ //If upperCase
        ESP_LOGI("Serial info","Uppercase case");
        position = 0;
        value = 0;
        i = 0;
        negative = false;
        while(!end){
            if (data[i] == '\0') {end = true;}
            else if (data[i] > 64 && data[i] < 91){position += data[i] - 64;}
            else if (data[i] > 47 && data[i] < 58){value += data[i] - 48;}
            else if (data[i] == '-'){negative = true;}
            else if (data[i] == ' '){}// nothing
            else {end = true;}
            i++;
        }
        ESP_LOGI("Serial info", "adress: %d, new value: %d" ,position, value);
        if (negative){value *= -1;}
        if (position > 0 && position < PARAM_MAX){
            partition_write(var ,position, value);
        }
        printParameters(var, parameters);
        //partition_debug(var, parameters);
        
    }
    else if (data[i] > 47 && data[i] < 58){ 
        // GET I2C ADRESS
        adress = 0x00;
    }
    
    else{
        ESP_LOGI("Serial info","%c", data[0]);
        switch (data[0]){
            case 'h':
                printHelp();
            case 's' :
                settingsCommand(data);
            case 'u' :
                utilityCommand(data);
            case 'p' :
                printParameters(var, parameters);
            default :
                specificCommand(data);
        }
    }
    
}