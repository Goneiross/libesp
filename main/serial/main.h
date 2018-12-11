#include "esp_console.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"

#include "serialAddon.h"
#include "parameters.h"
#include "UART.h"

#include "esp_partition.h"

#define LOG_LOCAL_LEVEL ESP_LOG_INFO

void partition_ini(esp_partition_t * var,esp_partition_t * log){
    var = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "var");
    if (var == NULL){
        ESP_LOGW("ERROR","No partition named var found");
        ESP_LOGW("ERROR","Restarting in 10 secs");
        vTaskDelay(10000/portTICK_RATE_MS);
        esp_restart();
    }
    log = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_ANY, "log");
    if (log == NULL){
        EPS_LOGW("ERROR","No partition named log found");
        ESP_LOGW("ERROR","Restarting in 10 secs");
        vTaskDelay(10000/portTICK_RATE_MS);
        esp_restart();
    }
}

void partition_read(partition, adress, data *){
    int size = 0;
    // GET SIZE
    esp_partition_read(partition, data, adress, size);
}

void partition_write(partition, adress, data){
    int size = 0;
    // GET SIZE 
    esp_partition_write(partition, adress, data, size);

}

void parameters_update(esp_partition_t * var, int *parameters[PARAM_MAX]){
    for (int i = 0; i < PARAM_MAX; i++){
        partition_read(var, ADRESS_BEGIN + i, parameters[i]);
    }
}

void printHelp(){
    ESP_LOGD("help","----- SERIAL HELP -----");
    ESP_LOGD("help","(h)elp");
    ESP_LOGD("help","(s)ettings");
    ESP_LOGD("help","(u)tilities");
    ESP_LOGD("help",'(p)arameters');
    specificHelp();
}

void printParameters(esp_partition_t * var, int *parameters[PARAM_MAX]){
    parameters_update(var, parameters);
    for (int i = 0; i < PARAM_MAX; i++){
        ESP_LOGD("parameters","----- ALL DEVICE PARAMETERS -----");
        ESP_LOGD("parameters","#%i : %i",i,parameters[i]);
    }
}

void main_serial(char* data){
    bool end = false;
    uint8_t i = 0;
    uint8_t adress = 0x0;
    uint16_t position = 0;
    int16_t value = 0;
    bool negative = false;
    int parameters[PARAM_MAX];


    const esp_partition_t* var = NULL;
    const esp_partition_t* log = NULL;
    partition_ini(var, log);

    if (data[0] > 64 && data[i] < 91){ //If upperCase
        position = 0;
        value = 0;
        i = 0;
        negative = false;
        while(!end){
            if (data[i] == '\0') {end = true;}
            else if (data[i] > 64 && data[i] < 91){position += data[i] - 64;}
            else if (data[i] > 47 && data[i] < 58){value += data[i] - 47;}
            else if (data[i] == '-'){negative = true;}
            else {end = true;}
            i++;
        }
        if (negative){value *= -1;}
            partition_write(var ,position, value);

        // WRITE VALUE TO POSITION
    }
    else if (data[i] > 47 && data[i] < 58){ 
        // GET I2C ADRESS
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

