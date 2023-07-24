# -*- coding:utf-8 -*-
# coding=utf-8

import sys
import os
import codecs
import shutil
import random
import re
import json
import struct

reload(sys) 
sys.setdefaultencoding('utf-8')

REAL_PWM_NUM_3 = "PWM_NUM_1_A27"
REAL_PWM_NUM_4 = "PWM_NUM_2_A28"

GPIO_CONFIG_FILE = "./custom_config.json"
GPIO_CONFIG_CODE_FILE = "../../user/src/examples/hb_auto_gpio.c"
USER_CONFIG_H_FILE = "../../user/inc/user_config.h"

UART_DATA_NUM_MAP = {}
GPIO_DEF_VOLTAGE_MAP = {}

def _creat_include_code(code_file):
    code_file.write("#include \"user_config.h\"\n")
    code_file.write("#include \"user_event.h\"\n")
    code_file.write("#include \"user_gpio.h\"\n")
    code_file.write("#include \"user_player.h\"\n")
    code_file.write("#include \"user_pwm.h\"\n")
    code_file.write("#include \"user_timer.h\"\n")
    code_file.write("#include \"user_uart.h\"\n")
    code_file.write("\n")

def _creat_define_code(code_file):
    code_file.write("#define TAG \"auto_gpio\"\n")
    code_file.write("\n")
    code_file.write("#define UART_SEND_MAX      16\n")
    code_file.write("\n")

def _creat_struct_code(code_file):
    code_file.write("typedef struct {\n")
    code_file.write("  char  data[UART_SEND_MAX];\n")
    code_file.write("  int   len;\n")
    code_file.write("}uart_data_t;\n")
    code_file.write("\n")

def _creat_uart_buf_arry(code_file, action_config_arry):
    global UART_DATA_NUM_MAP
    count = 0
    code_file.write("const uart_data_t g_uart_buf[] = {\n")
    for action_config in action_config_arry:
        action = action_config["action"]
        try:
            for ctrl_config in action_config["ctrl_items"]:
                function = ctrl_config["function"]
                if (function[0:4] == "UART"):
                    UART_DATA_NUM_MAP[action] = count
                    count += 1
                    data = ctrl_config["params"]["data"]
                    data_split = data.split()
                    if (len(data_split) > 16):
                        print("Lehgth of UART data must less than 16")
                    code_file.write("  {{0x" + ", 0x".join(data_split) + "}, " + str(len(data_split)) + "}, //" + action + "\n")
        except Exception as e:
            print('action ctrl parse err:' + str(e))
    code_file.write("};\n")
    code_file.write("\n")

def _creat_global_veriable_code(code_file, action_config_arry):
    _creat_uart_buf_arry(code_file, action_config_arry)

def _get_real_pwm_num(pin_config_arry):
    global REAL_PWM_NUM_3
    global REAL_PWM_NUM_4
    for pin_config in pin_config_arry:
        function = pin_config["function"]
        pin_num = int(pin_config["pin"])
        if (function == "PWM3"):
            if (pin_num == 21):
                REAL_PWM_NUM_3 = "PWM_NUM_1_B2"
            elif (pin_num == 1):
                REAL_PWM_NUM_3 = "PWM_NUM_1_B0"
            elif (pin_num == 5):
                REAL_PWM_NUM_3 = "PWM_NUM_1_A27"
            else:
                print("Invalid pin num for PWM3: " + str(pin_num))
        elif (function == "PWM4"):
            if (pin_num == 22):
                REAL_PWM_NUM_4 = "PWM_NUM_2_B3"
            elif (pin_num == 2):
                REAL_PWM_NUM_4 = "PWM_NUM_2_B1"
            elif (pin_num == 6):
                REAL_PWM_NUM_4 = "PWM_NUM_2_A28"
            else:
                print("Invalid pin num for PWM4: " + str(pin_num))
        else:
            pass

def _creat_pwm_ctrl_code(code_file, cmd, function, command, params):
    if (int(function[3]) == 3):
        pwm_num = REAL_PWM_NUM_3;
    elif (int(function[3]) == 4):
        pwm_num = REAL_PWM_NUM_4;
    else:
        print("Unknown pwm num: " + function)
        return None
    if (command == "set"):
        duty = params["duty"]
        code_file.write("      user_pwm_change_duty(" + pwm_num + ", " + str(duty) + ");\n")
    elif (command == "increase"):
        duty = params["duty"]
        code_file.write("      user_pwm_duty_inc(" + pwm_num + ", " + str(duty) + ");\n")
    elif (command == "decrease"):
        duty = params["duty"]
        code_file.write("      user_pwm_duty_dec(" + pwm_num + ", " + str(duty) + ");\n")
    else:
        print("Unknown command: " + command)

def _creat_uart_ctrl_code(code_file, cmd, function, command, params):
    uart_num = int(function[4])
    if (function[6:] != "TX"):
        print("Unknow fucntion: " + function + ", support TX only for UART")
        return None
    if (uart_num != 1):
        print("Unknow fucntion: " + function + ", support UART1 only")
        return None
    if (command != "send"):
        print("Unknow command: " + command + ", support send only")
        return None
    code_file.write("      user_uart_send(g_uart_buf[" + str(UART_DATA_NUM_MAP[cmd]) + "].data, g_uart_buf[" + str(UART_DATA_NUM_MAP[cmd]) + "].len);\n")

def _get_gpio_pulse_config(pin_config_arry):
    global GPIO_DEF_VOLTAGE_MAP
    for pin_config in pin_config_arry:
        function = pin_config["function"]
        pin_num = int(pin_config["pin"])
        if (function[0:4] == "GPIO"):
            def_voltage = pin_config["params"]["def_voltage"]
            GPIO_DEF_VOLTAGE_MAP[function] = def_voltage

def _creat_gpio_ctrl_code(code_file, cmd, function, command, params):
    real_gpio_num = "GPIO_NUM_" + function[5:]
    if (command == "set"):
        voltage = params["voltage"]
        if (voltage == "low"):
            code_file.write("      user_gpio_set_value(" + real_gpio_num + ", 0);\n")
        elif (voltage == "high"):
            code_file.write("      user_gpio_set_value(" + real_gpio_num + ", 1);\n")
        else:
            print("Unknown voltage: " + voltage)
    elif (command == "pulse"):
        period = params["period"]
        times = params["times"]
        if (GPIO_DEF_VOLTAGE_MAP[function] == "low"):
            def_voltage = 0
        elif (GPIO_DEF_VOLTAGE_MAP[function] == "high"):
            def_voltage = 1
        else:
            print("Unknow def_voltage: " + GPIO_DEF_VOLTAGE_MAP[function])
            return None
        code_file.write("      user_sw_timer_gpio_pulse(" + real_gpio_num + ", " + str(period) + ", " + str(times) + ", " + str(def_voltage) + ");\n")
    else:
        print("Unknown command: " + command)

def _creat_cmd_ctrl_code(code_file, action_config_arry):
    cmd_count = 0
    for action_config in action_config_arry:
        cmd = action_config['action']
        # skip wakeup event
        if (cmd == 'wakeup_uni'):
            continue
        cmd_count += 1
        if (1 == cmd_count):
            code_file.write("    if (0 == uni_strcmp(setting->cmd, \"" + cmd + "\")) {\n")
        else:
            code_file.write(" else if (0 == uni_strcmp(setting->cmd, \"" + cmd + "\")) {\n")
        try:
            ctrl_config_arry = action_config['ctrl_items']
            for ctrl_config in ctrl_config_arry:
                function = ctrl_config['function']
                command = ctrl_config['command']
                params = ctrl_config['params']
                if (function[0:3] == "PWM"):
                    _creat_pwm_ctrl_code(code_file, cmd, function, command, params)
                elif (function[0:4] == "UART"):
                    _creat_uart_ctrl_code(code_file, cmd, function, command, params)
                elif (function[0:4] == "GPIO"):
                    _creat_gpio_ctrl_code(code_file, cmd, function, command, params)
                else:
                    print("Unknown function: " + function)
        except Exception as e:
            print('action ctrl parse err:' + str(e))
        code_file.write("    }")
    if (cmd_count > 0):
        code_file.write(" else {\n")
        code_file.write("      LOGT(TAG, \"Unconcerned command: %s\", setting->cmd);\n")
        code_file.write("    }\n")

def _creat_custom_setting_cb_code(code_file, action_config_arry):
    code_file.write("static void _custom_setting_cb(USER_EVENT_TYPE event,\n")
    code_file.write("                               user_event_context_t *context) {\n")
    code_file.write("  event_custom_setting_t *setting = NULL;\n")
    code_file.write("  if (context) {\n")
    code_file.write("    setting = &context->custom_setting;\n")
    code_file.write("    LOGT(TAG, \"user command: %s\", setting->cmd);\n")
    _creat_cmd_ctrl_code(code_file, action_config_arry)
    code_file.write("    user_player_reply_list_random(setting->reply_files);\n")
    code_file.write("  }\n")
    code_file.write("}\n")
    code_file.write("\n")

def _creat_goto_wakeup_cb_code(code_file, action_config_arry):
    code_file.write("static void _goto_awakened_cb(USER_EVENT_TYPE event,\n")
    code_file.write("                               user_event_context_t *context) {\n")
    code_file.write("  event_goto_awakend_t *awkened = NULL;\n")
    code_file.write("  if (context) {\n")
    code_file.write("    awkened = &context->goto_awakend;\n")
    for action_config in action_config_arry:
        cmd = action_config['action']
        if (cmd == 'wakeup_uni'):
            try:
                ctrl_config_arry = action_config['ctrl_items']
                for ctrl_config in ctrl_config_arry:
                    function = ctrl_config['function']
                    command = ctrl_config['command']
                    params = ctrl_config['params']
                    if (function[0:3] == "PWM"):
                        _creat_pwm_ctrl_code(code_file, cmd, function, command, params)
                    elif (function[0:4] == "UART"):
                        _creat_uart_ctrl_code(code_file, cmd, function, command, params)
                    elif (function[0:4] == "GPIO"):
                        _creat_gpio_ctrl_code(code_file, cmd, function, command, params)
                    else:
                        print("Unknown function: " + function)
            except Exception as e:
                print('action ctrl parse err:' + str(e))
    code_file.write("    user_player_reply_list_random(awkened->reply_files);\n")
    code_file.write("  }\n")
    code_file.write("}\n")
    code_file.write("\n")

def _is_have_wakeup_event(code_file, action_config_arry):
    for action_config in action_config_arry:
        cmd = action_config['action']
        if (cmd == 'wakeup_uni'):
            return True
    return False

def _creat_event_cb_code(code_file, action_config_arry):
    code_file.write("static void _register_event_callback(void) {\n")
    code_file.write("  user_event_subscribe_event(USER_CUSTOM_SETTING, _custom_setting_cb);\n")
    if (_is_have_wakeup_event(code_file, action_config_arry)):
        code_file.write("  user_event_subscribe_event(USER_GOTO_AWAKENED, _goto_awakened_cb);\n")
    code_file.write("}\n")
    code_file.write("\n")

def _creat_gpio_init_code(code_file, pin_config_arry):
    code_file.write("  user_gpio_init();\n")
    for pin_config in pin_config_arry:
        function = pin_config["function"]
        if (function[0:4] == "GPIO"):
            def_voltage = pin_config["params"]["def_voltage"]
            real_gpio_num = "GPIO_NUM_" + function[5:]
            code_file.write("  user_gpio_set_mode(" + real_gpio_num + ", GPIO_MODE_OUT);\n")
            if (def_voltage == "low"):
                code_file.write("  user_gpio_set_value(" + real_gpio_num + ", 0);\n")
            elif (def_voltage == "high"):
                code_file.write("  user_gpio_set_value(" + real_gpio_num + ", 1);\n")
            else:
                print("Unknow def_voltage: " + def_voltage)

def _creat_pwm_init_code(code_file, pin_config_arry):
    for pin_config in pin_config_arry:
        function = pin_config["function"]
        if (function[0:3] == "PWM"):
            frequency = pin_config["params"]["frequency"]
            reverse_voltage = pin_config["params"]["reverse_voltage"]
            if (reverse_voltage == "enable"):
                is_high_duty = "false"
            elif (reverse_voltage == "disable"):
                is_high_duty = "true"
            else:
                print("Unknown reverse_voltage: " + reverse_voltage)
                continue
            reverse_voltage = pin_config["params"]["reverse_voltage"]
            if (function == "PWM3"):
                pwm_num = REAL_PWM_NUM_3
            elif (function == "PWM4"):
                pwm_num = REAL_PWM_NUM_4
            else:
                print("Unknown PWM num: " + function)
                continue
            code_file.write("  user_pwm_init(" + pwm_num + ", " + str(frequency) + ", " + is_high_duty + ");\n")
            code_file.write("  user_pwm_start(" + pwm_num + ", 0);\n")

def _creat_uart_init_code(code_file, pin_config_arry):
    for pin_config in pin_config_arry:
        pin_num = int(pin_config["pin"])
        function = pin_config["function"]
        if (function[0:4] == "UART"):
            uart_num = int(function[4])
            if (function[6:] != "TX"):
                continue
            if (uart_num != 1):
                continue
            baud_rate = pin_config["params"]["baud_rate"]
            data_bits = pin_config["params"]["data_bits"]
            stop_bits = pin_config["params"]["stop_bits"]
            parity_bits = pin_config["params"]["parity_bits"]
            os.system("sed -i -e \"/USE_UNIONE_PROTOCOL/s/ 1 / 0 /\" " + USER_CONFIG_H_FILE)
            os.system("sed -i -e \"/USE_SAMPLE_PROTOCOL/s/ 1 / 0 /\" " + USER_CONFIG_H_FILE)
            if (pin_num == 21):
                os.system("sed -i -e \"/USER_UART_PIN_SELECT/s/ 0 / 2 /\" " + USER_CONFIG_H_FILE)
            elif (pin_num == 1):
                os.system("sed -i -e \"/USER_UART_PIN_SELECT/s/ 0 / 1 /\" " + USER_CONFIG_H_FILE)
            elif (pin_num == 8):
                os.system("sed -i -e \"/USER_UART_PIN_SELECT/s/ 0 / 0 /\" " + USER_CONFIG_H_FILE)
            else:
                print("Unknown pin num: " + str(pin_num))
                continue
            os.system("sed -i -e \"/USER_UART_BAUD_RATE/s/ 19200 / " + str(baud_rate) + "  /\" " + USER_CONFIG_H_FILE)
            os.system("sed -i -e \"/USER_UART_DATA_BIT/s/8/" + str(data_bits) + "/\" " + USER_CONFIG_H_FILE)
            os.system("sed -i -e \"/USER_UART_STOP_BIT/s/ 1 / " + str(stop_bits) + " /\" " + USER_CONFIG_H_FILE)
            if (parity_bits == "none"):
                os.system("sed -i -e \"/USER_UART_PARITY/s/ 0 / 0 /\" " + USER_CONFIG_H_FILE)
            elif (parity_bits == "odd"):
                os.system("sed -i -e \"/USER_UART_PARITY/s/ 0 / 1 /\" " + USER_CONFIG_H_FILE)
            elif (parity_bits == "even"):
                os.system("sed -i -e \"/USER_UART_PARITY/s/ 0 / 2 /\" " + USER_CONFIG_H_FILE)
            code_file.write("  user_uart_init(NULL);\n")

def _creat_init_code(code_file, pin_config_arry):
    code_file.write("int hb_auto_gpio(void) {\n")
    _creat_gpio_init_code(code_file, pin_config_arry)
    _creat_pwm_init_code(code_file, pin_config_arry)
    _creat_uart_init_code(code_file, pin_config_arry)
    code_file.write("  _register_event_callback();\n")
    code_file.write("  return 0;\n")
    code_file.write("}\n")
    code_file.write("\n")

# create gpio config demo code
def _create_gpio_config_code(ctrl):
    if (ctrl == "auto"):
        code_file = open(GPIO_CONFIG_CODE_FILE, "w")
        json_file = open(GPIO_CONFIG_FILE, "r")
        config_json = json.load(json_file)
        try:
            pin_config_arry = config_json["local"]["pin_config"]
        except Exception as e:
            print('pin_config prase err:' + str(e))
            code_file.write("int hb_auto_gpio(void) {\n  return 0;\n}\n")
            code_file.close()
            json_file.close()
            return None
        try:
            action_config_arry = config_json["local"]["action_config"]
        except Exception as e:
            print('action_config prase err:' + str(e))
            action_config_arry = []
        _get_real_pwm_num(pin_config_arry)
        _get_gpio_pulse_config(pin_config_arry)
        _creat_include_code(code_file)
        _creat_define_code(code_file)
        _creat_struct_code(code_file)
        _creat_global_veriable_code(code_file, action_config_arry)
        _creat_custom_setting_cb_code(code_file, action_config_arry)
        if (_is_have_wakeup_event(code_file, action_config_arry)):
            _creat_goto_wakeup_cb_code(code_file, action_config_arry)
        _creat_event_cb_code(code_file, action_config_arry)
        _creat_init_code(code_file, pin_config_arry);
        code_file.close()
        json_file.close()