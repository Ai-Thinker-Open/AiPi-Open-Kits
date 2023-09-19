cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.c")

# User
sdk_add_compile_definitions(-DCONFIG_CLI_CMD_ENABLE -DLV_LVGL_H_INCLUDE_SIMPLE)

# sdk_add_compile_definitions(-DBL616_DHCP_DEBUG)
target_sources(app PRIVATE
   audio_pcm.c
   ir/module_wireless.c
   ir/ir_send/ir_send.c
)

sdk_add_include_directories(.)
sdk_add_include_directories(ui)
sdk_add_include_directories(ui/layer_manage)
sdk_add_include_directories(ir)
sdk_add_include_directories(ir/ir_send)
target_sources(app PRIVATE ${sources})
sdk_set_main_file(main.c)

project(mqtt)
