#include "esp_log.h"

void utilityHelp(){
    ESP_LOGD("help","----- SERIAL UTILITY HELP -----");
    ESP_LOGD("help","ur - reset");
}

void utilityCommand(char* data){
    switch(data[1]){
        case 'r':
            esp_restart();
        default :
            utilityHelp();
    }
}