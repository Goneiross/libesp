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

#define OFFSET 0x9000

void getTotalStorageSize(){
    size_t size = spi_flash_get_chip_size();
    ESP_LOGI("info","Chip size %d", size);
}

void read(int position, char* data[]){
    int size = sizeof(data);
    spi_flash_read(OFFSET + position, data, size);
}

void write(int position, int value){
    int size = 0;
    int tmp = value;
    do
    {
        tmp /= 10;
        size += 1;
    } while (tmp > 0);
    char data[size];
    int i = 0;
    do
    {
        data[size - 1 - i] = value % 10 + '0';
        value /= 10;
    } while (value > 0);
    ESP_LOGI("Write","write %s at %d ",data,OFFSET + position);
    spi_flash_erase_range(OFFSET, 4096);
    spi_flash_write(OFFSET + position, data, size);
}

void spi_test(){
    char buffer[4] = {0};
    char data[4] = "1111";
    ESP_LOGI("test","%d", sizeof(buffer));
    ESP_LOGE("SPI TEST","Begin SPI");
    spi_flash_erase_range(OFFSET, 2*4096);
    ESP_LOGE("SPI TEST","Erase done");
    spi_flash_read(OFFSET, buffer, 4);
    ESP_LOGE("SPI TEST","First read done : %s", buffer);
    spi_flash_write(OFFSET, data, 4);
    ESP_LOGE("SPI TEST","Write done");
    spi_flash_read(OFFSET, buffer, 4);
    ESP_LOGE("SPI TEST","Second read done : %s", buffer);
    ESP_LOGI("test","%d", sizeof(buffer));
    ESP_LOGE("SPI TEST","Done SPI");
}

void partition_dump(){

}

void parameters_update(uint16_t * parameters){
    for (uint8_t i = 0; i < PARAM_MAX; i++){
        read((uint8_t *) ADRESS_BEGIN + i, parameters[i]);
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

void printParameters(uint16_t *parameters){
    parameters_update(parameters);
    ESP_LOGI("Serial parameters","----- ALL DEVICE PARAMETERS -----");
    for (int i = 0; i < PARAM_MAX; i++){
        ESP_LOGI("Serial parameters","#%i : %i",i,parameters[i]);
    }
}

void partition_debug(const esp_partition_t * partition, uint16_t * parameters){
    printParameters(parameters);
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

    // spi_test();

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
            ESP_LOGI("Serial info","BEGIN");
            write(position, value);
            ESP_LOGI("Serial info","DONE");
        }
        
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
                printParameters(parameters);
            default :
                specificCommand(data);
        }
    }
    
}