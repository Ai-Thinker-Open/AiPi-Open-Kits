# SCP_4.3
车机GUI+按键，按键切换显示界面

## sdk
https://e.coding.net/axk/kaiyuanyingjian/bouffalo_sdk.git
拉取sdk后，将patch文件放到sdk根目录，执行命令git apply [.patch]

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