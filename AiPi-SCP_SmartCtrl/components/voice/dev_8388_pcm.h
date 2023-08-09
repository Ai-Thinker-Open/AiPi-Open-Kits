/**
 * @file 8388_pcm.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-08-09
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef DEV_8388_PCM_H
#define DEV_8388_PCM_H

typedef enum {
    AUDIO_WAV_OPEN_TIP = 0,
    AUDIO_WAV_LED_ALL_ON,
    AUDIO_WAV_LED_ALL_OFF,
    AUDIO_WAV_LED_RAD,
    AUDIO_WAV_LED_GREEN,
    AUDIO_WAV_LED_BLUE,
    AUDIO_WAV_OPENING_LED,
    AUDIO_WAV_SERVER_CONNECT,
    AUDIO_WAV_WEATHER_CHECK,
    AUDIO_WAV_WIFI_SCAN_START,
    AUDIO_WAV_WIFI_SCAN_DONE,
    AUDIO_WAV_LED_CLOSE,
}audio_enum_t;

typedef struct {
    audio_enum_t audio_numble;
    unsigned int addrss;
    unsigned int size;
}audio_wav_t;

int es8388_voice_init(void);
void aipi_play_voices(audio_enum_t audio_numble);
#endif