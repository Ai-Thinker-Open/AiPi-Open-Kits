SDK_DEMO_PATH ?= .
BL_SDK_BASE ?= $(SDK_DEMO_PATH)/../aithinker_Ai-M6X_SDK

NOW_REAL_PATH:= $(realpath ./)

export BL_SDK_BASE

CHIP ?= bl616
BOARD ?= board
BOARD_DIR ?= $(NOW_REAL_PATH)/../AiPi-Eyes-Rx

CROSS_COMPILE ?= riscv64-unknown-elf-

# add custom cmake definition
#cmake_definition+=-Dxxx=sss

include $(BL_SDK_BASE)/project.build