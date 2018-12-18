/*
    const esp_partition_t* var = esp_partition_find_first(2, ESP_PARTITION_SUBTYPE_ANY, "var");
    if (var == NULL){
        ESP_LOGE("Serial ERROR","No partition named var found");
        ESP_LOGE("Serial ERROR","Restarting in 10 secs");
        vTaskDelay(10000/portTICK_RATE_MS);
        esp_restart();
    }
    const esp_partition_t* log = esp_partition_find_first(2, ESP_PARTITION_SUBTYPE_ANY, "log");
    if (log == NULL){
        ESP_LOGE("Serial ERROR","No partition named log found");
        ESP_LOGE("Serial ERROR","Restarting in 10 secs");
        vTaskDelay(10000/portTICK_RATE_MS);
        esp_restart();
    }
    ESP_LOGI("Serial partition","Initialized");
    */

void partition_read(const esp_partition_t * partition, uint8_t * adress, char *data){ //CHECK POINTER
    int size = READ_WRITE_SIZE ;
    if (partition == NULL){
        ESP_LOGE("Serial test ERROR","Pointer to partition is NULL !");
    }
    esp_err_t  err = esp_partition_read(partition, data, adress, size);
    if (err != ESP_OK){
        ESP_LOGE("Serial read ERROR","Could not read data");
    }
    if (err == ESP_ERR_INVALID_ARG){
        ESP_LOGE("Serial read ERROR","Offset exceed partition size !");
    }
    else if (err == ESP_ERR_INVALID_SIZE){
        ESP_LOGE("Serial read ERROR","Wrong size, exceed partition size !");
    }
}

void partition_write(const esp_partition_t * partition, uint8_t adress, char* data){
    int size = READ_WRITE_SIZE;
    if (partition == NULL){
        ESP_LOGE("Serial test ERROR","Pointer to partition is NULL !");
    }
    esp_err_t  err = esp_partition_erase_range(partition, (int)(adress / 4096), 4096);
    if (err != ESP_OK){
            ESP_LOGE("Serial erase ERROR","Could not erase data");
    }
    if (err == ESP_ERR_INVALID_ARG){
        ESP_LOGE("Serial erase ERROR","Pointer to partition is NULL !");
    }
    else if (err == ESP_ERR_INVALID_SIZE){
        ESP_LOGE("Serial erase ERROR","Exceed partition size !");
    }

    ESP_LOGE("test","size %d",(int)(size));
    ESP_LOGE("test","adress %d",(int)(adress));
    ESP_LOGE("test","data %s",data);
    if (partition == NULL){
        ESP_LOGE("Serial test ERROR","Pointer to partition is NULL !");
    }

    err = esp_partition_write(partition, adress, data, size);
    if (err != ESP_OK){
        ESP_LOGE("Serial write ERROR","Could not write data");
    }
    if (err == ESP_ERR_INVALID_ARG){
        ESP_LOGE("Serial write ERROR","Offset exceed partition size !");
    }
    else if (err == ESP_ERR_INVALID_SIZE){
        ESP_LOGE("Serial write ERROR","Wrong size, exceed partition size !");
    }
    ESP_LOGE("test","%d",(int)(adress));
}

void partition_test(const esp_partition_t * partition){
    char buffer[READ_WRITE_SIZE];
    char data[4] = "2222";
    ESP_LOGE("PARTITION  TEST","Begin PARTITION TEST");
    partition_write(partition, 0x01, data);
    partition_read(partition, 0x01, buffer);
    ESP_LOGD("Parition test", "%s", buffer);
}