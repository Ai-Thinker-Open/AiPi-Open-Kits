SDK_DEMO_PATH ?= .
# BL_SDK_BASE ?= /home/kiwi/bl61x_combo/BL61x_ComboAT/bl61x_SDK
BL_SDK_BASE ?= ${PWD}/../../bl61x_SDK/os

export BL_SDK_BASE

NOW_REAL_PATH:= $(realpath ./)

CHIP ?= bl616
BOARD ?= aipi_cam-board
# BOARD ?= bl616dk
CROSS_COMPILE ?= riscv64-unknown-elf-
BOARD_DIR = ${NOW_REAL_PATH}

# add custom cmake definition
#cmake_definition+=-Dxxx=sss

include $(BL_SDK_BASE)/project.build
