#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include "audio_capture.h"
#include "audio_processing.h"

// 音频管理类
class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    // 初始化音频管理系统
    esp_err_t init();

    // 启动音频采集
    esp_err_t start_capture();

    // 停止音频采集
    esp_err_t stop_capture();

    // 播放音频
    esp_err_t play_audio(const uint8_t *data, size_t length);

    // 获取当前音频输入状态
    bool is_audio_capturing() const;

private:
    AudioCapture *audio_capture_;  // 音频采集对象
    bool is_capturing_;            // 是否正在采集音频
};

#endif // AUDIO_MANAGER_H
