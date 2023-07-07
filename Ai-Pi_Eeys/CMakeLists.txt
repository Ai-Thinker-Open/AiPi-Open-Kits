cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/demos/*.c")

# User
# sdk_add_compile_definitions(-DCONFIG_CLI_CMD_ENABLE)
# sdk_add_compile_definitions(-DCONFIG_EASYFLASH4)

# sdk_add_compile_definitions(-DBL616_DHCP_DEBUG)
target_sources(app PRIVATE

   # audio.c
   audio_pcm.c

   # 8388_pcm.c
   bsp_es8388.c
)

sdk_add_include_directories(.)
sdk_add_include_directories(demos/240X320_ui/custom)
sdk_add_include_directories(demos/240X320_ui/generated)
sdk_add_include_directories(demos/240X320_ui/generated/guider_customer_fonts)
sdk_add_include_directories(demos/240X320_ui/generated/guider_fonts)

target_sources(app PRIVATE ${sources})
sdk_set_main_file(main.c)

project(Eyes)
