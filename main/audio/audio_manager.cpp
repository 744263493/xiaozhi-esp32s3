#include "audio_manager.h"
#include "audio_utils.h"
#include "esp_log.h"

static const char *TAG = "AudioManager";

AudioManager::AudioManager() 
    : audio_capture_(nullptr), is_capturing_(false) {}

AudioManager::~AudioManager() {
    if (audio_capture_) {
        delete audio_capture_;
    }
}

esp_err_t AudioManager::init() {
    audio_capture_ = new AudioCapture();
    if (audio_capture_ == nullptr) {
        ESP_LOGE(TAG, "Failed to allocate memory for AudioCapture.");
        return ESP_ERR_NO_MEM;
    }
    
    // 初始化音频采集
    esp_err_t ret = audio_capture_->init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Audio capture initialization failed.");
        return ret;
    }
    
    ESP_LOGI(TAG, "AudioManager initialized successfully.");
    return ESP_OK;
}

esp_err_t AudioManager::start_capture() {
    if (is_capturing_) {
        ESP_LOGW(TAG, "Audio capture is already running.");
        return ESP_OK;
    }

    // 开始音频采集
    esp_err_t ret = audio_capture_->start();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start audio capture.");
        return ret;
    }

    is_capturing_ = true;
    ESP_LOGI(TAG, "Audio capture started.");
    return ESP_OK;
}

esp_err_t AudioManager::stop_capture() {
    if (!is_capturing_) {
        ESP_LOGW(TAG, "Audio capture is not running.");
        return ESP_OK;
    }

    // 停止音频采集
    esp_err_t ret = audio_capture_->stop();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to stop audio capture.");
        return ret;
    }

    is_capturing_ = false;
    ESP_LOGI(TAG, "Audio capture stopped.");
    return ESP_OK;
}

esp_err_t AudioManager::play_audio(const uint8_t *data, size_t length) {
    // 播放音频的功能代码（这里可以调用某些音频播放库）
    // TODO: 实现音频播放

    ESP_LOGI(TAG, "Playing audio data of length %d", length);
    return ESP_OK;
}

bool AudioManager::is_audio_capturing() const {
    return is_capturing_;
}
