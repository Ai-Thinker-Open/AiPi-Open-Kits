#
#compiler flag config domain
#
#CONFIG_TOOLPREFIX :=
#CONFIG_OPTIMIZATION_LEVEL_RELEASE := 1
#CONFIG_M4_SOFTFP := 1

#
#board config domain
#
CONFIG_BOARD_FLASH_SIZE := 2

#firmware config domain
#

#set CONFIG_ENABLE_ACP to 1 to enable ACP, set to 0 or comment this line to disable
#CONFIG_ENABLE_ACP:=1
CONFIG_BL_IOT_FW_AP:=1
CONFIG_BL_IOT_FW_AMPDU:=0
CONFIG_BL_IOT_FW_AMSDU:=0
CONFIG_BL_IOT_FW_P2P:=0
CONFIG_ENABLE_PSM_RAM:=1
#CONFIG_ENABLE_CAMERA:=1
#CONFIG_ENABLE_BLSYNC:=1
#CONFIG_ENABLE_VFS_SPI:=1
CONFIG_ENABLE_VFS_ROMFS:=1
CONFIG_ADC_ENABLE_TSEN:=1
CONFIG_EASYFLASH_ENABLE:=1
CONFIG_BL602_USE_ROM_DRIVER:=1
CONFIG_FREERTOS_TICKLESS_MODE:=0

CONFIG_SYS_APP_TASK_STACK_SIZE:=4096
CONFIG_SYS_APP_TASK_PRIORITY:=15
CONFIG_SYS_VFS_ENABLE:=1
CONFIG_SYS_VFS_UART_ENABLE:=1
CONFIG_SYS_AOS_CLI_ENABLE:=1
CONFIG_SYS_AOS_LOOP_ENABLE:=1
CONFIG_SYS_BLOG_ENABLE:=1
CONFIG_SYS_DMA_ENABLE:=1
CONFIG_SYS_USER_VFS_ROMFS_ENABLE:=1

CONFIG_BT:=1
CONFIG_BT_CENTRAL:=1
CONFIG_BT_OBSERVER:=1
CONFIG_BT_PERIPHERAL:=1
CONFIG_BT_STACK_CLI:=1
#CONFIG_BT_MESH := 1
CONFIG_BLE_STACK_DBG_PRINT := 1
CONFIG_BT_STACK_PTS := 0
CONF_ENABLE_COREDUMP:=1
ifeq ($(CONFIG_BT_MESH),1)
CONFIG_BT_MESH_PB_ADV := 1
CONFIG_BT_MESH_PB_GATT := 1
CONFIG_BT_MESH_FRIEND := 1
CONFIG_BT_MESH_LOW_POWER := 1
CONFIG_BT_MESH_PROXY := 1
CONFIG_BT_MESH_GATT_PROXY := 1
endif

#mbedtls
CONFIG_MBEDTLS_AES_USE_HW:=1
CONFIG_MBEDTLS_BIGNUM_USE_HW:=1
CONFIG_MBEDTLS_ECC_USE_HW:=1
CONFIG_MBEDTLS_SHA1_USE_HW:=1
CONFIG_MBEDTLS_SHA256_USE_HW:=1

#blog enable components format :=blog_testc cli vfs helper
LOG_ENABLED_COMPONENTS:=blog_testc hosal loopset looprt bloop main
