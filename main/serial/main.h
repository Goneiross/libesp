#include "esp_console.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"

#include "serialAddon.h"
#include "parameters.h"
#include "UART.h"

void printHelp(){
    printf("(h)elp");
    printf("(s)ettings");
    printf("(u)tilities");
    printf('(p)arameters');
    specificHelp();
}

void printParameters(){
    for (int i = 0; i < PARAM_MAX; i++){
        printf("#%i : %i",i,parameters[i]);
    }
}

void main_serial(char* data){
    bool end = false;
    uint8_t i = 0;
    uint8_t adress = 00;

    if (data[0] > 64 && data[i] < 91){
        while(!end){
            if (data[i] == '\0') {end = true;}
            else if (data[i] > 64 && data[i] < 91){ //If upperCase
    
            }
            else if ((data[i] > 47 && data[i] < 58) || data[i] == '-'){ //If lowerCase or negative

            }
            i++;
        }
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

