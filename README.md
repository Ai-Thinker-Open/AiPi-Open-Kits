# 小安派爱星云SDK

因为爱星云SDK 是通过原厂SDK适配的，所以特意创建此分支以使用爱星云SDK。同时，AiPi-Cam-D200源码也会放在此分支当中。

| 源码     | 概述 | 详细介绍|
| :----:    |:----: |:----: |
| blank| 爱星云基本连接代码| [点击查看](examples/blank/README.md) |
| AiPi-R286| 小安派-爱星云86盒固件源码| [点击查看](examples/AiPi-R286/README.md) |
| AiPi-Cam-D200| 小安派-Cam-D200 源码| [点击查看](examples/AiPi-Cam-D200/README.md) |

## 一、克隆Ai-Thinker仓库本分支

在linux的终端下输入,
### Github:
```
git clone -b AiPi-aiThinkerCloud https://github.com/Ai-Thinker-Open/AiPi-Open-Kits.git
```
### Gitee:
```
git clone -b AiPi-aiThinkerCloud https://gitee.com/Ai-Thinker-Open/AiPi-Open-Kits.git
```

将整个开源硬件的DEMO仓库克隆下来，正常情况下，需要等待些许时间。

## 二、拉取子模块

首先进入AiPi-Open-Kits目录下:
```
cd AiPi-Open-Kits
```
在该目录下输入以下两条指令初始化及拉取子模块:
```
git submodule init
git submodule update
```
设置路径:
```
. export.sh
```

## 三、编译和烧录

在AiPi-Open-Kits下的*example*目录看见各种小安派的Demo文件夹，使用cd命令进入到需要编译和烧录的Demo下
```
cd 对应Demo目录文件夹
make                          //编译命令
make flash COMX=/dev/ttyUSB0  //烧录命令
```
