/**
 * @file https_client.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-21
 *
 * @copyright Copyright (c) 2023
 *
*/
#ifndef HTTPS_CLIENT_H
#define HTTPS_CLIENT_H
#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_HOST "v0.yiketianqi.com"
#define HTTP_PATH "free/week?unescape=1&appid=17769781&appsecret=5IbudTJx"

    extern int clock_1_hour_value;
    extern int clock_1_min_value;
    extern int clock_1_sec_value;

    extern int date_day_value;
    extern int date_wday_value;
    extern int date_mont_value;
    extern int date_yesr_value;

    char* https_get_request(const char* host, const char* https_url);
    void https_get_weather_task(void* arg);
    char* https_get_data(const char* https_request_data);
#ifdef __cplusplus
}
#endif
#endif