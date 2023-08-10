// SPDX-License-Identifier: MIT
// Copyright 2020 NXP

/**
 * @file custom.c
 *
 */

 /*********************
  *      INCLUDES
  *********************/
#include "FreeRTOS.h"
#include "timers.h"
#include "queue.h"
#include <stdio.h>
#include "cJSON.h"
#include "lvgl.h"
#include "custom.h"
#include "user_esflash.h" 
#include "wifi_event.h"
#include "https_client.h"
#include "wifi_mgmr_ext.h"
#include "wifi_mgmr.h"
#include "log.h"
#include "user_mqtt.h"
  /*********************
   *      DEFINES
   *********************/
#define DBG_TAG "CUSTOM"

#define FISH_TEMP "temperature"
#define FISH_O2 "oxygen"
#define FISH_PH "pH"
#include "dev_8388_pcm.h"
   /**********************
    *      TYPEDEFS
    **********************/

    /**********************
     *  STATIC PROTOTYPES
     **********************/
xQueueHandle queue;
TaskHandle_t https_Handle;
TimerHandle_t http_timers;
static wifi_mgmr_scan_item_t wifi_aps[32];
/**********************
 *  STATIC VARIABLES
 **********************/
static void queue_receive_task(void* arg);
static custom_event_t queue_get_custom_event(char* json_data);
static int cjson_analysis_wifi_scan(char* json_data);
static char* cjson_analysis_ssid(char* json_data);
static char* cjson_analysis_password(char* json_data);
static char* cjson__analysis_ip(char* cjson_data);
static void http_hour_requst_time(TimerHandle_t timer);
static int cjson_get_weather(char* weather_data);
static mqtt_dev_type_t mqtt_analysis_type_for_msg(const char* json_data);
static double mqtt_analysis_get_data_for_fish(char* json_data, char* data_paramName);
static bool mqtt_get_rgb_data(char* json_data, uint8_t* red, uint8_t* green, uint8_t* blue);

/**
 * Create a demo application
 */
void custom_init(lv_ui* ui)
{
  /* Add your codes here */
  queue = xQueueCreate(10, 1024);
  xTaskCreate(queue_receive_task, "queue_receive_task", 1024, ui, 3, NULL);
  http_timers = xTimerCreate("http_timers", pdMS_TO_TICKS(1000), pdTRUE, 0, http_hour_requst_time);
  mqtt_client_init();
}
/**
 * @brief
 *
 * @param timer
*/
static uint16_t timers_http = 0;
static void http_hour_requst_time(TimerHandle_t timer)
{
  if (timers_http>=60*60) {
    LOG_I("Timed to http update,start https request");
    vTaskResume(https_Handle);
    timers_http = 0;
  }
  else {
    timers_http++;
  }

}
/**
 * @brief queue_receive_task 状态机
 *
 * @param arg
*/
static void queue_receive_task(void* arg)
{
  static wifi_mgmr_scan_params_t wifi_scan_config;

  char* queue_buff = NULL;
  char* ssid_list = NULL;
  char* ssid = NULL;
  char* password = NULL;
  char* ipv4_addr = NULL;
  lv_ui* ui = (lv_ui*)arg;
  ssid = flash_get_data(SSID_KEY, 32); //读取SSID
  password = flash_get_data(PASS_KEY, 32);//读取PASS

  if (ssid!=NULL)
  {
    LOG_I("read flash ssid:%s password:%s", ssid, password);
    wifi_connect(ssid, password);
  }
  else {
    LOG_E("ssid read value is NULL:%06X", SSID_KEY);
  }
  vPortFree(ssid);
  vPortFree(password);

  while (1) {
    queue_buff = pvPortMalloc(1024);
    memset(queue_buff, 0, 1024);
    xQueueReceive(queue, queue_buff, portMAX_DELAY);//读取

    switch (queue_get_custom_event(queue_buff))
    {

      case CUSTOM_EVENT_WIFI_SCAN_DONE://扫描完成事件
      {
        ssid_list = pvPortMalloc(256);
        memset(ssid_list, 0, 256);
        if (!cjson_analysis_wifi_scan(queue_buff)) {
          int ssids = wifi_mgmr_sta_scanlist_nums_get();
          ssids = (ssids>=20)?20:ssids;

          wifi_mgmr_sta_scanlist_dump(wifi_aps, ssids);
          // if(wifi_mgmr_sta_scanlist_nums_get())
          for (size_t i = 0; i < ssids; i++)
          {
            //进行字符串拼接
            strcat(ssid_list, wifi_aps[i].ssid);
            if ((ssids)-i != 1) {
              strcat(ssid_list, "\n");
            }
          }
          lv_dropdown_set_options(ui->screen_ddlist_ssid_list, ssid_list);
          vTaskDelay(100/portTICK_RATE_MS);
          lv_event_send(ui->screen_img_loading, LV_EVENT_CLICKED, NULL);
          memset(ui->ssid_list, 0, 256);
          strcpy(ui->ssid_list, ssid_list);
          ui->wifi_scan_done = true;
          vPortFree(ssid_list);

          aipi_play_voices(AUDIO_WAV_WIFI_SCAN_DONE);
        }
      }
      break;
      case CUSTOM_EVENT_GET_WIFI: //获取到WiFi 名称和密码
      {
        ssid = cjson_analysis_ssid(queue_buff);
        password = cjson_analysis_password(queue_buff);
        LOG_I("ssid=%s password:%s", ssid, password);
        wifi_connect(ssid, password);
        vPortFree(ssid);
        vPortFree(password);
      }
      break;

      case CUSTOM_EVENT_GOT_IP://获取到IP地址
      {
        ipv4_addr = cjson__analysis_ip(queue_buff);
        LOG_I(" ipv4 addr=%s", ipv4_addr);
        memset(queue_buff, 0, 1024);
        sprintf(queue_buff, "IP:%s", ipv4_addr);
        strcpy(ui->ssid, flash_get_data(SSID_KEY, 32));
        strcpy(ui->password, flash_get_data(PASS_KEY, 32));
        ui->wifi_status = true;
        // 更新屏幕显示
        if (ui->screen_type==0) {
          lv_img_set_src(ui->screen_img_wifi, &_wifi_alpha_22x18);
          lv_textarea_set_text(ui->screen_ta_pass, ui->password);
          //在列表中添加连接的SSID
          lv_dropdown_add_option(ui->screen_ddlist_ssid_list, ui->ssid, lv_dropdown_get_option_cnt(ui->screen_ddlist_ssid_list)+1);
          //找到列表中的SSID 并选中显示
          if (lv_dropdown_get_option_index(ui->screen_ddlist_ssid_list, ui->ssid)>=0) {
            lv_dropdown_set_selected(ui->screen_ddlist_ssid_list, lv_dropdown_get_option_index(ui->screen_ddlist_ssid_list, ui->ssid));
          }
          else {
            lv_dropdown_set_selected(ui->screen_ddlist_ssid_list, 0);
          }
        }
        vPortFree(ipv4_addr);

        if (https_Handle!=NULL) {
          vTaskDelete(https_Handle);
        }

        xTaskCreate(https_get_weather_task, "https task", 1024*2, NULL, 4, &https_Handle);
        mqtt_start_connect(MQTT_SERVER, MQTT_PORT, MQTT_USER_NAME, MQTT_PASSWOLD);
      }
      break;
      case CUSTOM_EVENT_GET_WIFI_DISCONNECT: //WiFi 断开
      {
        if (ui->screen_type==0) {
          lv_img_set_src(ui->screen_img_wifi, &_no_internet_alpha_22x18);
        }
      }
      break;
      case CUSTOM_EVENT_GET_WEATHER: //获取到天气
      {
        if (!cjson_get_weather(queue_buff)) {
          if (ui->screen_type==0) {
            lv_label_set_text_fmt(ui->screen_label_dizhi, "%s市  %s", ui->city, ui->waether);
          }
          aipi_play_voices(AUDIO_WAV_WEATHER_CHECK);


        }
      }
      break;
      case CUSTOM_EVENT_MQTT_CONNECT: //MQTT 连接服务器成功
      {
        LOG_F("CUSTOM_EVENT_MQTT_CONNECT queue data:%s", queue_buff);
        aipi_play_voices(AUDIO_WAV_SERVER_CONNECT);
        mqtt_app_subscribe(FISH_MQTT_SUB_TOPIC, 0);
        mqtt_app_subscribe(MQTT_CLIETN_TOPIC, 0);
      }
      break;
      case CUSTOM_EVENT_MQTT_DISCONNECT: //MQTT断开
      {
        LOG_F("CUSTOM_EVENT_MQTT_DISCONNECT queue data:%s", queue_buff);
        if (ui->wifi_status) {
          mqtt_client_init();
          mqtt_start_connect(MQTT_SERVER, MQTT_PORT, MQTT_USER_NAME, MQTT_PASSWOLD);
        }

      }
      break;
      case CUSTOM_EVENT_MQTT_GET_MSG: //MQTT 接收到数据
      {
        LOG_D("GET_MSG queue data:%.*s", strlen(queue_buff), queue_buff);
        switch (mqtt_analysis_type_for_msg(queue_buff))
        {
          case MQTT_DEV_TYPE_FISH:
          {
            if (ui->screen_type==0) {
              lv_obj_add_flag(ui->screen_cont_sensor_no, LV_OBJ_FLAG_HIDDEN);
              lv_label_set_text(ui->screen_label_3, "已连接");
              ui->fish_dev_connect_status = true;
              char* data_str = pvPortMalloc(16);
              memset(data_str, 0, 16);
              sprintf(data_str, "%.0f", mqtt_analysis_get_data_for_fish(queue_buff, FISH_O2)*100);
              lv_label_set_text_fmt(ui->screen_label_O2_vlue, "%s", data_str);
              memset(data_str, 0, 16);
              sprintf(data_str, "%.1f", mqtt_analysis_get_data_for_fish(queue_buff, FISH_TEMP));
              lv_label_set_text_fmt(ui->screen_label_temp_value, "%s℃", data_str);
              memset(data_str, 0, 16);
              sprintf(data_str, "%.2f", mqtt_analysis_get_data_for_fish(queue_buff, FISH_PH));
              lv_label_set_text_fmt(ui->screen_label_PH_value, "%s", data_str);
              vPortFree(data_str);
            }
          }
          break;
          case MQTT_DEV_TYPE_RGB_WB2:
          {
            LOG_I("device connect id:%d", MQTT_DEV_TYPE_RGB_WB2);
            if (ui->screen_type==0) {
              lv_obj_add_flag(ui->screen_cont_dev1_no, LV_OBJ_FLAG_HIDDEN);
              lv_label_set_text(ui->screen_label_status1, "Ai-WB2/已连接");
              ui->ai_wb2_dev->connect_status = true;
              //接收到RGB消息
              ui->ai_wb2_dev->switch_status = mqtt_get_rgb_data(queue_buff, &ui->ai_wb2_dev->red, &ui->ai_wb2_dev->green, &ui->ai_wb2_dev->blue);
              if (ui->ai_wb2_dev->switch_status) {
                lv_obj_add_flag(ui->screen_img_wb2_open, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui->screen_img_wb2_close, LV_OBJ_FLAG_HIDDEN);
                lv_img_set_src(ui->screen_img_rgb, &_RGB_close_alpha_32x32);

                lv_color_t wb2_rgb_color = {
                  .ch.red = ui->ai_wb2_dev->red,
                  .ch.green = ui->ai_wb2_dev->green,
                  .ch.blue = ui->ai_wb2_dev->blue,
                };
                lv_obj_set_style_img_recolor_opa(ui->screen_img_rgb, 255, 0);
                lv_obj_set_style_img_recolor(ui->screen_img_rgb, wb2_rgb_color, _LV_STYLE_STATE_CMP_SAME);

              }
              else {
                lv_obj_add_flag(ui->screen_img_wb2_close, LV_OBJ_FLAG_HIDDEN);
                lv_obj_clear_flag(ui->screen_img_wb2_open, LV_OBJ_FLAG_HIDDEN);
                lv_img_set_src(ui->screen_img_rgb, &_RGB_open_alpha_32x32);
                lv_obj_set_style_img_recolor_opa(ui->screen_img_rgb, 0, 0);
              }
            }
            else if (ui->screen_type==1) {
              ui->ai_wb2_dev->switch_status = mqtt_get_rgb_data(queue_buff, &ui->ai_wb2_dev->red, &ui->ai_wb2_dev->green, &ui->ai_wb2_dev->blue);
              // 配置另外一个界面的UI
              if (ui->ai_wb2_dev->switch_status) {
                lv_obj_add_state(ui->screen_rgb1_wb2_sw, LV_STATE_CHECKED);
                lv_obj_clear_flag(ui->screen_rgb1_cpicker_wb2_rgb, LV_OBJ_FLAG_HIDDEN);
                lv_color_t wb1_rgb_color = {
                  .ch.red = ui->ai_wb2_dev->red,
                  .ch.green = ui->ai_wb2_dev->green,
                  .ch.blue = ui->ai_wb2_dev->blue,
                };

                lv_colorwheel_set_rgb(ui->screen_rgb1_cpicker_wb2_rgb, wb1_rgb_color);
              }
              else {
                lv_obj_clear_state(ui->screen_rgb1_wb2_sw, LV_STATE_CHECKED);
                lv_obj_add_flag(ui->screen_rgb1_cpicker_wb2_rgb, LV_OBJ_FLAG_HIDDEN);
              }
            }

          }
          break;

          case MQTT_DEV_TYPE_RGB_M62:
          {
            LOG_I("device connect id:%d", MQTT_DEV_TYPE_RGB_M62);
            if (ui->screen_type==0) {
              lv_obj_add_flag(ui->screen_cont_dev2_no, LV_OBJ_FLAG_HIDDEN);
              lv_label_set_text(ui->screen_label_status2, "Ai-M62/已连接");
              ui->ai_m62_dev->connect_status = true;

              ui->ai_m62_dev->switch_status = mqtt_get_rgb_data(queue_buff, &ui->ai_m62_dev->red, &ui->ai_m62_dev->green, &ui->ai_m62_dev->blue);
              if (ui->ai_m62_dev->switch_status) {
                lv_obj_clear_flag(ui->screen_img_m62_close, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui->screen_img_m62_open, LV_OBJ_FLAG_HIDDEN);
                lv_img_set_src(ui->screen_img_rgb1, &_RGB_close_alpha_32x32);
                lv_color_t rgb_color = {
                    .ch.red = ui->ai_m62_dev->red,
                    .ch.green = ui->ai_m62_dev->green,
                    .ch.blue = ui->ai_m62_dev->blue,
                };
                lv_obj_set_style_img_recolor_opa(ui->screen_img_rgb1, 255, 0);
                lv_obj_set_style_img_recolor(ui->screen_img_rgb1, rgb_color, _LV_STYLE_STATE_CMP_SAME);
              }
              else {
                lv_obj_clear_flag(ui->screen_img_m62_open, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(ui->screen_img_m62_close, LV_OBJ_FLAG_HIDDEN);
                lv_img_set_src(ui->screen_img_rgb1, &_RGB_open_alpha_32x32);
                lv_obj_set_style_img_recolor_opa(ui->screen_img_rgb1, 0, 0);
              }

            }
            else if (ui->screen_type==2) {
              ui->ai_m62_dev->switch_status = mqtt_get_rgb_data(queue_buff, &ui->ai_m62_dev->red, &ui->ai_m62_dev->green, &ui->ai_m62_dev->blue);
              //配置另外一个界面的UI
              if (ui->ai_m62_dev->switch_status) {
                lv_obj_add_state(ui->screen_rgb2_Ai_M62_sw, LV_STATE_CHECKED);
                lv_obj_clear_flag(ui->screen_rgb2_cpicker_M62_rgb, LV_OBJ_FLAG_HIDDEN);
                lv_color_t m62_rgb_color = {
                  .ch.red = ui->ai_m62_dev->red,
                  .ch.green = ui->ai_m62_dev->green,
                  .ch.blue = ui->ai_m62_dev->blue,
                };

                lv_colorwheel_set_rgb(ui->screen_rgb2_cpicker_M62_rgb, m62_rgb_color);
              }
              else {
                lv_obj_clear_state(ui->screen_rgb2_Ai_M62_sw, LV_STATE_CHECKED);
                lv_obj_add_flag(ui->screen_rgb2_cpicker_M62_rgb, LV_OBJ_FLAG_HIDDEN);
              }
            }
          }
          break;
          case MQTT_DEV_TYPE_RGB_BW16:
          {
            LOG_I("device connect id:%d", MQTT_DEV_TYPE_RGB_BW16);
            if (ui->screen_type==0) {
              lv_obj_add_flag(ui->screen_cont_dev_no, LV_OBJ_FLAG_HIDDEN);
              lv_label_set_text(ui->screen_label_status3, "BW16/已连接");
              ui->bw16_dev->connect_status = true;
              ui->bw16_dev->switch_status = mqtt_get_rgb_data(queue_buff, &ui->bw16_dev->red, &ui->bw16_dev->green, &ui->bw16_dev->blue);

              if (ui->bw16_dev->switch_status) {
                lv_obj_clear_flag(guider_ui.screen_img_bw16_close, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(guider_ui.screen_img_bw16_open, LV_OBJ_FLAG_HIDDEN);

                lv_img_set_src(ui->screen_img_rgb3, &_RGB_close_alpha_32x32);
                lv_color_t rgb_color = {
                  .ch.red = ui->bw16_dev->red,
                  .ch.green = ui->bw16_dev->green,
                  .ch.blue = ui->bw16_dev->blue,
                };
                lv_obj_set_style_img_recolor_opa(ui->screen_img_rgb3, 255, 0);
                lv_obj_set_style_img_recolor(ui->screen_img_rgb3, rgb_color, _LV_STYLE_STATE_CMP_SAME);
              }
              else {

                lv_obj_clear_flag(guider_ui.screen_img_bw16_open, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(guider_ui.screen_img_bw16_close, LV_OBJ_FLAG_HIDDEN);
                lv_img_set_src(ui->screen_img_rgb3, &_RGB_open_alpha_32x32);
                lv_obj_set_style_img_recolor_opa(ui->screen_img_rgb3, 0, 0);
              }
            }
            else if (ui->screen_type==3) {
              ui->bw16_dev->switch_status = mqtt_get_rgb_data(queue_buff, &ui->bw16_dev->red, &ui->bw16_dev->green, &ui->bw16_dev->blue);
              if (ui->bw16_dev->switch_status) {
                lv_obj_add_state(ui->screen_rgb3_bw16_sw, LV_STATE_CHECKED);
                lv_obj_clear_flag(ui->screen_rgb3_cpicker_bw16_rgb, LV_OBJ_FLAG_HIDDEN);
                lv_color_t bw16_rgb_color = {
                  .ch.red = ui->bw16_dev->red,
                  .ch.green = ui->bw16_dev->green,
                  .ch.blue = ui->bw16_dev->blue
                };
                lv_colorwheel_set_rgb(ui->screen_rgb3_cpicker_bw16_rgb, bw16_rgb_color);
              }
              else {
                lv_obj_clear_state(ui->screen_rgb3_bw16_sw, LV_STATE_CHECKED);
                lv_obj_add_flag(ui->screen_rgb3_cpicker_bw16_rgb, LV_OBJ_FLAG_HIDDEN);
              }
            }
          }
          break;

          default:
            break;
        }
      }
      break;
      case CUSTOM_EVENT_MQTT_PUB_MSG: //MQTT 需要发布数据
      {
        if (mqtt_app_publish(MQTT_CLIETN_PUB_TOPIC, queue_buff, 0)==0)
          LOG_F("mqtt send :%s", queue_buff);
      }
      break;
      default:
        break;
    }
    vPortFree(queue_buff);
  }
}
/**
 * @brief queue_get_custom_event
 *      读取消息队列中的事件
 * @param json_data
 * @return custom_event_t
*/
static custom_event_t queue_get_custom_event(char* json_data)
{

  cJSON* root = cJSON_Parse(json_data);

  if (root==NULL) {
    LOG_E("\"%.*s\"is not json", strlen(json_data), json_data);
    return CUSTOM_EVENT_NONE;
  }
  cJSON* wifi = cJSON_GetObjectItem(root, "WiFi");
  if (wifi) {
    cJSON_Delete(root);
    return CUSTOM_EVENT_GET_WIFI;
  }
  cJSON* wifi_dis = cJSON_GetObjectItem(root, "wifi_disconnect");
  if (wifi) {
    cJSON_Delete(root);
    return CUSTOM_EVENT_GET_WIFI_DISCONNECT;
  }

  cJSON* ip = cJSON_GetObjectItem(root, "ip");
  if (ip) {
    cJSON_Delete(root);
    return CUSTOM_EVENT_GOT_IP;
  }

  cJSON* weather = cJSON_GetObjectItem(root, "weather");
  if (weather) {
    cJSON_Delete(root);
    return  CUSTOM_EVENT_GET_WEATHER;
  }

  cJSON* WIFI_SCAN = cJSON_GetObjectItem(root, "wifi_scan");
  if (WIFI_SCAN) {
    cJSON_Delete(root);
    return CUSTOM_EVENT_WIFI_SCAN_DONE;
  }

  cJSON* mqtt_connect = cJSON_GetObjectItem(root, "mqtt_connect");
  if (mqtt_connect) {
    cJSON_Delete(root);
    return CUSTOM_EVENT_MQTT_CONNECT;
  }

  cJSON* mqtt_disconnect = cJSON_GetObjectItem(root, "mqtt_disconnect");
  if (mqtt_disconnect) {
    cJSON_Delete(root);
    return CUSTOM_EVENT_MQTT_DISCONNECT;
  }

  cJSON* mqtt_msg = cJSON_GetObjectItem(root, "mqtt_msg");

  if (mqtt_msg) {
    cJSON_Delete(root);
    return CUSTOM_EVENT_MQTT_GET_MSG;
  }
  cJSON* mqtt_pub = cJSON_GetObjectItem(root, "RGB");
  if (mqtt_pub) {
    cJSON_Delete(root);
    return CUSTOM_EVENT_MQTT_PUB_MSG;
  }

  // cJSON* buttom_red = cJSON_GetObjectItem(root, "buttom_red");
  // if (buttom_red) {
  //   int status = buttom_red->valueint;
  //   cJSON_Delete(root);
  //   return status?CUSTOM_EVENT_BUTTOM_RED_HIGH:CUSTOM_EVENT_BUTTOM_RED_LOW;
  // }

  // cJSON* buttom_green = cJSON_GetObjectItem(root, "buttom_green");
  // if (buttom_green) {
  //   int status = buttom_green->valueint;
  //   cJSON_Delete(root);
  //   return status?CUSTOM_EVENT_BUTTOM_GREEN_HIGH:CUSTOM_EVENT_BUTTOM_GREEN_LOW;
  // }
  // cJSON* buttom_blue = cJSON_GetObjectItem(root, "buttom_blue");
  // if (buttom_blue) {
  //   int status = buttom_blue->valueint;
  //   cJSON_Delete(root);
  //   return status?CUSTOM_EVENT_BUTTOM_BLUE_HIGH:CUSTOM_EVENT_BUTTOM_BLUE_LOW;
  // }
  cJSON* buttom_all_on = cJSON_GetObjectItem(root, "buttom_all_on");
  if (buttom_all_on) {

    cJSON_Delete(root);
    return CUSTOM_EVENT_BUTTOM_ALL_ON;
  }
  cJSON* buttom_all_off = cJSON_GetObjectItem(root, "buttom_all_off");
  if (buttom_all_off) {
    cJSON_Delete(root);
    return CUSTOM_EVENT_BUTTOM_ALL_OFF;
  }
  cJSON_Delete(root);

  return 0;
}

/**
 * @brief cjson_analysis_wifi_scan
 *
 * @param json_data
 * @param ssid_arry SSID list
 * @return int
*/
static int cjson_analysis_wifi_scan(char* json_data)
{
  if (json_data==NULL) {
    return -1;
  }

  cJSON* root = cJSON_Parse(json_data);
  if (root==NULL) {
    LOG_E("<%s> is't JSON");
    return -1;
  }
  cJSON* root_scan = cJSON_GetObjectItem(root, "wifi_scan");
  cJSON* scan_status = cJSON_GetObjectItem(root_scan, "status");

  switch (scan_status->valueint)
  {
    case 1:
    {
      LOG_I("scan_status is start");

      cJSON_Delete(root);
      return scan_status->valueint;
    }
    /* code */
    break;
    case 0:
    {
      LOG_I("scan_status is done");
      cJSON_Delete(root);
      return scan_status->valueint;
    }
    break;
    default:
      break;
  }

  return 0;
}

/**
 * @brief 获取WiFi名称
 *
 * @param json_data
 * @return char*
*/
static char* cjson_analysis_ssid(char* json_data)
{
  static char* ssid_str;
  ssid_str = pvPortMalloc(32);
  memset(ssid_str, 0, 32);
  cJSON* root = cJSON_Parse(json_data);
  if (root==NULL) {
    LOG_I("[%s] is not json", json_data);
    return NULL;
  }
  cJSON* wifi = cJSON_GetObjectItem(root, "WiFi");
  if (wifi==NULL)return NULL;
  cJSON* ssid = cJSON_GetObjectItem(wifi, "ssid");
  memcpy(ssid_str, ssid->valuestring, sizeof(ssid->valuestring)*8);
  cJSON_Delete(root);
  return ssid_str;
}

/**
 * @brief 获取WiFi 密码
 *
 * @param json_data
 * @return char*
*/
static char* cjson_analysis_password(char* json_data)
{
  static char* pass_str;
  pass_str = pvPortMalloc(32);
  memset(pass_str, 0, 32);
  cJSON* root = cJSON_Parse(json_data);
  if (root==NULL) {
    LOG_I("[%s] is not json", json_data);
    return NULL;
  }
  cJSON* wifi = cJSON_GetObjectItem(root, "WiFi");
  if (wifi==NULL)return NULL;
  cJSON* password = cJSON_GetObjectItem(wifi, "password");
  memcpy(pass_str, password->valuestring, sizeof(password->valuestring)*8);
  cJSON_Delete(root);
  return pass_str;
}

/**
 * @brief 获取IP地址
 *
 * @param cjson_data
 * @return char*
*/
static char* cjson__analysis_ip(char* cjson_data)
{
  static char* IP_str;

  IP_str = pvPortMalloc(64);
  memset(IP_str, 0, 64);

  cJSON* root = cJSON_Parse(cjson_data);
  if (root==NULL) {
    LOG_I("[%s] is not json", cjson_data);
    return NULL;
  }
  cJSON* ip_s = cJSON_GetObjectItem(root, "ip");
  cJSON* ip = cJSON_GetObjectItem(ip_s, "IP");
  memcpy(IP_str, ip->valuestring, sizeof(ip->valuestring)*16);
  cJSON_Delete(root);
  return IP_str;
}

/**
 * @brief
 *
 * @param weather_data
*/
static int cjson_get_weather(char* weather_data)
{
  for (size_t i = 0; i < strlen(weather_data); i++)
  {
    printf("%c", weather_data[i]);
  }
  printf("\r\n");

  cJSON* root = cJSON_Parse(weather_data);
  if (root==NULL) {
    LOG_E("[%s] is not json\r\n", __func__);
    return -1;
  }
  cJSON* weather_cjson = cJSON_GetObjectItem(root, "weather");
  cJSON* errcode = cJSON_GetObjectItem(weather_cjson, "errcode");
  if (errcode!=NULL) {

    cJSON* errmsg = cJSON_GetObjectItem(weather_cjson, "errmsg");
    LOG_E("errcode=%d:%s", errcode->valueint, errmsg->valuestring);
    cJSON_Delete(root);
    return  -1;;
  }

  cJSON* city_cjsno = cJSON_GetObjectItem(weather_cjson, "city");//城市名称
  if (city_cjsno ==NULL) {

    LOG_E("[city_cjsno ] is not json\r\n");
    cJSON_Delete(root);
    return -1;;
  }

  cJSON* wea_today = cJSON_GetObjectItem(weather_cjson, "wea");//今天天气
  if (wea_today==NULL) {

    LOG_E("[wea_today] is not json\r\n");
    cJSON_Delete(root);
    return -1;;
  }

  memcpy(guider_ui.city, city_cjsno->valuestring, strlen(city_cjsno->valuestring));
  memcpy(guider_ui.waether, wea_today->valuestring, strlen(wea_today->valuestring));

  cJSON_Delete(root);
  return 0;

}
/**
 * @brief mqtt_analysis_topic_for_msg
 *      从消息中解析Topic
 * @param json_data
 * @return char*
*/
static mqtt_dev_type_t mqtt_analysis_type_for_msg(const char* json_data)
{
  char* data_type = json_data;

  cJSON* root = cJSON_Parse(json_data);
  if (root==NULL) {

    LOG_I("[%s] is not json", json_data);
    return MQTT_DEV_TYPE_NONE;
  }
  LOG_T(json_data);

  cJSON* MQTT_MSG = cJSON_GetObjectItem(root, "mqtt_msg");
  cJSON* MQTT_TOPIC = cJSON_GetObjectItem(MQTT_MSG, "topic");
  cJSON* MQTT_DATA = cJSON_GetObjectItem(MQTT_MSG, "data");
  //鱼缸设备
  cJSON* MQTT_HEADER = cJSON_GetObjectItem(MQTT_DATA, "header");
  if (MQTT_HEADER!=NULL) {
    cJSON* MQTT_TYPE = cJSON_GetObjectItem(MQTT_HEADER, "type");
    if (MQTT_TYPE!=NULL) {
      cJSON_Delete(root);
      return MQTT_DEV_TYPE_FISH;
    }
  }
  //RGB 设备
  cJSON* MQTT_BOARD = cJSON_GetObjectItem(MQTT_DATA, "board_id");
  if (MQTT_BOARD!=NULL)
  {
    cJSON_Delete(root);

    return MQTT_BOARD->valueint+1;
  }

  cJSON_Delete(root);
  return MQTT_DEV_TYPE_NONE;
}
/**
 * @brief mqtt_analysis_get_temperature_for_fish
 *
 * @param json_data
 * @return int
*/
static double mqtt_analysis_get_data_for_fish(char* json_data, char* data_paramName)
{
  char* data_type = json_data;

  cJSON* root = cJSON_Parse(data_type);
  if (root==NULL) {

    LOG_I("[%s] is not json", data_type);
    return MQTT_DEV_TYPE_NONE;
  }

  cJSON* MQTT_MSG = cJSON_GetObjectItem(root, "mqtt_msg");
  cJSON* MQTT_TOPIC = cJSON_GetObjectItem(MQTT_MSG, "topic");
  cJSON* MQTT_DATA = cJSON_GetObjectItem(MQTT_MSG, "data");
  //鱼缸设备
  cJSON* MQTT_HEADER = cJSON_GetObjectItem(MQTT_DATA, "header");
  cJSON* MQTT_ATTR = cJSON_GetObjectItem(MQTT_DATA, "attr");
  cJSON* MQTT_DATA_TYPE = cJSON_GetObjectItem(MQTT_ATTR, data_paramName);

  double data_value = MQTT_DATA_TYPE->valuedouble;
  cJSON_Delete(root);
  return data_value;
}
/**
 * @brief mqtt_get_rgb_data
 *
 * @param json_data queue json data
 * @param red red data 0~255
 * @param green green 0~255
 * @param blue
 * @return true
 * @return false
*/
static bool mqtt_get_rgb_data(char* json_data, uint8_t* red, uint8_t* green, uint8_t* blue)
{
  char* data_type = json_data;

  cJSON* root = cJSON_Parse(json_data);
  if (root==NULL) {

    LOG_I("[%s] is not json", json_data);
    return MQTT_DEV_TYPE_NONE;
  }
  LOG_T(json_data);
  cJSON* MQTT_MSG = cJSON_GetObjectItem(root, "mqtt_msg");
  cJSON* MQTT_TOPIC = cJSON_GetObjectItem(MQTT_MSG, "topic");
  cJSON* MQTT_DATA = cJSON_GetObjectItem(MQTT_MSG, "data");
  cJSON* MQTT_STATUS = cJSON_GetObjectItem(MQTT_DATA, "status");
  cJSON* MQTT_RGB = cJSON_GetObjectItem(MQTT_DATA, "RGB");
  if (MQTT_RGB!=NULL) {
    cJSON* RGB_R = cJSON_GetObjectItem(MQTT_RGB, "R");
    cJSON* RGB_G = cJSON_GetObjectItem(MQTT_RGB, "G");
    cJSON* RGB_B = cJSON_GetObjectItem(MQTT_RGB, "B");
    *red = (uint8_t)RGB_R->valueint;
    *green = (uint8_t)RGB_G->valueint;
    *blue = (uint8_t)RGB_B->valueint;
  }
  else {
    cJSON_Delete(root);
    return false;
  }
  bool swtich_value = MQTT_STATUS->valueint;
  cJSON_Delete(root);

  return swtich_value;
}