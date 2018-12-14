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

void getTotalStorageSize(){
    size_t size = spi_flash_get_chip_size();
    ESP_LOGI("info","Chip size %d", size);
}

void partition_read(const esp_partition_t * partition, uint8_t * adress, int16_t data){
    int size = 0 ;
    // GET CORRECT SIZE !!!
    esp_err_t  err = esp_partition_read(partition, data, adress, size);
    if (err != ESP_OK){
        ESP_LOGI("Serial read ERROR","Could not read data");
    }
    if (err == ESP_ERR_INVALID_ARG){
        ESP_LOGI("Serial read ERROR","Offset exceed partition size !");
    }
    else if (err == ESP_ERR_INVALID_SIZE){
        ESP_LOGI("Serial read ERROR","Wrong size, exceed partition size !");
    }
}

void partition_write(const esp_partition_t * partition, uint8_t adress, int16_t data){
    int size = 0;
    // GET CORRECT SIZE !!!
    esp_err_t  err = esp_partition_erase_range(partition, adress, 4096);
    if (err != ESP_OK){
            ESP_LOGI("Serial erase ERROR","Could not erase data");
    }
    if (err == ESP_ERR_INVALID_ARG){
        ESP_LOGI("Serial erase ERROR","Pointer to partition is NULL !");
    }
    else if (err == ESP_ERR_INVALID_SIZE){
        ESP_LOGI("Serial erase ERROR","Exceed partition size !");
    }


    err = esp_partition_write(partition, adress, (void *)data, size);
    if (err != ESP_OK){
        ESP_LOGI("Serial write ERROR","Could not write data");
    }
    if (err == ESP_ERR_INVALID_ARG){
        ESP_LOGI("Serial write ERROR","Offset exceed partition size !");
    }
    else if (err == ESP_ERR_INVALID_SIZE){
        ESP_LOGI("Serial write ERROR","Wrong size, exceed partition size !");
    }
}

void partition_write_test(const esp_partition_t * partition){
    esp_partition_erase_range(partition, 0x00, 4096);
    esp_partition_write(partition, 0x0001, (const void *) 0xA2, 2);
    ESP_LOGI("Serial test","TEST");
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

    const esp_partition_t* var = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "var");
    if (var == NULL){
        ESP_LOGI("Serial ERROR","No partition named var found");
        ESP_LOGI("Serial ERROR","Restarting in 10 secs");
        vTaskDelay(10000/portTICK_RATE_MS);
        esp_restart();
    }
    const esp_partition_t* log = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "log");
    if (log == NULL){
        ESP_LOGI("Serial ERROR","No partition named log found");
        ESP_LOGI("Serial ERROR","Restarting in 10 secs");
        vTaskDelay(10000/portTICK_RATE_MS);
        esp_restart();
    }
    ESP_LOGI("Serial partition","Initialized");

    //partition_write_test(var);

    /*
    esp_err_t err = esp_partition_erase_range(var, 0x00, 4096);
    if (err != ESP_OK){
            ESP_LOGI("Serial ERROR","Could not erase data");
    }
    esp_partition_write(var, 0x01, 0xA2, 4);
    */

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