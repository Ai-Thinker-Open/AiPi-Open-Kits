# AiPi-knob
## 小安派-knob介绍：
硬件：1，圆屏（240*240）+旋钮编码器交互方式；2，红外发射与接收；3, RGB灯；4，USB接口；5，内置音频输入与输出。6，M61高性能模组；
软件：1，中控GUI+旋钮编码器交互方式。 2，pwm调RGB色光。 3，红外发射与接收功能。 4，语音播报。

## sdk
拉取sdk后，修改Makefile文件里的BL_SDK_BASE ?= <sdk路径>

## patch
拉取sdk后，打入patch文件0001-encoder_ir-1-lvgl.patch


## Support CHIP

|      CHIP        | Remark |
|:----------------:|:------:|
|BL616/BL618       |        |

## Compile

- BL616/BL618

```
make CHIP=bl616 BOARD=bl616dk
```

## Flash

```
make flash COMX=xxx ## xxx is your com name
```