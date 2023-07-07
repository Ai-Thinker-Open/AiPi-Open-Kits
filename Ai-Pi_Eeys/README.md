# AiPi-Eyes 天气站

## 一、使用指南

## 1.功能介绍 
- **自动连接：**  AiPi-Eyes 天气站会自动连接历史连接过的WiFi
- **自动更新时间：** 根据SNTP 服务器更新时间
- **自动更新天气情况：** 通过HTTPS 获取天气情况
- **支持触摸配网：** 可以利用键盘输出相应的SSID及PASSWORD 配置网络
- **支持未来七天的天气预报，默认显示三天**
- **代码开源**
## 2.配网操作

`点击` 右上角的"`WiFi`"网络图标后，会切换到配网界面，在`SSID` 输入框中输入WiFi 名称，在`PASS`输入框中输入WiFi密码后，`点击` "`确定按钮`"后可触发配网，配网成功后会自动切换到主页面。

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
```