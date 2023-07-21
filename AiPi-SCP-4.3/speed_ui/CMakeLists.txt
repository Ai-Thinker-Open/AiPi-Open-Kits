SET(SOURCES speed_ui/custom/custom.c
    speed_ui/generated/events_init.c
    speed_ui/generated/gui_guider.c
    speed_ui/generated/setup_scr_record.c
    speed_ui/generated/setup_scr_speed.c
    
    )

add_library(ui ${SOURCES})
