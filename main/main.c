#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "ble_compatibility_test.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "mqtt_client.h"
#include "lwip/err.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "Share_var.h"
#include "BLE.h"
#include "MQTT.h"
void loop(void *pvParameters)
{
    while (1)
    {
        if (flag_BLE)
        {
            disable_ble();
            wifi_connect();
            mqtt_start();
            flag_BLE = false;
        }
        if(flag_WiFi){
            mqtt_stop();
            wifi_disconnect();
            enable_ble();
            flag_WiFi = false;
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay 1 second
    }
}
void app_main(void)
{
    esp_err_t ret;
    /* Initialize NVS. */
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    enable_ble();
    wifi_init();
    mqtt_config_init();

    xTaskCreate(loop, "loop", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 1, NULL);
    
}


