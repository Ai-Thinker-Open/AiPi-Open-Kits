cmake_minimum_required(VERSION 3.15)

include(proj.conf)

find_package(bouffalo_sdk REQUIRED HINTS $ENV{BL_SDK_BASE})

sdk_add_include_directories(rtos/include .)
sdk_add_include_directories(video)
# sdk_add_include_directories(audio)
sdk_add_include_directories(web)

# User
sdk_add_compile_definitions(-DCONFIG_CLI_CMD_ENABLE)
#sdk_add_compile_definitions(-DBL616_DHCP_DEBUG)
# sdk_add_compile_definitions(-DSIMU_DATA_TEST)
sdk_add_compile_definitions(-DCONFIG_PSRAM_COPY_CODE)
sdk_add_compile_definitions(-DCONFIG_USB_EHCI_ISO)

target_sources(app PRIVATE
    rtos/bl_cam_frame.c
    video/jpeg_head.c video/mjpeg.c video/dbi_disp.c video/pbuff_dvp.c
    # audio/auadc.c audio/audac.c
    web/mlwip_https.c
)

if(CONFIG_CHERRYUSB)
# add_subdirectory(${BL_SDK_BASE}/components/usb/usbh_uvc_uac usbh_uvc_uac)
# sdk_add_static_library(libusbh_uvc_uac.a libhibooster.a)
sdk_add_static_library(libusbh_uvc_uac.a)
target_sources(app PRIVATE 
video/usbh_uvc_callback.c 
# audio/usbh_uac_callback.c
)
endif()

sdk_set_linker_script(bl616_psram.ld)
sdk_set_main_file(main.c)

project(image_transmission)
