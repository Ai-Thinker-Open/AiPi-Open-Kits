# *************************************************************#
# 工程源文件配置
# Engineering Source File Configuration
# *************************************************************#

# ############################ 保持默认 #######################
# #########################  Keep default ####################
cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

# sdk_set_linker_script(config/bl616_flash.ld)

# #############################################################

# 搜集所有的C文件( Collect source files)
file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/components/cloud/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/device/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/relays/*.c"
)

file(GLOB_RECURSE UI "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/custom/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/generated/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/generated/guider_fonts/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/components/UI/generated/images/*.c"
)
aux_source_directory(./src SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/driver SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/protocol SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/system SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/utils SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/application SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/3rdparty/http-parser/src SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/3rdparty/aiio/tcp_transport SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/3rdparty/aiio/tcp_transport SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/3rdparty/aiio/aiio_tls SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/3rdparty/aiio/aiio_tls/aiio-tls-crypto SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/3rdparty/aiio/mqtt/aiio_mqtt SRC_FILES)
aux_source_directory(./../../aiTinkerCloud_SDK/3rdparty/aiio/mqtt/aiio_mqtt/lib SRC_FILES)

# 添加头文件的引用路径（Add .h include directories）
sdk_add_include_directories(main config components/wifi components/state components/relays components/device components/cloud)

sdk_add_include_directories(components/UI components/UI/custom components/UI/generated components/UI/generated/guider_customer_fonts components/UI/generated/guider_fonts components/UI/generated/images)
sdk_add_include_directories(../../aiTinkerCloud_SDK/driver)
sdk_add_include_directories(../../aiTinkerCloud_SDK/protocol)
sdk_add_include_directories(../../aiTinkerCloud_SDK/system)
sdk_add_include_directories(../../aiTinkerCloud_SDK/utils)
sdk_add_include_directories(../../aiTinkerCloud_SDK/application)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/http-parser/include)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio/tcp_transport/include)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio/tcp_transport/private_include)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio/aiio_tls)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio/aiio_tls/aiio-tls-crypto)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio/aiio_http_client/include)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio/aiio_http_client/lib/include)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio/aiio_http_server/include)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio/aiio_http_server/src/port)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio/mqtt/aiio_mqtt/lib/include)
sdk_add_include_directories(../../aiTinkerCloud_SDK/3rdparty/aiio/mqtt/aiio_mqtt/include)

sdk_add_include_directories(.)
sdk_add_include_directories(main)

# 把C文件添加到工程里（Add .c file to the project）
target_sources(app PRIVATE ${sources} ${UI} ${SRC_FILES})

# 设置main.c 的文件（Set the document source for main. c）
# 注意：不能和file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/components/*.c") 冲突（Note: Cannot conflict with “file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/components/*.c")”）
sdk_set_main_file(main/main.c)

# 设置工程名称，这个配置决定了编译出来的文件名。(Set the project name, which determines the compiled file name)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})
