cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/components/**/*.c")

# User
# sdk_add_compile_definitions(-DCONFIG_CLI_CMD_ENABLE)
# sdk_add_compile_definitions(-DCONFIG_EASYFLASH4)

# sdk_add_compile_definitions(-DBL616_DHCP_DEBUG)
sdk_add_include_directories(main config components/wifi components/mqttClient components/cJson components/RGB components/system_status)

target_sources(app PRIVATE ${sources})

sdk_set_main_file(main/main.c)

project(AiPi-M62-RGB)
