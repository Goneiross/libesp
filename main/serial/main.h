#include "driver/uart.h"
#include "esp_console.h"
#include "esp_partition.h"
#include "linenoise/linenoise.h"

#include "esp_log.h"

#include "serialAddon.h"
#include "parameters.h"
#include "settings.h"
#include "utility.h"
#include "UART.h"

#define PARAM_MAX 8

void partition_write_test(const esp_partition_t * partition){
    esp_partition_write(partition, 0x00, (void *) 0x2A, 2);
}

void partition_read(const esp_partition_t * partition, uint8_t * adress, int16_t data){
    int size = 0;
    // GET SIZE
    esp_partition_read(partition, data, adress, size);
}

void partition_write(const esp_partition_t * partition, uint8_t adress, int16_t data){
    int size = 0;
    // GET SIZE 
    esp_partition_write(partition, adress, (void *)data, size);

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
    for (int i = 0; i < PARAM_MAX; i++){
        ESP_LOGI("Serial parameters","----- ALL DEVICE PARAMETERS -----");
        ESP_LOGI("Serial parameters","#%i : %i",i,parameters[i]);
    }
}

void main_serial(char* data){
    bool end = false;
    uint8_t i = 0;
    uint8_t adress = 0x0;
    uint16_t position = 0;
    int16_t value = 0; // could be negative !
    bool negative = false;
    uint16_t parameters[PARAM_MAX];

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

    //partition_ini(var, log);

    if (data[0] > 64 && data[i] < 91){ //If upperCase
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
        
    }
    else if (data[i] > 47 && data[i] < 58){ 
        // GET I2C ADRESS
        adress = 0x00;
    }
    
    else{
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