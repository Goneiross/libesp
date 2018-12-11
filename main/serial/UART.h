#include "string.h"

void serial_ini(){
   const uart_port_t uart_num = UART_NUM_2;
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
    uart_driver_install((uart_port_t)CONFIG_CONSOLE_UART_NUM,256, 0, 0, NULL, 0);
    esp_console_init(&console_config); 
}

uint16_t UART_sendData(uart_port_t uart_num, const char *log, const char *data)
{
    const int length = strlen(data);
    const int writeBytes = uart_write_bytes(uart_num, data, length);
    if (writeBytes != length)
    {
        ESP_LOGI(log, "ERROR : %d bytes were not written", length - writeBytes);
    }
    else
    {
        ESP_LOGI(log, "Wrote %d bytes", writeBytes);
    }
    return (writeBytes);
}

uint16_t UART_receiveData(uart_port_t uart_num, const char *log, int expected)
{
    uint16_t *data[2048];
    int readBytes = uart_read_bytes(uart_num, data, 8, 5000); // &data ?
    if (readBytes > 0)
    {
        if ((expected == -1) || (readBytes == expected))
        {
            ESP_LOGI("LoRa868T20D_READ", "Read %d bytes", readBytes);
        }
        else
        {
            ESP_LOGI("LoRa868T20D_READ", "Read %d bytes but %d bytes were expected", readBytes, expected);
        }
    }
    else
    {
        ESP_LOGI("LoRa868T20D_READ", "No data read");
    }

    return (data);
}