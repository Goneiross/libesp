#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "./serial/main.h"

void app_main(){
    main_serial("A 0");
}