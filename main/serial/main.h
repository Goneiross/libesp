#include "esp_console.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"

#include "serialAddon.h"
#include "parameters.h"
#include "UART.h"

void serial_ini(){
   const int uart_num = UART_NUM_2;
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_CTS_RTS,
        .rx_flow_ctrl_thresh = 122,
    };
    esp_console_config_t console_config = {
            .max_cmdline_args = 8,
            .max_cmdline_length = 256,
    };
    uart_param_config(uart_num, &uart_config);
    uart_driver_install(CONFIG_CONSOLE_UART_NUM,256, 0, 0, NULL, 0);
    esp_console_init(&console_config); 
}

void printHelp(){
    printf("(h)elp");
    printf("(s)ettings");
    printf("(u)tilities");
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

    while(!end){
        if (data[i] == '\0') {end = true;}
        else if (data[i] > 64 && data[i] < 91){ //If upperCase
        
        }
        else if ((data[i] > 47 && data[i] < 58) || data[i] == '-'){ //If lowerCase or negative

        }
        i++;
    }

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

