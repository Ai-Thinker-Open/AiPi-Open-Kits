# -*- coding:utf-8 -*-
# coding=utf-8

import sys
import os
import codecs
import shutil
import random
import re
import json

from collections import OrderedDict
import auto_code

reload(sys) 
sys.setdefaultencoding('utf-8')

"""
1. PCM文件命名规则:
    所有文件PCM名字必须以纯数字命名。
    1 ~ 100 预留给系统特殊用途的PCM文件
    101 ~ 用命名配置产生的PCM文件。是根据配置文件cmd_content_total生成的PCM文件

2. cmd_content_total文件规则
    2.1 格式
        "="之前是命令名称，之后是命令词；@之后是回复语，没有回复语时也没有‘@’字符；
        多条命令词用"|"隔开；多条回复语用"|"隔开。

        命令    命令词        回复语（可无，没有回复语时也没有‘@’字符）
        xxx_cmd=命令词1|命令词2@回复语1|回复语2
        xxx_cmd=命令词1|命令词2
    2.2
        cmd_content_total文件应包含所有命令词。
        以”uni“为后缀的命令词是系统默认处理的命令词。
        asr_timeout_uni是虚拟命令词，用于关联识别超时回复用
"""

DEFAULT_PCM_DIR = "./default_tones/"
PCM_DIR = "./tones/"
WAV_PCM_DIR = "./wav_tones/"
PCM_BASE_ADDR = 0x0L    #必须是4K的整数倍
PCM_BIN = "./pcm.bin"
TARGET_PCM_ARRAY_FILE = "../../include/uni_pcm_array.h"
TARGET_PCM_DTAT_FILE = "../../include/pcm_bin.h"

ASRFIX_DAT = "./asrfix.dat"
TARGET_ASRFIX_DATA_FILE = "../../include/asrfix.h"

UNI_CONFIG = "./config.bin"
UNI_CONFIG_DEBUG = "./config_debug.bin"
UNI_CONFIG_RELEASE = "./config_release.bin"
TARGET_UNI_CONFIG_FILE = "../../include/config_bin.h"

GRAMMAR_DATA = "./grammar.dat"
TARGET_GRAMMAR_DATA_FILE = "../../include/grammar.h"

AIK_CONFIG = "./aik.json"
AIK_CONFIG_DEBUG = "./aik_debug.json"
AIK_CONFIG_RELEASE = "./aik_release.json"
TARGET_AIK_CONFIG_FILE = "../../include/aik-config.h"

CUSTOM_CONFIG_FILE = "./custom_config.json"
TARGET_CUST_CONFIG_H_FILE = "../../include/uni_cust_config.h"

CMD_CONTENT_FILE = "./cmd_reply_data.json"
TARGET_NUL_CONTENT_H_FILE = "../../include/uni_nlu_content.h"

CMD_CODE_START_NUM = 0x00
TARGET_SUCP_CONT_FILE = "./sucp_cmd_map.txt"
TARGET_CMD_CODE_H_FILE = "../../include/uni_cmd_code.h"

THRESH_DAT_FILE = "./thresh.dat"
ASR_THRESH_DAT_FILE = "./asr_thresh.dat"

# bin file transform to array
def _bin_2_array(bin_path, arrayName, inc_path):
    bin_file = open(bin_path, "rb")
    inc_file = open(inc_path, "w")
    inc_file.write("const unsigned char " + arrayName + "[] = {\n")
    while True:
        data = bin_file.read(16) # 每一次读取16个字符
        if not data:
            break
        str_line = "    " #空出4个空格
        for i in data:
            str_line += "0x{0:02x}".format(ord(i)) + ", "
        inc_file.write(str_line + "\n")
    inc_file.write("};\n")
    bin_file.close()
    inc_file.close()

def _get_mp3_filelist(dir):
    file_list = []
    numbers = []
    for home, dirs, files in os.walk(dir):
        cnt = 0
        for filename in files:
            file_name = os.path.splitext(filename)
            if (file_name[1] == ".mp3"):
                idx = int(file_name[0])
                numbers.append(idx)
            # 文件名列表，包含完整路径
            #file_list.append(os.path.join(home, filename))
            # 文件名列表，只包含文件名
    numbers.sort()
    for idx in numbers:
        filename = str(idx) + '.mp3'
        file_list.append(filename)
    return file_list

def _copy_default_tons():
    file_list = _get_mp3_filelist(DEFAULT_PCM_DIR)
    for file in file_list:
        file_name = os.path.splitext(file)
        if (file_name[1] == ".mp3"):
            org_file = os.path.join(DEFAULT_PCM_DIR, file)
            dst_file = os.path.join(PCM_DIR, file)
            shutil.copyfile(org_file, dst_file)

def _wav_2_mp3_tons():
    for home, dirs, files in os.walk(WAV_PCM_DIR):
        for filename in files:
            wav_file = os.path.splitext(filename)
            if (wav_file[1] == ".wav"):
                mp3_file = wav_file[0] + ".mp3"
                wav_file_path = os.path.join(WAV_PCM_DIR, filename)
                mp3_file_path = os.path.join(PCM_DIR, mp3_file)
                os.system("lame -b 16 " + wav_file_path + " " + mp3_file_path)

# create pcm bin and array file
def _create_pcm_bin_array(ctrl):
    flash_file = open(PCM_BIN, "wb")
    inc_file = open(TARGET_PCM_ARRAY_FILE, "w")
    _copy_default_tons()
    if (ctrl == "auto"):
        _wav_2_mp3_tons()
    file_list = _get_mp3_filelist(PCM_DIR)
    inc_file.write("#ifndef INC_UNI_PCM_ARRY_H_\n")
    inc_file.write("#define INC_UNI_PCM_ARRY_H_\n")
    inc_file.write("\n")
    inc_file.write("typedef struct {\n")
    inc_file.write("  uni_u32 number;\n")
    inc_file.write("  uni_u32 offset;\n")
    inc_file.write("  uni_u32 len;\n")
    inc_file.write("}PCM_RESOURCE;\n")
    inc_file.write("\n")
    inc_file.write("const PCM_RESOURCE g_pcm_arry[] = {\n")
    flash_addr = PCM_BASE_ADDR;
    pcm_file_size = 0
    for file in file_list:
        file_name = os.path.splitext(file)
        if (file_name[1] == ".mp3"):
            file = os.path.join(PCM_DIR, file)
            file_size = os.path.getsize(file)
            pcm_file = open(file, "rb");
            flash_file.write(pcm_file.read())
            pcm_file_size += file_size
            if (file_size % 64):
                fill_num = 64 - (file_size % 64)
                file_size += fill_num
                for i in range(fill_num):
                    flash_file.write("\0")
            inc_file.write("  {" + file_name[0].upper() + ", " + hex(flash_addr) + ", " + hex(file_size) + "},\n")
            flash_addr += file_size
    inc_file.write("};\n")
    flash_file.close()
    inc_file.write("\n")
    inc_file.write("#endif\n")
    inc_file.close()
    _bin_2_array(PCM_BIN, "pcm_bin", TARGET_PCM_DTAT_FILE)

# create ASR acoustic model array file
def _create_asrfix_array(ctrl):
    _bin_2_array(ASRFIX_DAT, "global_kws_lp_acoutstic_model", TARGET_ASRFIX_DATA_FILE)

# parse thresh data file to get wakeup thresh
def _get_thresh_map(file_name):
    fa_thresh_map = {}
    if (os.path.isfile(file_name)):
        thresh_file = open(file_name, "r")
        lines = thresh_file.readlines()
        for line in lines:
            fa_thresh = line.split(',')
            if (len(fa_thresh) == 2):
                fa_str = fa_thresh[0].strip()
                thresh_str = fa_thresh[1].strip()
                fa_lit = fa_str.split('=')
                thresh_lit = thresh_str.split('=')
                if (len(fa_lit) == 2) and (len(thresh_lit) == 2):
                    if (fa_lit[0].strip() == "fa") and (thresh_lit[0].strip() == "thresh"):
                        fa_thresh_map[fa_lit[1].strip()] = float(thresh_lit[1].strip())
        thresh_file.close()
    return fa_thresh_map

def _get_wakeup_thresh_map():
    return _get_thresh_map(THRESH_DAT_FILE)

def _get_asr_thresh_map():
    return _get_thresh_map(ASR_THRESH_DAT_FILE)

# parse config and modiy lasr timeout
def _modify_config_bin(config_bin):
    ci_cfg_file = open(CUSTOM_CONFIG_FILE, "r")
    config_json = json.load(ci_cfg_file)
    timeout = config_json["local"]["custom_setting"]["timeout_time"]
    ci_cfg_file.close()
    cfg_bin_file = open(config_bin, "r")
    cfg_bin_json = json.load(cfg_bin_file, object_pairs_hook=OrderedDict)
    cfg_bin_file.close()
    cfg_bin_file = open(config_bin, "w")
    if (timeout > 0):
        lasr_asr = cfg_bin_json["lasr"]["lasr_asr"]
        if (lasr_asr and lasr_asr.has_key("timeout")):
            cfg_bin_json["lasr"]["lasr_asr"]["timeout"] = timeout
    json.dump(cfg_bin_json, cfg_bin_file, indent=4, separators=(',', ': '), encoding="utf-8", ensure_ascii=False, sort_keys=False)
    cfg_bin_file.close()

# create app config array file
def _create_config_bin_array(ctrl):
    if (ctrl == "auto"):
        _modify_config_bin(UNI_CONFIG_RELEASE)
        _modify_config_bin(UNI_CONFIG_DEBUG)
        _bin_2_array(UNI_CONFIG_DEBUG, "config_bin", TARGET_UNI_CONFIG_FILE)
    else:
        _bin_2_array(UNI_CONFIG, "config_bin", TARGET_UNI_CONFIG_FILE)

# create ASR grammar model array file
def _create_grammar_array(ctrl):
    _bin_2_array(GRAMMAR_DATA, "global_kws_lp_grammar", TARGET_GRAMMAR_DATA_FILE)

# parse config and modiy aik.json timeout
def _modify_aik_json_file(aik_json):
    ci_cfg_file = open(CUSTOM_CONFIG_FILE, "r")
    config_json = json.load(ci_cfg_file)
    timeout = config_json["local"]["custom_setting"]["timeout_time"]
    wakeup_sensitivity = "middle"
    cmd_sensitivity = "middle"
    try:
        wakeup_sensitivity = config_json["local"]["wakeup"]["sensitivity"]
    except Exception as e:
        print('wakeup sensitivity prase err:' + str(e))
    try:
        cmd_sensitivity = config_json["local"]["cmd"]["sensitivity"]
    except Exception as e:
        print('cmd sensitivity prase err:' + str(e))
    ci_cfg_file.close()
    if "low" == wakeup_sensitivity:
        fa = "0.1"
        fa_snd = "0.3"
        fa_sleep = "0.02"
    elif "high" == wakeup_sensitivity:
        fa = "0.4"
        fa_snd = "0.8"
        fa_sleep = "0.05"
    else:
        fa = "0.2"
        fa_snd = "0.4"
        fa_sleep = "0.04"
    if "low" == cmd_sensitivity:
        fa_cmd = "0.1"
    elif "high" == cmd_sensitivity:
        fa_cmd = "0.8"
    else:
        fa_cmd = "0.4"
    fa_thresh_map = _get_wakeup_thresh_map()
    cfg_bin_file = open(aik_json, "r")
    cfg_bin_json = json.load(cfg_bin_file, object_pairs_hook=OrderedDict)
    cfg_bin_file.close()
    cfg_bin_file = open(aik_json, "w")
    kws_lp = cfg_bin_json["kws_lp"]
    if (timeout > 0):
        if (kws_lp and kws_lp.has_key("timeout")):
            cfg_bin_json["kws_lp"]["timeout"] = timeout
    kws_lp["wakeup"]["fa"] = fa
    kws_lp["wakeup"]["fa_second"] = fa_snd
    kws_lp["wakeup"]["fa_sleep"] = fa_sleep
    kws_lp["command"]["fa"] = fa_cmd
    try:
        wake_thres = cfg_bin_json["kws_lp"]["wakeup"]["wuws"][0]["fa_table"]
        if (kws_lp and wake_thres):
            if (len(wake_thres) == 10):
                if (fa_thresh_map.has_key("0.01")):
                    wake_thres[0] = fa_thresh_map["0.01"]
                if (fa_thresh_map.has_key("0.02")):
                    wake_thres[1] = fa_thresh_map["0.02"]
                if (fa_thresh_map.has_key("0.04")):
                    wake_thres[2] = fa_thresh_map["0.04"]
                if (fa_thresh_map.has_key("0.05")):
                    wake_thres[3] = fa_thresh_map["0.05"]
                if (fa_thresh_map.has_key("0.1")):
                    wake_thres[4] = fa_thresh_map["0.1"]
                if (fa_thresh_map.has_key("0.2")):
                    wake_thres[5] = fa_thresh_map["0.2"]
                if (fa_thresh_map.has_key("0.3")):
                    wake_thres[6] = fa_thresh_map["0.3"]
                if (fa_thresh_map.has_key("0.4")):
                    wake_thres[7] = fa_thresh_map["0.4"]
                if (fa_thresh_map.has_key("0.5")):
                    wake_thres[8] = fa_thresh_map["0.5"]
                if (fa_thresh_map.has_key("0.8")):
                    wake_thres[9] = fa_thresh_map["0.8"]
    except Exception as e:
        print(AIK_CONFIG + ' prase err:' + str(e))

    try:
        asr_fa_map = _get_asr_thresh_map()
        asr_thres = cfg_bin_json["kws_lp"]["command"]["fa_table"]
        if (kws_lp and asr_thres):
            if (len(asr_thres) == 10):
                if (asr_fa_map.has_key("0.01")):
                    asr_thres[0] = asr_fa_map["0.01"]
                if (asr_fa_map.has_key("0.02")):
                    asr_thres[1] = asr_fa_map["0.02"]
                if (asr_fa_map.has_key("0.04")):
                    asr_thres[2] = asr_fa_map["0.04"]
                if (asr_fa_map.has_key("0.05")):
                    asr_thres[3] = asr_fa_map["0.05"]
                if (asr_fa_map.has_key("0.1")):
                    asr_thres[4] = asr_fa_map["0.1"]
                if (asr_fa_map.has_key("0.2")):
                    asr_thres[5] = asr_fa_map["0.2"]
                if (asr_fa_map.has_key("0.3")):
                    asr_thres[6] = asr_fa_map["0.3"]
                if (asr_fa_map.has_key("0.4")):
                    asr_thres[7] = asr_fa_map["0.4"]
                if (asr_fa_map.has_key("0.5")):
                    asr_thres[8] = asr_fa_map["0.5"]
                if (asr_fa_map.has_key("0.8")):
                    asr_thres[9] = asr_fa_map["0.8"]
    except Exception as e:
        print(AIK_CONFIG + ' prase err:' + str(e))
    json.dump(cfg_bin_json, cfg_bin_file, indent=2, separators=(',', ': '), encoding="utf-8", ensure_ascii=False, sort_keys=False)
    cfg_bin_file.close()

# create app config array file
def _create_aik_json_array(ctrl):
    if (ctrl == "auto"):
        _modify_aik_json_file(AIK_CONFIG_RELEASE)
        _modify_aik_json_file(AIK_CONFIG_DEBUG)
        _bin_2_array(AIK_CONFIG_DEBUG, "aik_config", TARGET_AIK_CONFIG_FILE)
    else:
        _bin_2_array(AIK_CONFIG, "aik_config", TARGET_AIK_CONFIG_FILE)

# joint pcm list to string
def _joint_pcm_list_str(pcm_list):
    str = "["
    if (len(pcm_list) < 1):
        str += "]"
        return str
    for pcm in pcm_list:
        name = pcm.split('.')[0]
        str += (name + ", ")
    str = str[0:-2] + "]"
    return str

# create uni_cust_config.h
def _create_cust_config_file(ctrl):
    if (ctrl == "manual"):
        return None
    fo = open(TARGET_CUST_CONFIG_H_FILE, 'w')
    fo.write("#ifndef __UNI_CUST_CONFIG_H__\n")
    fo.write("#define __UNI_CUST_CONFIG_H__\n")
    fo.write("\n")
    ci_cfg_file = open(CUSTOM_CONFIG_FILE, "r")
    config_json = json.load(ci_cfg_file)
    ci_cfg_file.close()
    # UNI_ASR_TIMEOUT
    timeout = config_json["local"]["custom_setting"]["timeout_time"]
    fo.write("#define UNI_ASR_TIMEOUT           " + str(timeout) + "    //ASR timeout Xs\n")
    fo.write("\n")
    # /*---MIC config---*/
    fo.write("#define UNI_MIC_ARRY_TYPE_LINER   0\n")
    fo.write("#define UNI_MIC_ARRY_TYPE_CIRCLE  1\n")
    fo.write("#define ACOUSTIC_MODEL_FIELD_NEAR 0\n")
    fo.write("#define ACOUSTIC_MODEL_FIELD_FAR  1\n")
    fo.write("\n")
    fo.write("/*---MIC config---*/\n")
    micarray = config_json["local"]["micarray"]
   # type of acoustic model
    field = "far"
    if micarray.has_key("field"):
        field = micarray["field"]
    if "near" == field:
        fo.write("#define AACOUSTIC_MODEL_FIELD  ACOUSTIC_MODEL_FIELD_NEAR\n")
    else:
        fo.write("#define AACOUSTIC_MODEL_FIELD  ACOUSTIC_MODEL_FIELD_FAR\n")
    mic_num = micarray["number"]
    fo.write("#define UNI_MIC_ARRY_NUM          " + str(mic_num) + "\n")
    if micarray.has_key("topo"):
        mic_type = micarray["topo"]
        if  ("circle" == mic_type):
            fo.write('#define UNI_MIC_ARRY_TYPE         UNI_MIC_ARRY_TYPE_CIRCLE\n')
        else:
            fo.write('#define UNI_MIC_ARRY_TYPE         UNI_MIC_ARRY_TYPE_LINER\n')
    if micarray.has_key("distance"):
        mic_distance = micarray["distance"]
        fo.write('#define UNI_MIC_ARRY_DISTANCE     ' + str(mic_distance) + "\n")
    fo.write("\n")
    # DEFAULT_PCM_WELCOME and DEFAULT_PCM_ASR_TIMEOUT
    cmd_replay_file = open(CMD_CONTENT_FILE, "r")
    cmd_replay_json = json.load(cmd_replay_file)
    cmd_replay_file.close()
    for item in cmd_replay_json:
        if (item["cmd"] == "asr_timeout_uni"):
            value = _joint_pcm_list_str(item["pcm"])
            fo.write("#define DEFAULT_PCM_ASR_TIMEOUT   \"" + value + "\"\n")
        elif (item["cmd"] == "bringup_uni"):
            value = _joint_pcm_list_str(item["pcm"])
            fo.write("#define DEFAULT_PCM_WELCOME       \"" + value + "\"\n")
        elif (item["cmd"] == "wakeup_uni"):
            value = _joint_pcm_list_str(item["pcm"])
            fo.write("#define DEFAULT_PCM_WAKEUP        \"" + value + "\"\n")
        elif (item["cmd"] == "exitUni"):
            value = _joint_pcm_list_str(item["pcm"])
            fo.write("#define DEFAULT_PCM_SLEEP         \"" + value + "\"\n")
        elif (item["cmd"] == "volumeMinUni"):
            value = _joint_pcm_list_str(item["pcm"])
            fo.write("#define DEFAULT_PCM_VOL_MIN       \"" + value + "\"\n")
        elif (item["cmd"] == "volumeMaxUni"):
            value = _joint_pcm_list_str(item["pcm"])
            fo.write("#define DEFAULT_PCM_VOL_MAX       \"" + value + "\"\n")
        elif (item["cmd"] == "volumeMidUni"):
            value = _joint_pcm_list_str(item["pcm"])
            fo.write("#define DEFAULT_PCM_VOL_MID       \"" + value + "\"\n")
        elif (item["cmd"] == "volumeUpUni"):
            value = _joint_pcm_list_str(item["pcm"])
            fo.write("#define DEFAULT_PCM_VOL_UP        \"" + value + "\"\n")
        elif (item["cmd"] == "volumeDownUni"):
            value = _joint_pcm_list_str(item["pcm"])
            fo.write("#define DEFAULT_PCM_VOL_DOWN      \"" + value + "\"\n")
        else:
            cmd = item["cmd"]
            if ( -1 != cmd.find("UNI_LOCAL_TONE_")):
                pos = cmd.find("LOCAL_TONE")
                cmd = cmd[pos:]
                value = _joint_pcm_list_str(item["pcm"])
                fo.write("#define " + cmd + "      \"" + value + "\"\n")
    fo.write("\n")
    fo.write("#endif /*__UNI_CUST_CONFIG_H__*/\n")
    fo.close()

# create NLU contents
def _create_nlu_contents_file(ctrl):
    if (ctrl == "manual"):
        return None
    inc_file = open(TARGET_NUL_CONTENT_H_FILE, "w")
    inc_file.write("#ifndef INC_UNI_NLU_CONTENT_H_\n")
    inc_file.write("#define INC_UNI_NLU_CONTENT_H_\n")
    inc_file.write("\n")
    inc_file.write("typedef struct {\n")
    inc_file.write("  uni_u32 key_word_hash_code; /* 存放识别词汇对应的hashcode */\n")
    inc_file.write("  uni_u8  nlu_content_str_index; /* 存放nlu映射表中的索引，实现多个识别词汇可对应同一个nlu，暂支持256条，如果不够换u16 */\n")
    inc_file.write("  char    *hash_collision_orginal_str /* 类似Java String equal，当hash发生碰撞时，赋值为识别词汇，否则设置为NULL */;\n")
    inc_file.write("} uni_nlu_content_mapping_t;\n")
    inc_file.write("\n")
    type_str = "enum {\n"
    map_str = "/*TODO perf sort by hashcode O(logN), now version O(N)*/\n"
    map_str += "const uni_nlu_content_mapping_t g_nlu_content_mapping[] = {\n"
    content_str = "const char* const g_nlu_content_str[] = {\n"
    cmd_replay_file = open(CMD_CONTENT_FILE, "r")
    cmd_replay_json = json.load(cmd_replay_file)
    cmd_replay_file.close()
    check_hash_list = []
    for item in cmd_replay_json:
        cmd = item["cmd"]
        if (item["cmd"] == "asr_timeout_uni"):
            continue
        elif (item["cmd"] == "bringup_uni"):
            continue
        elif ( -1 != cmd.find("UNI_LOCAL_TONE_")):
            continue
        else :
            cmd_list = item["cmd_word"]
            if ("NULL" == cmd_list[0]):
                continue
            # type enum
            type = "eCMD_" + item["cmd"]
            type_str += "  " + type + ",\n"
            # g_nlu_content_mapping
            hash_list = item["hash"]
            i = 0
            for hash in hash_list:
                if (check_hash_list.count(hash) > 0):
                    map_str += "  {" + str(hash)+ "U/*" + cmd_list[i] + "*/, " + type + ", \"" + cmd_list[i] + "\"},\n"
                else:
                    map_str += "  {" + str(hash)+ "U/*" + cmd_list[i] + "*/, " + type + ", NULL},\n"
                check_hash_list.append(hash)
                i += 1
            # g_nlu_content_str[]
            content_str += "[" + type + "] = \"{\\\"asr\\\":\\\"" + cmd_list[0] + "\\\",\\\"cmd\\\":\\\"" + item["cmd"] + "\\\",\\\"pcm\\\":\\\"" + _joint_pcm_list_str(item["pcm"]) + "\\\"}\",\n"
    type_str += "};\n"
    map_str += "};\n"
    content_str += "};\n"
    inc_file.write(type_str)
    inc_file.write("\n")
    inc_file.write(content_str)
    inc_file.write("\n")
    inc_file.write(map_str)
    inc_file.write("\n")
    inc_file.write("#endif\n")
    inc_file.close()

# get chacksum
def _get_check_sum(cmd_code):
    c = 0x5A
    c = (c + cmd_code) & 0xFF
    return c

# create cmd code array file
def _create_cmd_code_array(ctrl):
    if (ctrl == "manual"):
        return None
    inc_file = open(TARGET_CMD_CODE_H_FILE, "w")
    map_file = open(TARGET_SUCP_CONT_FILE, "w")
    inc_file.write("#ifndef INC_UNI_CMD_CODE_H_\n")
    inc_file.write("#define INC_UNI_CMD_CODE_H_\n")
    inc_file.write("\n")
    inc_file.write("typedef struct {\n")
    inc_file.write("  uni_u8      cmd_code; /* cmd code fro send base on SUCP */\n")
    inc_file.write("  const char  *cmd_str; /* action string on UDP */;\n")
    inc_file.write("} cmd_code_map_t;\n")
    inc_file.write("\n")
    inc_file.write("const cmd_code_map_t g_cmd_code_arry[] = {\n")
    cmd_replay_file = open(CMD_CONTENT_FILE, "r")
    cmd_replay_json = json.load(cmd_replay_file)
    cmd_replay_file.close()
    cmd_code = CMD_CODE_START_NUM;
    for item in cmd_replay_json:
        cmd = item["cmd"]
        if (item["cmd"] == "asr_timeout_uni"):
            continue
        elif (item["cmd"] == "bringup_uni"):
            continue
        elif ( -1 != cmd.find("UNI_LOCAL_TONE_")):
            continue
        else :
            cmd_list = item["cmd_word"]
            if ("NULL" == cmd_list[0]):
                continue
            inc_file.write("  {" + hex(cmd_code) + ", \"" + item["cmd"] + "\"},\n")
            map_file.write(item["cmd"] + "=" + '|'.join(item["cmd_word"]) + "@" + '|'.join(item["reply"]) + "\n")
            map_file.write("    0x5a " + hex(cmd_code) + " 0x0 0x0 " + hex(_get_check_sum(cmd_code)) + "\n")
        cmd_code += 1;
    inc_file.write("};\n")
    inc_file.write("\n")
    inc_file.write("#endif\n")
    inc_file.close();
    map_file.close();

if __name__ == '__main__':
    ctrl = ""
    if (len(sys.argv) > 1):
        if (sys.argv[1] == "auto"):
            ctrl = "auto"
        elif (sys.argv[1] == "manual"):
            ctrl = "manual"
        else:
            print("Use: python res_build_tool.py auto|manual(default auto if none)")
            sys.exit()
    else:
        ctrl = "auto"
    _create_pcm_bin_array(ctrl)
    _create_asrfix_array(ctrl)
    _create_config_bin_array(ctrl)
    _create_grammar_array(ctrl)
    _create_aik_json_array(ctrl)
    _create_cust_config_file(ctrl)
    _create_nlu_contents_file(ctrl)
    _create_cmd_code_array(ctrl)
    auto_code._create_gpio_config_code(ctrl)
