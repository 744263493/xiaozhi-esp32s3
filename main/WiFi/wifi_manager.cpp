// wifi_manager.cpp
#include "wifi_manager.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include <esp_err.h>
#include <esp_netif_ip_addr.h>
#include <cstring>

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static EventGroupHandle_t wifi_event_group;
static const char *TAG = "WiFi";

static esp_event_handler_instance_t instance_wifi;
static esp_event_handler_instance_t instance_ip;

void wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) {
    if (event_base == WIFI_EVENT) {
        if (event_id == WIFI_EVENT_STA_START) {
            ESP_LOGI(TAG, "Wi-Fi started, trying to connect...");
            esp_wifi_connect();
        } else if (event_id == WIFI_EVENT_STA_CONNECTED) {
            ESP_LOGI(TAG, "Wi-Fi connected.");
            xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
        } else if (event_id == WIFI_EVENT_STA_DISCONNECTED) {
            wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*) event_data;
            ESP_LOGW(TAG, "Wi-Fi disconnected. Reason: %d", event->reason);
            xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
            
            vTaskDelay(pdMS_TO_TICKS(3000));
            esp_wifi_connect();
        }
    } else if (event_base == IP_EVENT) {
        if (event_id == IP_EVENT_STA_GOT_IP) {
            ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
            ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
        }
    }
}

void wifi_init()
{
    esp_log_level_set("*", ESP_LOG_INFO);

    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_event_group = xEventGroupCreate();
    
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, &instance_wifi));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
        IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, &instance_ip));

    // 正确的Wi-Fi配置初始化方式
    wifi_config_t wifi_config = {};
    strncpy((char*)wifi_config.sta.ssid, "5007", sizeof(wifi_config.sta.ssid));
    strncpy((char*)wifi_config.sta.password, "88888888", sizeof(wifi_config.sta.password));
    wifi_config.sta.scan_method = WIFI_ALL_CHANNEL_SCAN;
    wifi_config.sta.bssid_set = false;
    wifi_config.sta.channel = 0;
    wifi_config.sta.listen_interval = 1;
    wifi_config.sta.sort_method = WIFI_CONNECT_AP_BY_SIGNAL;
    wifi_config.sta.threshold.rssi = -127;
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.sta.pmf_cfg.capable = false;
    wifi_config.sta.pmf_cfg.required = false;

    wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));
    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    EventBits_t bits = xEventGroupWaitBits(
        wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY
    );
    
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "WiFi connected successfully");
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGE(TAG, "WiFi connection failed");
    }
}

void wifi_disconnect()
{
    ESP_LOGI(TAG, "Disconnecting from WiFi...");
    esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, instance_wifi);
    esp_event_handler_instance_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, instance_ip);
    
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_wifi_deinit());
    
    vEventGroupDelete(wifi_event_group);
}

int wifi_status()
{
    wifi_ap_record_t ap_info;
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        return 1;
    }
    return 0;
}