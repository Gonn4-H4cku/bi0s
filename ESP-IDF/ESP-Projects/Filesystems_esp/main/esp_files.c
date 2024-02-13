#include <stdio.h>
#include "esp_spiffs.h"
#include "esp_log.h"

static const char* TAG = "ESP-Files";

void app_main(void)
{
    esp_vfs_spiffs_conf_t config=
    {
        .base_path = "/storage",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed= true

    };

    esp_err_t result = esp_vfs_spiffs_register(&config);
    if(result!=ESP_OK)
    {
        ESP_LOGE(TAG,"Failed to Initialize spiffs (%s)",esp_err_to_name(result));
    }

    size_t total = 0, used =0;
    result = esp_spiffs_info(config.partition_label,&total,&used);

    if(result!=ESP_OK)
    {
        ESP_LOGE(TAG,"Failed to get partition info (%s)",esp_err_to_name(result));
    }
    else
    {
        printf("Partiotions Total:%d , Used:%d \n",total,used);
    }

    FILE* f = fopen("/storage/myfile.txt","r");
    if(f==NULL)
    {
        ESP_LOGE(TAG,"Failed to open text File.");
        return;
    }
    char data[100];
    fgets(data,sizeof(data),f);
    fclose(f);

    printf("data from file: %s\n",data);
}
