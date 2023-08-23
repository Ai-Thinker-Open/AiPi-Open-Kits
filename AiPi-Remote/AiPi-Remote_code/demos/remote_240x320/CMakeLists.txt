# SET(SOURCES custom/custom.c
# custom/lv_dclock.c
# generated/events_init.c
# generated/gui_guider.c
# generated/setup_scr_cont_4.c
# generated/setup_scr_WiFi_config.c
# generated/guider_fonts/lv_font_Antonio_Regular_43.c
# generated/guider_fonts/lv_font_montserratMedium_16.c
# generated/guider_fonts/lv_font_montserratMedium_12.c
# generated/guider_fonts/lv_font_simhei_10.c
# generated/guider_fonts/lv_font_simhei_16.c
# generated/guider_fonts/lv_font_simhei_25.c
# generated/guider_fonts/lv_font_simhei_13.c
# generated/guider_fonts/lv_font_simsun_12.c
# generated/images/_wifi_alpha_16x16.c
# )
file(GLOB_RECURSE SOURCES "custom/*.c" "guider_fonts/*.c" "images/*.c" "./*.c")
sdk_add_include_directories(.)

add_library(ui ${SOURCES})
