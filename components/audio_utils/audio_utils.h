#ifndef AUDIO_UTILS_H
#define AUDIO_UTILS_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// OPUS 编解码函数声明
int opus_decode(const uint8_t *encoded_data, size_t encoded_length, uint8_t *decoded_data, size_t *decoded_length);
int opus_encode(const uint8_t *pcm_data, size_t pcm_length, uint8_t *encoded_data, size_t *encoded_length);

// MP3 编解码函数声明
int mp3_to_pcm(const uint8_t *mp3_data, size_t mp3_length, uint8_t *pcm_data, size_t *pcm_length);
int pcm_to_mp3(const uint8_t *pcm_data, size_t pcm_length, uint8_t *mp3_data, size_t *mp3_length);

// WAV 编解码函数声明
int wav_to_pcm(const uint8_t *wav_data, size_t wav_length, uint8_t *pcm_data, size_t *pcm_length);
int pcm_to_wav(const uint8_t *pcm_data, size_t pcm_length, uint8_t *wav_data, size_t *wav_length);

#ifdef __cplusplus
}
#endif

#endif // AUDIO_UTILS_H