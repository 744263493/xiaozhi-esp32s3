#ifndef AUDIO_CAPTURE_H
#define AUDIO_CAPTURE_H

#include "driver/i2s_std.h"  // 使用标准I2S模式
#include "esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

// ES8311初始化（需根据编解码器配置）
esp_err_t es8311_codec_init();

// 音频采集接口
esp_err_t audio_capture_init();
esp_err_t audio_capture_read(uint8_t *buffer, size_t length);

#ifdef __cplusplus
}
#endif

#endif // AUDIO_CAPTURE_H