SET(SOURCES fonts/*.c
    imgs/image_language/*.c
    imgs/image_light/*.c
    imgs/image_standby/*.c
    imgs/image_wash/*.c
    layer_manage/*.c
    *.c
    imgs/*.c
    )


# sdk_add_include_directories(.)
# sdk_add_include_directories(layer_manage)
add_library(ui ${SOURCES})
