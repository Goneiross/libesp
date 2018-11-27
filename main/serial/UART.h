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
    int readBytes = uart_read_bytes(uart_num, &data, 8, 5000);
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

    printf(data);
    return (data);
}