# AiPi-UNO-Emac 以太网口通讯源码

## 编译烧录
```
make flash
```
## 正常启动的Log
```
  ____               __  __      _       _       _     
 |  _ \             / _|/ _|    | |     | |     | |    
 | |_) | ___  _   _| |_| |_ __ _| | ___ | | __ _| |__  
 |  _ < / _ \| | | |  _|  _/ _` | |/ _ \| |/ _` | '_ \ 
 | |_) | (_) | |_| | | | || (_| | | (_) | | (_| | |_) |
 |____/ \___/ \__,_|_| |_| \__,_|_|\___/|_|\__,_|_.__/ 

Build:15:22:46,Dec  5 2023
Copyright (c) 2022 Bouffalolab team
=========== flash cfg ==============
jedec id   0xC84017
mid            0xC8
iomode         0x04
clk delay      0x01
clk invert     0x01
read reg cmd0  0x05
read reg cmd1  0x35
write reg cmd0 0x01
write reg cmd1 0x31
qe write len   0x01
cread support  0x01
cread code     0x20
burst wrap cmd 0x77
=====================================
dynamic memory init success, ocram heap size = 253 Kbyte 
sig1:ffffffff
sig2:0000f32f
cgen1:9f7ffffd
lwip statck init
lwip init done
sys_mbox_new done!
sys_mutex_new done!
tcpip thread init done!
netif config
emac init
emac phy addr:0x0000
emac phy reg read 0x60e0
emac phy id 1 =00000007
emac phy id 2 =0000c0f1
ETH PHY init ok!
PHY[7c0f1] @0 ready on 100Mbps, full duplex
EMAC start
[OS] Starting emac rx task...
[OS] bl Netif is up
Static IP address: 192.168.123.100
dns_ipaddr:::8080808 
Active Partition[0] consumed 596 Bytes
======= PtTable_Config @0x62fcdc3c=======
magicCode 0x54504642; version 0x0000; entryCnt 8; age 0; crc32 0xE38928F3
idx  type device active_index     name   Address[0]  Address[1]  Length[0]   Length[1]   age
[00]  16     0         0        Boot2  0x00000000  0x00000000  0x0000e000  0x00000000  0
[01]  00     0         0           FW  0x00010000  0x00210000  0x00200000  0x00168000  0
[02]  10     0         0          mfg  0x00210000  0x00000000  0x00168000  0x00000000  0
[03]  02     0         0        media  0x00378000  0x00000000  0x00071000  0x00000000  0
[04]  03     0         0          PSM  0x003e9000  0x00000000  0x00008000  0x00000000  0
[05]  04     0         0          KEY  0x003f1000  0x00000000  0x00002000  0x00000000  0
[06]  05     0         0         DATA  0x003f3000  0x00000000  0x00005000  0x00000000  0
[07]  06     0         0      factory  0x003f8000  0x00000000  0x00008000  0x00000000  0
[MTD] >>>>>> Hanlde info Dump >>>>>>
      name PSM
      id 0
      offset 0x003e9000(4100096)
      size 0x00008000(32Kbytes)
      xip_addr 0xa03d8000
[MTD] <<<<<< Hanlde info End <<<<<<
ENV AREA SIZE 32768, SECTOR NUM 8
*default_env_size = 0x00000001
bouffalolab />
```
## TCP 客户端测试连接
串口SHELL 输入:
```
wifi_tcp_client <IP addr> <port>
```


