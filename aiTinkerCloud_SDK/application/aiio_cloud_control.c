/**
 * @brief   Define the cloud  control the attribute state interface
 * 
 * @file    aiio_cloud_control.c
 * @copyright Copyright (C) 2020-2023, Shenzhen Anxinke Technology Co., Ltd
 * @note    This is mainly destribing the attribute state that is received from cloud, And the different type is come from different command interface
 * @par Change Logs:
 * <table>
 * <tr><th>Date               <th>Version             <th>Author           <th>Notes
 * <tr><td>2023-06-16          <td>1.0.0            <td>zhuolm             <td> The  different command interface of control attribute state.
 */
#include "aiio_cloud_control.h"
// #include "aiio_cloud_led.h"
#include "aiio_log.h"
#include "aiio_wan_service.h"
#include "config.h"


static bool powerstate_status = false;
static unsigned char spray_mode_status = 0;
static unsigned long spray_mode_stepless_status = 0;
static bool light_switch_status = false;
static unsigned long light_brightness_status = 0;
static unsigned char light_mode_status = 0;
static unsigned long light_builtin_status = 0;
static unsigned long clean_time_status = 0;
static bool clean_reset_status = false;
static bool clean_tip_switch_status = false;
static bool clean_tip_status = false;
static bool oil_tip_status = false;
static bool water_tip_status = false;
static unsigned long rest_screen_time_status = 0;
static bool rest_screen_switch_status = false;
static bool low_battery_tip_status = false;
static unsigned long remain_electricity_status = 0;
static unsigned long remain_water_status = 0;
static unsigned char aroma_status = 0;
static unsigned char play_switch_status = 0;
static unsigned long play_music_status = 0;
static unsigned char music_list_status = 0;
static unsigned char play_list_status = 0;
static unsigned long play_volume_status = 0;
static unsigned char play_mode_status = 0;
static unsigned char play_next_status = 0;


static void aiio_receive_bool_cmd(char *msgMid, char *from, uint8_t dpid, bool cmd);
static void aiio_receive_string_cmd(char *msgMid, char *from, uint8_t dpid, char *cmd);
static void aiio_receive_value_cmd(char *msgMid, char *from, uint8_t dpid, int value);
static void aiio_receive_array_cmd(char *msgMid, char *from, uint8_t dpid, cJSON *cmd);
static void aiio_receive_object_cmd(char *msgMid, char *from, uint8_t dpid, cJSON *cmd);



void aiio_parse_control_data(char *msgMid, char *from, char *control_data)
{
    cJSON *cjson_root   = NULL;
    cJSON *cjson_control = NULL;
    uint16_t cmd_num = 0;
    cJSON *cjson_value   = NULL;
    char  str[10] = {0};

    if(control_data == NULL)
    {
        aiio_log_e("param err \r\n");
        return ;
    }

    aiio_log_d("control_data = %s \r\n", control_data);
    cjson_root = cJSON_Parse(control_data);
    if(cjson_root == NULL)
    {
        aiio_log_e("json parse err \r\n");
        return ;
    }

    cjson_control = cJSON_GetObjectItem(cjson_root, PROTOCOL_STR_CONTROL);
    if(cjson_control == NULL)
    {
        aiio_log_e("cjson_control is NULL \r\n");
        cJSON_Delete(cjson_root);
        return ;
    }

    if(cjson_control)
    {
        for(cmd_num = 1; cmd_num < 256; cmd_num++)                              /*  Check the attribute command by polling that is received from cloud, The range of attribute command value is [1 - 255]*/
        {
            snprintf(str, 10, "%d", cmd_num);
            // aiio_log_d("dpid = %s \r\n", str);

            cjson_value = cJSON_GetObjectItem(cjson_control, str);
            if(cjson_value)
            {
                switch (cjson_value->type)
                {
                    case cJSON_Number:
                    {
                        aiio_log_i("receive value data\r\n");
                        aiio_receive_value_cmd(msgMid, from, cmd_num, cJSON_GetNumberValue(cjson_value));
                    }
                    break;

                    case cJSON_String:
                    {
                        aiio_log_i("receive string data\r\n");
                        aiio_receive_string_cmd(msgMid, from, cmd_num, cJSON_GetStringValue(cjson_value));
                    }
                    break;

                    case cJSON_Array:
                    {
                        aiio_log_i("receive array data \r\n");
                        aiio_receive_array_cmd(msgMid, from, cmd_num, cjson_value);
                    }
                    break;

                    case cJSON_Object:
                    {
                        aiio_log_i("receive object data \r\n");
                        aiio_receive_object_cmd(msgMid, from, cmd_num, cjson_value);
                    }
                    break;

                    case cJSON_True:
                    {
                        aiio_log_i("receive bool data \r\n");
                        aiio_receive_bool_cmd(msgMid, from, cmd_num, true);
                    }
                    break;

                    case cJSON_False:
                    {
                        aiio_log_i("receive bool data \r\n");
                        aiio_receive_bool_cmd(msgMid, from, cmd_num, false);
                    }
                    break;
                    
                    default:
                        aiio_log_e("can't find cjson type \r\n");
                        break;
                }
            }
        }
    }

    cJSON_Delete(cjson_root);
}



static void aiio_receive_bool_cmd(char *msgMid, char *from, uint8_t dpid, bool cmd)
{
    switch (dpid)
    {
        case CMD_POWERSTATE:
        {
            if(cmd)
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_ON);
                powerstate_status = true;
            }
            else
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_OFF);
                powerstate_status = false;
            }

            aiio_report_bool_attibute_status(msgMid, from, dpid, cmd);
        }
        break;

        case CMD_LIGHT_SWITCH:
        {
            if(cmd)
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_ON);
                light_switch_status = true;
            }
            else
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_OFF);
                light_switch_status = false;
            }

            aiio_report_bool_attibute_status(msgMid, from, dpid, cmd);
        }
        break;

        case CMD_CLEAN_RESET:
        {
            if(cmd)
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_ON);
                clean_reset_status = true;
            }
            else
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_OFF);
                clean_reset_status = false;
            }

            aiio_report_bool_attibute_status(msgMid, from, dpid, cmd);
        }
        break;

        case CMD_CLEAN_TIP_SWITCH:
        {
            if(cmd)
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_ON);
                clean_tip_switch_status = true;
            }
            else
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_OFF);
                clean_tip_switch_status = false;
            }

            aiio_report_bool_attibute_status(msgMid, from, dpid, cmd);
        }
        break;

        case CMD_CLEAN_TIP:
        {
            if(cmd)
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_ON);
                clean_tip_status = true;
            }
            else
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_OFF);
                clean_tip_status = false;
            }

            aiio_report_bool_attibute_status(msgMid, from, dpid, cmd);
        }
        break;

        case CMD_OIL_TIP:
        {
            if(cmd)
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_ON);
                oil_tip_status = true;
            }
            else
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_OFF);
                oil_tip_status = false;
            }

            aiio_report_bool_attibute_status(msgMid, from, dpid, cmd);
        }
        break;

        case CMD_WATER_TIP:
        {
            if(cmd)
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_ON);
                water_tip_status = true;
            }
            else
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_OFF);
                water_tip_status = false;
            }

            aiio_report_bool_attibute_status(msgMid, from, dpid, cmd);
        }
        break;

        case CMD_REST_SCREEN_SWITCH:
        {
            if(cmd)
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_ON);
                rest_screen_switch_status = true;
            }
            else
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_OFF);
                rest_screen_switch_status = false;
            }

            aiio_report_bool_attibute_status(msgMid, from, dpid, cmd);
        }
        break;

        case CMD_LOW_BATTERY_TIP:
        {
            if(cmd)
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_ON);
                low_battery_tip_status = true;
            }
            else
            {
                //aiio_turn_on_led_status(ALL_LED_TURN_OFF);
                low_battery_tip_status = false;
            }

            aiio_report_bool_attibute_status(msgMid, from, dpid, cmd);
        }
        break;
        
        default:
            aiio_log_e("can't find dpid[%d]", dpid);
            break;
    }
}




static void aiio_receive_string_cmd(char *msgMid, char *from, uint8_t dpid, char *cmd)
{
    switch (dpid)
    {
        case CMD_PLAY_LIST:
        {
            aiio_log_i("receive :%s \r\n", cmd);
            aiio_report_string_attibute_status(msgMid, from, dpid, cmd);
        }
        break;

        default:
            aiio_log_e("can't find dpid[%d]", dpid);
            break;
    }
}


static void aiio_receive_value_cmd(char *msgMid, char *from, uint8_t dpid, int value)
{
    switch (dpid)
    {
        case CMD_SPRAY_MODE:
        {
            aiio_log_i("receive :%d \r\n", value);
            spray_mode_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_LIGHT_BRIGHTNESS:
        {
            aiio_log_i("receive :%d \r\n", value);
            light_brightness_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_LIGHT_MODE:
        {
            aiio_log_i("receive :%d \r\n", value);
            light_mode_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_LIGHT_BUILTIN:
        {
            aiio_log_i("receive :%d \r\n", value);
            light_builtin_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_CLEAN_TIME:
        {
            aiio_log_i("receive :%d \r\n", value);
            clean_time_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_REST_SCREEN_TIME:
        {
            aiio_log_i("receive :%d \r\n", value);
            rest_screen_time_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_REMAIN_ELECTRICITY:
        {
            aiio_log_i("receive :%d \r\n", value);
            remain_electricity_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_REMAIN_WATER:
        {
            aiio_log_i("receive :%d \r\n", value);
            remain_water_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_AROMA:
        {
            aiio_log_i("receive :%d \r\n", value);
            aroma_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_PLAY_SWITCH:
        {
            aiio_log_i("receive :%d \r\n", value);
            play_switch_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_MUSIC_LIST:
        {
            aiio_log_i("receive :%d \r\n", value);
            music_list_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_PLAY_VOLUME:
        {
            aiio_log_i("receive :%d \r\n", value);
            play_volume_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_PLAY_MODE:
        {
            aiio_log_i("receive :%d \r\n", value);
            play_mode_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        case CMD_PLAY_NEXT:
        {
            aiio_log_i("receive :%d \r\n", value);
            play_next_status = value;
            aiio_report_int_attibute_status(msgMid, from, dpid, value);
        }
        break;

        default:
            aiio_log_e("can't find dpid[%d]", dpid);
            break;
    }
}


static void aiio_receive_array_cmd(char *msgMid, char *from, uint8_t dpid, cJSON *cmd)
{
    
}



static void aiio_receive_object_cmd(char *msgMid, char *from, uint8_t dpid, cJSON *cmd)
{
    
}


void aiio_report_bool_attibute_status(char *msgMid, char *from, uint8_t dpid, bool status)
{
    cJSON  *json_device = NULL;
    char dpid_str[10] = {0};
    char  *json_str = NULL;

    json_device = cJSON_CreateObject();
    if (!json_device)
    {
        aiio_log_e("json create object fail \r\n");
        return ;
    }

    snprintf(dpid_str, sizeof(dpid_str), "%d", dpid);

    if(status)
    {
        cJSON_AddTrueToObject(json_device, dpid_str);
    }
    else
    {
        cJSON_AddFalseToObject(json_device, dpid_str);
    }

    json_str = cJSON_PrintUnformatted(json_device);
    if(json_str == NULL)
    {
        aiio_log_e("json create str fail \r\n");
        cJSON_Delete(json_device);
        return ;
    }
#ifdef CONFIG_WAN_NETWORK_MODULE
    aiio_service_report_attribute(msgMid, from, json_str);
#endif
    cJSON_Delete(json_device);
    cJSON_free(json_str);
}


void aiio_report_string_attibute_status(char *msgMid, char *from, uint8_t dpid, char* value)
{
    cJSON  *json_device = NULL;
    char dpid_str[10] = {0};
    char  *json_str = NULL;

    if(value == NULL)
    {
        aiio_log_e("param err \r\n");
        return ;
    }

    json_device = cJSON_CreateObject();
    if (!json_device)
    {
        aiio_log_e("json create object fail \r\n");
        return ;
    }

    snprintf(dpid_str, sizeof(dpid_str), "%d", dpid);

    cJSON_AddStringToObject(json_device, dpid_str, value);

    json_str = cJSON_PrintUnformatted(json_device);
    if(json_str == NULL)
    {
        aiio_log_e("json create str fail \r\n");
        cJSON_Delete(json_device);
        return ;
    }
#ifdef CONFIG_WAN_NETWORK_MODULE
    aiio_service_report_attribute(msgMid, from, json_str);
#endif
    cJSON_Delete(json_device);
    cJSON_free(json_str);
}

void aiio_report_int_attibute_status(char *msgMid, char *from, uint8_t dpid, int value)
{
    cJSON  *json_device = NULL;
    char dpid_str[10] = {0};
    char  *json_str = NULL;

    json_device = cJSON_CreateObject();
    if (!json_device)
    {
        aiio_log_e("json create object fail \r\n");
        return ;
    }

    snprintf(dpid_str, sizeof(dpid_str), "%d", dpid);

    cJSON_AddNumberToObject(json_device, dpid_str, value);

    json_str = cJSON_PrintUnformatted(json_device);
    if(json_str == NULL)
    {
        aiio_log_e("json create str fail \r\n");
        cJSON_Delete(json_device);
        return ;
    }
#ifdef CONFIG_WAN_NETWORK_MODULE
    aiio_service_report_attribute(msgMid, from, json_str);
#endif
    cJSON_Delete(json_device);
    cJSON_free(json_str);
}


static int32_t aiio_packet_json_bool_value(cJSON * json_root, uint8_t dpid, bool value)
{
    char dpid_str[10] = {0};

    if(json_root == NULL)
    {
        aiio_log_e("param err \r\n");
        return -1;
    }

    snprintf(dpid_str, sizeof(dpid_str), "%d", dpid);

    if(value)
    {
        cJSON_AddTrueToObject(json_root, dpid_str);
    }
    else
    {
        cJSON_AddFalseToObject(json_root, dpid_str);
    }

    return 0;
}


static int32_t aiio_packet_json_int_value(cJSON * json_root, uint8_t dpid, int value)
{
    char dpid_str[10] = {0};

    if(json_root == NULL)
    {
        aiio_log_e("param err \r\n");
        return -1;
    }
    snprintf(dpid_str, sizeof(dpid_str), "%d", dpid);

    cJSON_AddNumberToObject(json_root, dpid_str, value);

    return 0;
}


static int32_t aiio_packet_json_string_value(cJSON * json_root, uint8_t dpid, char* value)
{
    char dpid_str[10] = {0};

    if(json_root == NULL)
    {
        aiio_log_e("param err \r\n");
        return -1;
    }
    snprintf(dpid_str, sizeof(dpid_str), "%d", dpid);

    cJSON_AddStringToObject(json_root, dpid_str, value);

    return 0;
}


void aiio_report_all_attibute_status(char *msgMid, char *from)
{
    cJSON  *json_device = NULL;
    char  *json_str = NULL;

    json_device = cJSON_CreateObject();
    if (!json_device)
    {
        aiio_log_e("json create object fail \r\n");
        return ;
    }

    aiio_packet_json_bool_value(json_device, CMD_POWERSTATE, powerstate_status);
    aiio_packet_json_bool_value(json_device, CMD_LIGHT_SWITCH, light_switch_status);
    aiio_packet_json_bool_value(json_device, CMD_CLEAN_RESET, clean_reset_status);
    aiio_packet_json_bool_value(json_device, CMD_CLEAN_TIP_SWITCH, clean_tip_switch_status);
    aiio_packet_json_bool_value(json_device, CMD_CLEAN_TIP, clean_tip_status);
    aiio_packet_json_bool_value(json_device, CMD_OIL_TIP, oil_tip_status);
    aiio_packet_json_bool_value(json_device, CMD_WATER_TIP, water_tip_status);
    aiio_packet_json_bool_value(json_device, CMD_REST_SCREEN_SWITCH, rest_screen_switch_status);
    aiio_packet_json_bool_value(json_device, CMD_LOW_BATTERY_TIP, low_battery_tip_status);

    aiio_packet_json_int_value(json_device, CMD_SPRAY_MODE, spray_mode_status);
    aiio_packet_json_int_value(json_device, CMD_LIGHT_BRIGHTNESS, light_brightness_status);
    aiio_packet_json_int_value(json_device, CMD_LIGHT_MODE, light_mode_status);
    aiio_packet_json_int_value(json_device, CMD_LIGHT_BUILTIN, light_builtin_status);
    aiio_packet_json_int_value(json_device, CMD_CLEAN_TIME, clean_time_status);
    aiio_packet_json_int_value(json_device, CMD_REST_SCREEN_TIME, rest_screen_time_status);
    aiio_packet_json_int_value(json_device, CMD_REMAIN_ELECTRICITY, remain_electricity_status);
    aiio_packet_json_int_value(json_device, CMD_REMAIN_WATER, remain_water_status);
    aiio_packet_json_int_value(json_device, CMD_AROMA, aroma_status);
    aiio_packet_json_int_value(json_device, CMD_PLAY_SWITCH, play_switch_status);
    aiio_packet_json_int_value(json_device, CMD_PLAY_MUSIC, play_music_status);
    aiio_packet_json_int_value(json_device, CMD_MUSIC_LIST, music_list_status);
    aiio_packet_json_int_value(json_device, CMD_PLAY_VOLUME, play_volume_status);
    aiio_packet_json_int_value(json_device, CMD_PLAY_MODE, powerstate_status);
    aiio_packet_json_int_value(json_device, CMD_PLAY_NEXT, play_next_status);

    aiio_packet_json_string_value(json_device, CMD_PLAY_LIST, "zlm");

    json_str = cJSON_PrintUnformatted(json_device);
    if(json_str == NULL)
    {
        aiio_log_e("json create str fail \r\n");
        cJSON_Delete(json_device);
        return ;
    }
#ifdef CONFIG_WAN_NETWORK_MODULE
    aiio_service_report_attribute(msgMid, from, json_str);
#endif
    cJSON_Delete(json_device);
    cJSON_free(json_str);
    
}
