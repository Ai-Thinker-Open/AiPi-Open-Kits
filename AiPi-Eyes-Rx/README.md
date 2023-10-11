# 86panel solution

本方案是基于 BL618 的 86panel solution, 这里简单描述一下本方案的基本情况，以便于客户更好的进行二次开发。

## 基本目录结构

本方案相关的目录结构，和相关的文件资源介绍如下：

```bash
├── board
│   ├── bl616_flash.ld  # 本方案使用的链接脚本
│   ├── board.c # 本方案使用的 board 配置文件
│   ├── board.h
│   ├── fw_header.c # 本方案使用的 fw header 配置
│   ├── fw_header.h
│   ├── CMakeLists.txt
│   └── config
│       ├── bl_factory_params_IoTKitA_auto.dts # 默认使用的 dts 文件
│       ├── whole_img_bootloader.bin # 默认使用的 bootloader 文件，本 bootloader 为此方案专用文件
│       ├── edata.bin # 此方案必须文件
│       └── partition_cfg_edata_4M.toml # 默认 partition 文件
├── docs # 相关设计文档存放目录
├── flash_prog_cfg.ini # 本方案 flash 烧录配置文件
├── lcd_conf_user.h # lcd 屏幕配置文件
├── lv_conf.h # lvgl 图形库配置文件
├── main.c # main
├── Makefile # makefile
├── proj.conf # 此方案相关组件配置
├── README.md # readme
├── touch_conf_user.h # touch 配置文件
├── CMakeLists.txt
└── demos
    ├── benchmark # lvgl benchmark 目录
    └── fan_temp  # 此方案默认 ui 文件
        ├── ui.c
        ├── ui_events.c
        ├── ui_font_Number.c
        ├── ui.h
        ├── ui_helpers.c
        ├── ui_helpers.h
        └── ui_img_2716252_png.c

```

## 使用本方案必要的准备

由于本方案的特殊性，使用了加密的专用 bootloader，因此必须首先使用博流提供的专用烧录板，进行加密 bootloader 文件的密钥烧写，否则此 bootloader 不能正常运行。

### 专用烧写板使用方法

- 烧写板与模组通信引脚定义：GPIO23（烧写板 UART_TXD）、GPIO24（烧写板 UART_RXD）、GPIO25（烧写板控制 Rst_Pin）、GPIO26（烧写板控制 Boot_Pin）、3.3V 和 GND（供电）。

专用烧写板与 BL618 模组连接示意图如下图所示：

```c
/*
  ##############                              +++++++++++++
  #            # VCC      <------->       VCC +           +
  #            # GND      <------->       GND +           +
  #            # IO23(TX) <------->  IO22(RX) +           +
  # 专用烧写板  # IO24(RX) <------->  IO21(TX) +   BL618   +
  #            # IO25     <------->   CHIP_EN +           +
  #            # IO26     <-------> IO2(BOOT) +           +
  #            #                              +           +
  ##############                              +++++++++++++
*/
```

- 首先将烧写板的 `USB2` type-C 连接到 PC，此 USB 接口将给烧录板供电，并枚举出一个串口（为专用烧录板工作 log 口）。烧写过程中必须保证烧写板供电不断。
- 将烧写板与模组之间按上图所示线序连接好。先连接好其他引脚，最后连接 VCC 给芯片供电。
- 当专用烧写板上的`绿灯`常亮时，即表示，针对该芯片的密钥烧写完成（“接线完成”至“烧写完成”的时间不应超过3秒，超过则可认为烧写异常）。

*注意：正常情况下，烧写完成后绿灯会保持常亮，当且仅当更换模组之后，绿灯会先灭，然后执行烧写流程，烧写完成后再次转为常亮。*

**模组烧写完成一次之后，不需要在烧写第二次了，仅烧一次即可。**

### 专用烧写板指示灯功能

- 黄灯闪烁，通信中。
- 黄灯常灭，通信异常。
- 绿灯常灭，当前模组未完成烧写。
- 绿灯常亮，当前模组已完成烧写。

### 异常情况排查

- 黄灯常灭，绿灯常灭。
  - 通信存在异常，本次烧写失败。需要检查烧写板与模组的接线情况，确认如下几项，
    - 模组供电 3.3V 是否正常。
    - 烧写板与模组之间是否共地。
    - 烧写板 GPIO23/24/25/26 是否与模组正常连接。
    - 模组是否有损坏。
- 黄灯闪烁，绿灯常灭。
  - 通信正常，烧写失败。需要确认模组是否已经进行过一次 efuse 烧写。需要将烧写板的串口 log 和其 GPIO23/24 线上通信的数据抓包进行分析。
- 其它。
  - 烧写板异常。重新上电烧写板。若异常依旧，需要将烧写板的串口 log 和其 GPIO23/24 线上通信的数据抓包进行分析。


## Solution固件烧写

- 编译本项目，直接使用 `make` 命令即可完成编译。
- 编译完成后，使用 `make flash COMX=xxx` 即可完成项目下载。也可以使用图形化界面的 flash_cube 工具，进行烧写。按照 `flash_prog_cfg.ini` 文件的配置烧写即可。

需要注意的是，本方案中使用的 bootloader 是专用版本，是加密后的版本，edata 是包含当前已支持的 DPI driver 的固件，后续更新会保证 API 接口兼容。

## 运行现象

- 下载正常后，运行 demo 后，屏幕会正常点亮，显示设计的 UI, 并且 es8388 初始化成功后，会使能录音和播放，录音后的音频会直接播放出来。

## Support CHIP

base BL618

## How Compile this project

如果是编译带8388的开发板（AiPi-Eyes-R1）需要配置如下参数

​		修改 app_main.h #define DEF_USER_ES8388_EN (1)

​		修改 proj.conf

​				set(CONFIG_SOLUTION_FUNC_AUDIO_AUADC    0)
​				set(CONFIG_SOLUTION_FUNC_AUDIO_AUDAC    0)

如果是编译不带8388的开发板（AiPi-Eyes-R2）需要配置如下参数

​		修改 app_main.h #define DEF_USER_ES8388_EN (0)

​		修改 proj.conf

​				set(CONFIG_SOLUTION_FUNC_AUDIO_AUADC    1)
​				set(CONFIG_SOLUTION_FUNC_AUDIO_AUDAC    1)

```bash
make CHIP=bl616 BOARD=board
```



## Flash download

```bash
make flash CHIP=BL616 COMX=xxx # xxx is your COM name
```

## This solution flash full image layout

```
++++++++++++++++++++++++
|      bootloader      |
++++++++++++++++++++++++
|         PT A         |
++++++++++++++++++++++++
|         PT B         |
++++++++++++++++++++++++
|      FW A header     |
++++++++++++++++++++++++
|         FW A         |
++++++++++++++++++++++++
|      FW B header     |
++++++++++++++++++++++++
|      FW B / mfg      |
++++++++++++++++++++++++
|        ......        |
++++++++++++++++++++++++
|        edata         |
++++++++++++++++++++++++

```

##

