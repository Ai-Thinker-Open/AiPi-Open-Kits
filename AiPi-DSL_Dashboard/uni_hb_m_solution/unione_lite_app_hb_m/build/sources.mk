################################################################################
# Automatically-generated file. Do not edit!
################################################################################

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
SAG_SRCS += \

OBJCOPY_OUTPUTS := 
OBJS := 
C_DEPS := 
GCOV_OUT := 
SYMBOL_OUTPUTS := 
READELF_OUTPUTS := 
GPROF_OUT := 
SIZE_OUTPUTS := 
EXECUTABLES := 
S_UPPER_DEPS := 

CCFLAGS        := -O1 -g3 -mcmodel=medium -Wall -mcpu=d1088-spu -c -fmessage-length=0 \
                  -ldsp -mext-dsp -fsingle-precision-constant -ffunction-sections \
                  -fdata-sections -mext-dsp -mext-zol -MMD -MP

CCFLAGS        += -DFUNC_OS_EN=1 -DFLASH_BOOT_EN=$(UART_UPDATE) \
                  -DBUILD_DATE=\"2023072418\" -DBUILD_ID=\"113642\"

# The git version of mcu app
APP_VERSION    := "v4.0.8"
CCFLAGS        += -DAPP_VERSION=\"$(APP_VERSION)\"

# The build time of mcu app
BUILD_TIME      = $(shell date +"%Y-%m-%d\ %H:%M:%S")
CCFLAGS        += -DBUILD_TIME=\"$(BUILD_TIME)\"

INCLUDE_DIR    +=..
INCLUDE_DIR    +=../include
INCLUDE_DIR    +=../include/include
INCLUDE_DIR    +=../include/include/osal
INCLUDE_DIR    +=../middleware/rtos/freertos/inc
INCLUDE_DIR    +=../middleware/rtos/rtos_api
INCLUDE_DIR    +=../src
INCLUDE_DIR    +=../src/app/inc
INCLUDE_DIR    +=../src/app/inc/sessions
INCLUDE_DIR    +=../src/hal/inc
INCLUDE_DIR    +=../src/sdk/audio/audio_common/inc
INCLUDE_DIR    +=../src/sdk/audio/audio_player/inc
INCLUDE_DIR    +=../src/sdk/idle_detect/inc
INCLUDE_DIR    +=../src/sdk/player/inc
INCLUDE_DIR    +=../src/sdk/player/src/pcm/inc
INCLUDE_DIR    +=../src/sdk/uart/inc
INCLUDE_DIR    +=../src/sdk/vui/inc
INCLUDE_DIR    +=../src/utils/arpt/inc
INCLUDE_DIR    +=../src/utils/auto_string/inc
INCLUDE_DIR    +=../src/utils/bitmap/inc
INCLUDE_DIR    +=../src/utils/black_board/inc
INCLUDE_DIR    +=../src/utils/cJSON/inc
INCLUDE_DIR    +=../src/utils/config/inc
INCLUDE_DIR    +=../src/utils/crc16/inc
INCLUDE_DIR    +=../src/utils/data_buf/inc
INCLUDE_DIR    +=../src/utils/event/inc
INCLUDE_DIR    +=../src/utils/event_list/inc
INCLUDE_DIR    +=../src/utils/event_route/inc
INCLUDE_DIR    +=../src/utils/float2string/inc
INCLUDE_DIR    +=../src/utils/fsm/inc
INCLUDE_DIR    +=../src/utils/hash/inc
INCLUDE_DIR    +=../src/utils/interruptable_sleep/inc
INCLUDE_DIR    +=../src/utils/list/inc
INCLUDE_DIR    +=../src/utils/log/inc
INCLUDE_DIR    +=../src/utils/string/inc
INCLUDE_DIR    +=../src/utils/timer/inc
INCLUDE_DIR    +=../src/utils/uart/inc
INCLUDE_DIR    +=../user/inc
INCLUDE_DIR    +=../user/src/examples
INCLUDE_DIR    +=../user/src/examples/drivers
INCLUDE_DIR    +=../user/src/examples/protocol

INCLUDE_FLAGS   = $(addprefix -I , $(INCLUDE_DIR))
CCFLAGS        += $(INCLUDE_FLAGS)

# Every subdirectory with source files must be described here
SUBDIRS := \
middleware/rtos/freertos/src \
middleware/rtos/rtos_api \
src/app/src \
src/app/src/sessions \
src/hal/src \
src/sdk/audio/audio_player/src \
src/sdk/idle_detect/src \
src/sdk/player/src/pcm/src \
src/sdk/player/src \
src/sdk/uart/src \
src/sdk/vui/src \
src/utils/arpt/src \
src/utils/auto_string/src \
src/utils/bitmap/src \
src/utils/black_board/src \
src/utils/cJSON/src \
src/utils/config/src \
src/utils/crc16/src \
src/utils/data_buf/src \
src/utils/event/src \
src/utils/event_list/src \
src/utils/event_route/src \
src/utils/float2string/src \
src/utils/fsm/src \
src/utils/hash/src \
src/utils/interruptable_sleep/src \
src/utils/list/src \
src/utils/log/src \
src/utils/string/src \
src/utils/timer/src \
src/utils/uart/src \
startup \
user/src \
user/src/examples
