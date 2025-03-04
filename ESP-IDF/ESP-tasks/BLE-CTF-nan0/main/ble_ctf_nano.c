#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include "sdkconfig.h"

#define GPIO_PIN GPIO_NUM_0

bool status = false;
bool m1 = false;

char *TAG = "BLE-CTF-NANO";
uint8_t ble_addr_type;
void ble_app_advertise(void);

struct ble_gap_adv_params adv_params;

// Write data to ESP32 defined as server
static int device_write(uint16_t conn_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    // printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);

    //const char * data = (char *)ctxt->om->om_data;
    //printf("%d\n",strcmp(data, (char *)"1\r"));
    
    //printf("Data from the client: %.*s\n", ctxt->om->om_len, ctxt->om->om_data);
    
    char dataStr[ctxt->om->om_len + 1];
    memcpy(dataStr, ctxt->om->om_data, ctxt->om->om_len);
    dataStr[ctxt->om->om_len] = '\0';

    //printf("string : %s\n",dataStr);
    /*printf("Raw Data from the client: ");
    for (int i = 0; i < ctxt->om->om_len; i++) 
    {
    printf("%02X ", ctxt->om->om_data[i]); // Print each byte as a hexadecimal value
    }
    printf("\n");*/

    if (strcmp(dataStr, "start") == 0)
    {
        m1 = true;
        
        
    }
    vTaskDelay(200);
    
    return 0;
}

// Read data from ESP32 defined as server
static int device_read(uint16_t con_handle, uint16_t attr_handle, struct ble_gatt_access_ctxt *ctxt, void *arg)
{
    if(m1)
    {
        os_mbuf_append(ctxt->om, "flag{ble_kinda_!easy_}", strlen("flag{ble_kinda_!easy_}"));
        //m1 = false; 
    
    }
    if(!m1)
    {
        os_mbuf_append(ctxt->om, "You didnt start the CTF !", strlen("You didnt start the CTF !"));
    
    }
    vTaskDelay(10);
    return 0;
    
    
}

// Array of pointers to other service definitions
// UUID - Universal Unique Identifier
static const struct ble_gatt_svc_def gatt_svcs[] = {
    {.type = BLE_GATT_SVC_TYPE_PRIMARY,
     .uuid = BLE_UUID16_DECLARE(0x180),                 // Define UUID for device type
     .characteristics = (struct ble_gatt_chr_def[]){
         {.uuid = BLE_UUID16_DECLARE(0xFEF4),           // Define UUID for reading
          .flags = BLE_GATT_CHR_F_READ,
          .access_cb = device_read},
         {.uuid = BLE_UUID16_DECLARE(0xDEAD),           // Define UUID for writing
          .flags = BLE_GATT_CHR_F_WRITE,
          .access_cb = device_write},
         {0}}},
    {0}};

// BLE event handling
static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type)
    {
    // Advertise if connected
    case BLE_GAP_EVENT_CONNECT:
        ESP_LOGI("GAP", "BLE GAP EVENT CONNECT %s", event->connect.status == 0 ? "OK!" : "FAILED!");
        if (event->connect.status != 0)
        {
            ble_app_advertise();
        }
        break;
    // Advertise again after completion of the event
    case BLE_GAP_EVENT_DISCONNECT:
        ESP_LOGI("GAP", "BLE GAP EVENT DISCONNECTED");
        if (event->connect.status != 0)
        {
            ble_app_advertise();
        }
        
        break;
    case BLE_GAP_EVENT_ADV_COMPLETE:
        ESP_LOGI("GAP", "BLE GAP EVENT");
        ble_app_advertise();
        break;
    default:
        break;
    }
    return 0;
}

// Define the BLE connection
void ble_app_advertise(void)
{
    // GAP - device name definition
    struct ble_hs_adv_fields fields;
    const char *device_name;
    memset(&fields, 0, sizeof(fields));
    device_name = ble_svc_gap_device_name(); // Read the BLE device name
    fields.name = (uint8_t *)device_name;
    fields.name_len = strlen(device_name);
    fields.name_is_complete = 1;
    ble_gap_adv_set_fields(&fields);

    // GAP - device connectivity definition
    //struct ble_gap_adv_params adv_params;
    memset(&adv_params, 0, sizeof(adv_params));
    
}

// The application
void ble_app_on_sync(void)
{
    ble_hs_id_infer_auto(0, &ble_addr_type); // Determines the best address type automatically
    ble_app_advertise();                     // Define the BLE connection
}

// The infinite task
void host_task(void *param)
{
    nimble_port_run(); // This function will return only when nimble_port_stop() is executed
}


void bootwait(void *param)
{
    int secnd = 0;  
    while (1)
    {
        int val = gpio_get_level(GPIO_PIN);  
        if (val == 0)
        {
            secnd++;  
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
        else
        {
            if(secnd>3)
            {
            //printf("Seconds : %d\n",secnd);
            ESP_LOGI("BOOT BUTTON:", "Button Pressed FOR 3 SECOND\n");

            status = true;
            secnd=0;
            adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; // connectable or non-connectable
            adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN; // discoverable or non-discoverable
            ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
            vTaskDelay(pdMS_TO_TICKS(10)); 

            }

        }
        if(status)
        {
            adv_params.conn_mode = BLE_GAP_CONN_MODE_UND; // connectable or non-connectable
            adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN; // discoverable or non-discoverable
            ble_gap_adv_start(ble_addr_type, NULL, BLE_HS_FOREVER, &adv_params, ble_gap_event, NULL);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
void app_main()
{
    gpio_set_direction(GPIO_NUM_2,GPIO_MODE_OUTPUT);
    gpio_set_direction(GPIO_PIN,GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_PIN, GPIO_PULLUP_ONLY);

    nvs_flash_init();                          // 1 - Initialize NVS flash using
    // esp_nimble_hci_and_controller_init();      // 2 - Initialize ESP controller
    nimble_port_init();                        // 3 - Initialize the host stack
    ble_svc_gap_device_name_set("BLE-CTF-NANO"); // 4 - Initialize NimBLE configuration - server name
    ble_svc_gap_init();                        // 4 - Initialize NimBLE configuration - gap service
    ble_svc_gatt_init();                       // 4 - Initialize NimBLE configuration - gatt service
    ble_gatts_count_cfg(gatt_svcs);            // 4 - Initialize NimBLE configuration - config gatt services
    ble_gatts_add_svcs(gatt_svcs);             // 4 - Initialize NimBLE configuration - queues gatt services.
    ble_hs_cfg.sync_cb = ble_app_on_sync;      // 5 - Initialize application
    nimble_port_freertos_init(host_task);      // 6 - Run the thread

    xTaskCreate(bootwait, "boot_wait", 2048, NULL, 5, NULL);
}