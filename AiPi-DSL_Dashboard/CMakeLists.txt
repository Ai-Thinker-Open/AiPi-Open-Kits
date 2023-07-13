cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/lvgl_config/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/wifi/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/UI/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/components/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/BLE_HID/*.c")

file(GLOB_RECURSE UI "${CMAKE_CURRENT_SOURCE_DIR}/UI/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/UI/generated/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/UI/generated/guider_fonts/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/UI/generated/images/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/UI/custom/*.c")

# User
# sdk_add_compile_definitions(-DCONFIG_CLI_CMD_ENABLE)
# sdk_add_compile_definitions(-DCONFIG_EASYFLASH4)

# sdk_add_compile_definitions(-DBL616_DHCP_DEBUG)
sdk_add_include_directories(main wifi BLE_HID lvgl_config UI UI/generated UI/generated/guider_fonts UI/generated/images UI/custom components)

target_sources(app PRIVATE ${sources} ${UI})
sdk_set_main_file(main/main.c)

project(AiPi-DSL_Dashboard)
