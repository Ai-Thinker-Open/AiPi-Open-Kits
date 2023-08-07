cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/components/**/*.c")

file(GLOB_RECURSE UI "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/config/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/custom/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/generated/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/generated/guider_fonts/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/generated/images/*.c"
)

# User
# sdk_add_compile_definitions(-DCONFIG_CLI_CMD_ENABLE)
# sdk_add_compile_definitions(-DCONFIG_EASYFLASH4)

# sdk_add_compile_definitions(-DBL616_DHCP_DEBUG)
sdk_add_include_directories(main config components/wifi components/UI components/config components/mqttClient components/voice components/cJson components/esay_flash components/http)

sdk_add_include_directories(components/UI/config components/UI/custom components/UI/generated components/UI/generated/guider_customer_fonts components/UI/generated/guider_fonts)

target_sources(app PRIVATE ${sources} ${UI})

sdk_set_main_file(main/main.c)

project(AiPi-SCP_SmartCtrl)
