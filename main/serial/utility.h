#include "esp_log.h"
#include "esp_system.h"

void utilityHelp(){
    ESP_LOGI("Serial help","----- SERIAL UTILITY HELP -----");
    ESP_LOGI("Serial help","ur - reset");
}

void utilityCommand(char* data){
    switch(data[1]){
        case 'r':
            esp_restart();
        default :
            utilityHelp();
    }
}