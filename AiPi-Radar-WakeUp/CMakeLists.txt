# *************************************************************#
# 工程源文件配置
# Engineering Source File Configuration
# *************************************************************#

# ############################ 保持默认 #######################
# #########################  Keep default ####################
cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

# #############################################################

# 搜集所有的C文件( Collect source files)
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/components/*.c")
file(GLOB_RECURSE UI "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/*.c")

# 添加头文件的引用路径（Add .h include directories）
sdk_add_include_directories(main config components/wifi components/USB_HID components/Radar_recv components/CJSON components/http_client)

sdk_add_include_directories(components/UI components/UI/custom components/UI/generated components/UI/generated/guider_fonts)

# 把C文件添加到工程里（Add .c file to the project）
target_sources(app PRIVATE ${sources} ${UI})

# 设置main.c 的文件（Set the document source for main. c）
# 注意：不能和file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/components/*.c") 冲突（Note: Cannot conflict with “file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/components/*.c")”）
sdk_set_main_file(main/main.c)

# 设置工程名称，这个配置决定了编译出来的文件名。(Set the project name, which determines the compiled file name)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})
