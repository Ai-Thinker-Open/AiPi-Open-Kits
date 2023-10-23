# ********************************#
# 编译配置
# Compile Configuration
# ********************************#

# 当前工程文件路径(Current project file path)
SDK_DEMO_PATH ?= .
# 配置SDK路径(Configure SDK path)
BL_SDK_BASE ?= $(SDK_DEMO_PATH)/../aithinker_Ai-M6X_SDK
# 设置SDK路径(Set SDK path)

NOW_REAL_PATH:= $(realpath ./)

export BL_SDK_BASE

#配置芯片型号，M61/M62都配置成bl616(Configure chip models, with both M61 and M62 configured as bl616)
CHIP ?= bl616
#配置板子类型，M61/M62 保持 “bl616dk”(Configuration board subtype, M61/M62 maintains' bl616dk ')
BOARD ?= board
BOARD_DIR ?= $(NOW_REAL_PATH)
#配置编译工具链(Configure Compilation Toolchain)
CROSS_COMPILE ?= riscv64-unknown-elf-

# add custom cmake definition
#cmake_definition+=-Dxxx=sss

#引用实际的编译配置(Reference the actual compilation configuration)
include $(BL_SDK_BASE)/project.build