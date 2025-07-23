#include "audio_utils.h"
#include <opus/opus.h>
#include <mp3lame/lame.h>
#include <sndfile.h>
#include <string.h>
#include <stdio.h>

#define MAX_PCM_SIZE 1024 * 4
#define MAX_ENCODED_SIZE 1024

// OPUS 解码
int opus_decode(const uint8_t *encoded_data, size_t encoded_length, uint8_t *decoded_data, size_t *decoded_length) {
    OpusDecoder *decoder = opus_decoder_create(48000, 1, NULL);
    if (!decoder) {
        return -1; // 错误：无法创建解码器
    }

    int frame_size = opus_decode(decoder, encoded_data, encoded_length, decoded_data, MAX_PCM_SIZE, 0);
    if (frame_size < 0) {
        opus_decoder_destroy(decoder);
        return -1; // 解码失败
    }

    *decoded_length = frame_size;
    opus_decoder_destroy(decoder);
    return 0;
}

// OPUS 编码
int opus_encode(const uint8_t *pcm_data, size_t pcm_length, uint8_t *encoded_data, size_t *encoded_length) {
    OpusEncoder *encoder = opus_encoder_create(48000, 1, OPUS_APPLICATION_VOIP, NULL);
    if (!encoder) {
        return -1; // 错误：无法创建编码器
    }

    int max_encoded_size = opus_encoder_ctl(encoder, OPUS_GET_BITRATE_REQUEST, NULL);
    *encoded_length = opus_encode(encoder, pcm_data, pcm_length / 2, encoded_data, max_encoded_size);

    if (*encoded_length < 0) {
        opus_encoder_destroy(encoder);
        return -1; // 编码失败
    }

    opus_encoder_destroy(encoder);
    return 0;
}

// MP3 解码到 PCM
int mp3_to_pcm(const uint8_t *mp3_data, size_t mp3_length, uint8_t *pcm_data, size_t *pcm_length) {
    // 使用 libmp3lame 进行解码
    lame_t lame = lame_init();
    if (!lame) {
        return -1; // 错误：无法初始化LAME
    }

    lame_set_in_samplerate(lame, 44100);
    lame_set_num_channels(lame, 1);
    lame_set_mode(lame, STEREO);
    lame_init_params(lame);

    // 解码 MP3
    int num_samples = lame_decode(lame, mp3_data, mp3_length, pcm_data, MAX_PCM_SIZE);
    if (num_samples < 0) {
        lame_close(lame);
        return -1; // 解码失败
    }

    *pcm_length = num_samples;
    lame_close(lame);
    return 0;
}

// PCM 编码到 MP3
int pcm_to_mp3(const uint8_t *pcm_data, size_t pcm_length, uint8_t *mp3_data, size_t *mp3_length) {
    lame_t lame = lame_init();
    if (!lame) {
        return -1; // 错误：无法初始化LAME
    }

    lame_set_in_samplerate(lame, 44100);
    lame_set_num_channels(lame, 1);
    lame_set_mode(lame, STEREO);
    lame_init_params(lame);

    // 编码 PCM 为 MP3
    *mp3_length = lame_encode_buffer_interleaved(lame, (short*)pcm_data, pcm_length / 2, mp3_data, MAX_ENCODED_SIZE);
    if (*mp3_length < 0) {
        lame_close(lame);
        return -1; // 编码失败
    }

    lame_close(lame);
    return 0;
}

// WAV 解码到 PCM
int wav_to_pcm(const uint8_t *wav_data, size_t wav_length, uint8_t *pcm_data, size_t *pcm_length) {
    SF_INFO sf_info;
    SNDFILE *file = sf_open_fd((const int)wav_data, SFM_READ, &sf_info, 1);
    if (!file) {
        return -1; // 无法打开WAV文件
    }

    *pcm_length = sf_read_short(file, (short*)pcm_data, MAX_PCM_SIZE);
    sf_close(file);
    return 0;
}

// PCM 编码为 WAV
int pcm_to_wav(const uint8_t *pcm_data, size_t pcm_length, uint8_t *wav_data, size_t *wav_length) {
    SF_INFO sf_info;
    sf_info.channels = 1;
    sf_info.samplerate = 44100;
    sf_info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;
    
    SNDFILE *file = sf_open_fd((int)wav_data, SFM_WRITE, &sf_info, 1);
    if (!file) {
        return -1; // 无法打开WAV文件进行写入
    }

    sf_write_short(file, (short*)pcm_data, pcm_length / 2);
    sf_close(file);

    *wav_length = pcm_length; // WAV文件的长度等于PCM的长度
    return 0;
}