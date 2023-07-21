# lvgl
驱屏，联网校时，音频录播，按键控灯

## sdk
https://e.coding.net/axk/kaiyuanyingjian/bouffalo_sdk.git
拉取sdk后，将patch文件放到sdk根目录，执行命令git apply [.patch]

## Support CHIP

|      CHIP        | Remark |
|:----------------:|:------:|
|BL602/BL604       |        |
|BL702/BL704/BL706 |        |
|BL616/BL618       |        |
|BL808             |        |

## Compile

- BL602/BL604

```
make CHIP=bl602 BOARD=bl602dk
```

- BL702/BL704/BL706

```
make CHIP=bl702 BOARD=bl702dk
```

- BL616/BL618

```
make CHIP=bl616 BOARD=bl616dk
```

- BL808

```
make CHIP=bl808 BOARD=bl808dk CPU_ID=m0
make CHIP=bl808 BOARD=bl808dk CPU_ID=d0
```

## Flash

```
make flash CHIP=chip_name COMX=xxx # xxx is your com name
```