cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

# file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/speed_ui/generated/guider_fonts/*.c")
# file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/speed_ui/generated/images/*.c")
# file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/speed_ui/generated/*.c")
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/ui/*.c")
file(GLOB_RECURSE app "${CMAKE_CURRENT_SOURCE_DIR}/app/*.c")

# User
# sdk_add_compile_definitions(-DCONFIG_CLI_CMD_ENABLE)

# sdk_add_compile_definitions(-DBL616_DHCP_DEBUG)
target_sources(app PRIVATE

    # speed_ui/custom/custom.c
)

sdk_add_include_directories(.)

# sdk_add_include_directories(speed_ui/generated)
# sdk_add_include_directories(speed_ui/generated/guider_customer_fonts)
# sdk_add_include_directories(speed_ui/generated/guider_fonts)
# sdk_add_include_directories(speed_ui/custom)
sdk_add_include_directories(ui)
sdk_add_include_directories(app)
target_sources(app PRIVATE ${sources} ${app})
sdk_set_main_file(main.c)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})
