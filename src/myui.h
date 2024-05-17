
#ifndef _MY_UI_H
#define _MY_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined __has_include
#if __has_include("lvgl.h")
#include "lvgl.h"
#elif __has_include("lvgl/lvgl.h")
#include "lvgl/lvgl.h"
#else
#include "lvgl.h"
#endif
#else
#include "lvgl.h"
#endif

#include "lv_i18n.h"
#include "ui_helpers.h"
#include "ui_events.h"

//#define ONLINE_COLOR 0x2BC27B

extern lv_obj_t * ui_pnlInvPower1;

extern lv_obj_t * ui_pnlInvPower2;
extern lv_obj_t * ui_pnlInvPower3;
extern lv_obj_t * ui_pnlInvPower4;
void myui_MainScreen_screen_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
