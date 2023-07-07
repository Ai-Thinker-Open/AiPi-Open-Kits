SET(SOURCES components/ui_comp_scrolldots.c
    components/ui_comp.c
    screens/ui_Screen4.c
    screens/ui_Screen1.c
    screens/ui_Screen2.c
    screens/ui_Screen3.c
    screens/ui_Screen5.c
    screens/ui_Screen6.c
    screens/ui_Screen7.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_ai_png.c
    images/ui_img_240x320_png.c)

add_library(ui ${SOURCES})
