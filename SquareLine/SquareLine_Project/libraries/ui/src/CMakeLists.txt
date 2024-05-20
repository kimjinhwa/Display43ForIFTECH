SET(SOURCES screens/ui_InitScreen.c
    screens/ui_MainScreen.c
    screens/ui_SettingScreen.c
    screens/ui_scrControll.c
    screens/ui_scrMeasure.c
    screens/ui_scrAlarm1.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    ui_events.cpp
    images/ui_img_alarm40_40_png.c
    images/ui_img_flowmain1_png.c
    images/ui_img_inverteracdc_png.c
    images/ui_img_batterybank_png.c
    images/ui_img_scr_png.c
    images/ui_img_outputpowerline_png.c
    images/ui_img_outputpowerlineon_png.c
    images/ui_img_alarm1_png.c
    images/ui_img_alarm25_25_png.c
    images/ui_img_alarm30_30_png.c
    images/ui_img_convpwrline_1_png.c
    images/ui_img_convpwrline_2_png.c
    fonts/ui_font_digitalWatch20.c
    fonts/ui_font_digitalWatch26.c
    fonts/ui_font_digitalWatch32.c
    fonts/ui_font_digitalWatch56.c
    fonts/ui_font_malgun14.c
    fonts/ui_font_malgun20.c
    fonts/ui_font_malgun26.c)

add_library(ui ${SOURCES})
