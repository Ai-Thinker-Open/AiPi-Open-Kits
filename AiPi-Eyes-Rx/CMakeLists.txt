cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

file(GLOB_RECURSE sources "${CMAKE_CURRENT_SOURCE_DIR}/demos/*.c")

sdk_add_include_directories(.)
target_sources(app PRIVATE ${sources}
    bsp_es8388.c
    es8388_task.c

    # auadc.c
    # audac.c
    audio_record_play.c
    bflb_romfs.c
)
sdk_set_main_file(main.c)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
project(${PROJECT_NAME})
