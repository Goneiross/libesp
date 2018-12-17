#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "./serial/main.h"

void task(){
    main_serial("B 1");
    vTaskDelete(NULL);
}

void app_main(){
    
    xTaskCreatePinnedToCore(&task,"task", 32000, NULL, 0 ,NULL, 0);
    
}