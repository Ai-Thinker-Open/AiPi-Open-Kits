cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

# file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/demos/*.c")
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/demos/remote_240x320/*.c")
file(GLOB_RECURSE app "${CMAKE_CURRENT_SOURCE_DIR}/app/*.c")

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
sdk_add_include_directories(demos/remote_240x320/custom)
sdk_add_include_directories(demos/remote_240x320/generated)
sdk_add_include_directories(demos/remote_240x320/generated/guider_customer_fonts)
sdk_add_include_directories(demos/remote_240x320/generated/guider_fonts)

# sdk_add_include_directories(demos/ac_240_320)
# sdk_add_include_directories(demos/images)
# sdk_add_include_directories(demos/screens)
sdk_add_include_directories(app)
target_sources(app PRIVATE ${sources})
target_sources(app PRIVATE ${app})
sdk_set_main_file(main.c)

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
get_filename_component(PROJECT_NAME ${PARENT_DIR} NAME)
project(${PROJECT_NAME})
