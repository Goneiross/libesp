#include "esp_log.h"

void settingsHelp(){
    ESP_LOGD("help","----- SERIAL SETTINGS HELP -----");
}

void settingsCommand(char* data){
    switch(data[1]){
        default :
            settingsHelp();
    }
}