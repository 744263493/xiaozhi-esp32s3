#include "driver/gpio.h"
#include "driver/i2s.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include"audio/audio_capture.h"

#define SAMPLE_RATE 16000  // 设置采样率为16kHz

#define I2S_BCK_IO      (GPIO_NUM_33)
#define I2S_WS_IO       (GPIO_NUM_5)
#define I2S_DO_IO       (GPIO_NUM_22)
#define I2S_DI_IO       (GPIO_NUM_21)

static const char *TAG = "audio_capture";

esp_err_t audio_capture_init() {
    ESP_LOGI(TAG, "Initializing audio capture...");
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),  // 显式转换为i2s_mode_t类型
        .sample_rate = SAMPLE_RATE,  // 设置采样率
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,  // 单声道
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,  // MSB格式
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = 8,
        .dma_buf_len = 1024,
    };

    i2s_pin_config_t pin_config = {
        .bck_io_num = I2S_BCK_IO,
        .ws_io_num = I2S_WS_IO,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_DI_IO
    };

    // 初始化I2S驱动
    esp_err_t ret = i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2S driver install failed");
        return ret;
    }

    // 设置引脚
    ret = i2s_set_pin(I2S_NUM_0, &pin_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2S set pin failed");
        return ret;
    }

    ESP_LOGI(TAG, "Audio capture initialized successfully!");
    return ESP_OK;
}

esp_err_t audio_capture_read(uint8_t *buffer, size_t length) {
    ESP_LOGI(TAG, "Reading audio data...");
    size_t bytes_read = 0;
    esp_err_t ret = i2s_read(I2S_NUM_0, buffer, length, &bytes_read, portMAX_DELAY);  // 使用portMAX_DELAY
    if (ret != ESP_OK || bytes_read != length) {
        ESP_LOGE(TAG, "Read failed: %d bytes, err=0x%x", bytes_read, ret);
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "Audio data read successfully!");
    return ESP_OK;
}
