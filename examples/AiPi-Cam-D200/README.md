# AiPi-CAM-D200

## 注意
cam-D 需将proj.conf文件的CONFIG_CHERRYUSB置0, cam-U则置1

## sdk
https://e.coding.net/axk/kaiyuanyingjian/bouffalo_sdk.git

## update
1，新增支持200万像素CG2145;
2，cam模式修改为mjpeg联动模式;

## 一、使用指南

## 1.功能介绍 
- **自动开热点：**  使用手机或电脑连接热点:AiPi-Cam，密码：12345678
- **web服务器：** 打开网页输入网站192.168.169.1
- **视频流：** 点击Start Stream，观看摄像头影像
- **拍照：** 按键进行拍照存储SD卡
- **闪光灯：** 长按`Burn`按钮打开闪光灯
- **代码开源**

## 二、开发指南

### 1.克隆


### 2.编译
```
make 
```
### 3.下载
```
make flash COMX=/dev/ttyUSB0
```
## 三、切换到100W 像素或200W像素的办法

进入*bl61x_SDK/os/bsp/common/image_sensor* 路径，打开*gc2145.h* 头文件，把宏定义GC2145_SIZE_USE 进行修改：
```
#define GC2145_SIZE_USE       GC2145_SIZE_1600X1200
```
> GC2145_SIZE_1280X720 为100W像素
>
>GC2145_SIZE_1600X1200 为200W像素

