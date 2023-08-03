# AiPi-Eyes_DU PWM呼吸灯

## 一、使用指南

**实现RGB三色灯循环呼吸亮灭**

## 二、开发指南

### 1.克隆

```
git clone -b release/v1.0.0 https://github.com/Ai-Thinker-Open/Ai-Pi_Eeys.git

cd Ai-Pi_Eeys
git submodule init
git submodule update

cd aithinker_Ai-M6x_SDK
. install.sh
. export.sh
cd ..
```

### 2.编译
```
make 
```
### 3.下载
```
make flash COMX=/dev/ttyUSB0

//使用Type-C线连接TTL-USB进入烧录模式方法如下：
//命令下发后，长按Burn键（S2），烧录开始后即可松开Burn键（S2）
```