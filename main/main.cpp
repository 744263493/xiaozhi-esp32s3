/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
//#include "led_strip.h"
#include "sdkconfig.h"

// main.cpp
#include "WiFi/wifi_manager.h"
#include "audio/audio_capture.h"
#include "audio/audio_manager.h"
#include "audio/audio_utils.h"
#include <nvs_flash.h>


/*
extern "C" void app_main()
{

    // 初始化 NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // _______________________wifi模块______________________________________
    wifi_init();

    bool was_connected = false;  // 用于记录之前的连接状态

    while (true) {
        bool is_connected = wifi_status();  // 获取当前连接状态

        if (is_connected && !was_connected) {
            printf("Wi-Fi connected\n");
            was_connected = true;  // 更新连接状态
        } else if (!is_connected && was_connected) {
            printf("Wi-Fi disconnected\n");
            was_connected = false;  // 更新连接状态
        }

        vTaskDelay(pdMS_TO_TICKS(5000));  // 每5秒检查一次Wi-Fi状态
    }

}
*/


// 定义任务句柄和标志位
static TaskHandle_t wifi_task_handle = NULL;
static TaskHandle_t audio_task_handle = NULL;
static volatile bool wifi_connected = false;

// WiFi任务（独立运行）
static void wifi_task(void *pvParameters) {
    wifi_init();
    bool was_connected = false;

    while (1) {
        bool is_connected = wifi_status();
        
        if (is_connected && !was_connected) {
            ESP_LOGI("WiFi", "Connected");
            wifi_connected = true;
            was_connected = true;
        } else if (!is_connected && was_connected) {
            ESP_LOGI("WiFi", "Disconnected");
            wifi_connected = false;
            was_connected = false;
        }

        vTaskDelay(pdMS_TO_TICKS(5000)); // 5秒检查一次
    }
}

// 音频任务（独立测试）
static void audio_task(void *pvParameters) {
    esp_err_t ret = audio_capture_init();
    if (ret != ESP_OK) {
        ESP_LOGE("Audio", "Init failed!");
        vTaskDelete(NULL);
    }

    uint8_t buffer[1024];
    while (1) {
        ret = audio_capture_read(buffer, sizeof(buffer));
        if (ret == ESP_OK) {
            ESP_LOGI("Audio", "Data read: %d bytes", sizeof(buffer));
            if (wifi_connected) {
                // 处理音频数据
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}


extern "C" void app_main() {
    // 初始化NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 创建独立任务（优先级可调）
    xTaskCreate(wifi_task, "wifi_task", 4096, NULL, 5, &wifi_task_handle);
    xTaskCreate(audio_task, "audio_task", 4096, NULL, 6, &audio_task_handle);

    // 主循环可留空或监控任务状态
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}