#include "esp_console.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"

#include "serialAddon.h"
#include "parameters.h"
#include "UART.h"

void printHelp(){
    ESP_LOGD("help","----- SERIAL HELP -----");
    ESP_LOGD("help","(h)elp");
    ESP_LOGD("help","(s)ettings");
    ESP_LOGD("help","(u)tilities");
    ESP_LOGD("help",'(p)arameters');
    specificHelp();
}

void printParameters(){
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

    if (data[0] > 64 && data[i] < 91){ //If upperCase
        position = 0;
        i = 0;
        while(!end){
            if (data[i] == '\0') {end = true;}
            else if (data[i] > 64 && data[i] < 91){position += data[i] - 63;}
            else {end = true;}
            i++;
        }
    }
    else if (data[i] > 47 && data[i] < 58){ 

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
                printParameters();
            default :
                specificCommand(data);
        }
    }
}

