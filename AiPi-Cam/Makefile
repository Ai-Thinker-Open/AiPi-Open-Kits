SDK_DEMO_PATH ?= .
BL_SDK_BASE ?= $(SDK_DEMO_PATH)/../aithinker_Ai-M6X_SDK

export BL_SDK_BASE

NOW_REAL_PATH:= $(realpath ./)

CHIP ?= bl616
BOARD ?= bl616vtb
CROSS_COMPILE ?= riscv64-unknown-elf-
BOARD_DIR = ${NOW_REAL_PATH}

# add custom cmake definition
#cmake_definition+=-Dxxx=sss

include $(BL_SDK_BASE)/project.build
