
#include "ui.h"
#include "myui.h"
#include "ui_helpers.h"
//lv_obj_t * ui_pnlInvPower1;
//lv_obj_t * ui_pnlInvPower2;
//lv_obj_t * ui_pnlInvPower3;
//lv_obj_t * ui_pnlInvPower4;

#define ONLINE_COLOR 0x20945E 

void myui_MainScreen_screen_init(void){
    // ui_pnlInvPower1 = lv_obj_create(ui_Container4);
    // lv_obj_set_width(ui_pnlInvPower1, 27);
    // lv_obj_set_height(ui_pnlInvPower1, 14);
    // lv_obj_set_x(ui_pnlInvPower1, 150);   //inv scr에서 수평으로 가는 라인
    // lv_obj_set_y(ui_pnlInvPower1, 6);
    // lv_obj_set_align(ui_pnlInvPower1, LV_ALIGN_CENTER);
    // lv_obj_add_flag(ui_pnlInvPower1, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_FLOATING);     /// Flags
    // lv_obj_clear_flag(ui_pnlInvPower1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    // lv_obj_set_style_radius(ui_pnlInvPower1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_pnlInvPower1, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(ui_pnlInvPower1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlInvPower1, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_opa(ui_pnlInvPower1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_width(ui_pnlInvPower1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_pnlInvPower2 = lv_obj_create(ui_Container4); // 바이패스에서 수평으로 가는 라인. 
    // lv_obj_set_width(ui_pnlInvPower2, 43);
    // lv_obj_set_height(ui_pnlInvPower2, 14);
    // lv_obj_set_x(ui_pnlInvPower2, 156);
    // lv_obj_set_y(ui_pnlInvPower2, -72);
    // lv_obj_set_align(ui_pnlInvPower2, LV_ALIGN_CENTER);
    // lv_obj_add_flag(ui_pnlInvPower2, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_FLOATING);     /// Flags
    // lv_obj_clear_flag(ui_pnlInvPower2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    // lv_obj_set_style_radius(ui_pnlInvPower2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_pnlInvPower2, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(ui_pnlInvPower2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlInvPower2, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_opa(ui_pnlInvPower2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_width(ui_pnlInvPower2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_pnlInvPower3 = lv_obj_create(ui_Container4); // 바이패스에서 수직으로 가는 라인. 
    // lv_obj_set_width(ui_pnlInvPower3, 14);
    // lv_obj_set_height(ui_pnlInvPower3, 80);
    // lv_obj_set_x(ui_pnlInvPower3, 171);
    // lv_obj_set_y(ui_pnlInvPower3, -39);
    // lv_obj_set_align(ui_pnlInvPower3, LV_ALIGN_CENTER);
    // lv_obj_add_flag(ui_pnlInvPower3, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_FLOATING);     /// Flags
    // lv_obj_clear_flag(ui_pnlInvPower3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    // lv_obj_set_style_radius(ui_pnlInvPower3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_pnlInvPower3, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(ui_pnlInvPower3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlInvPower3, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_opa(ui_pnlInvPower3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_width(ui_pnlInvPower3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_pnlInvPower4 = lv_obj_create(ui_Container4);
    // lv_obj_set_width(ui_pnlInvPower4, 35);
    // lv_obj_set_height(ui_pnlInvPower4, 14);
    // lv_obj_set_x(ui_pnlInvPower4, 181);
    // lv_obj_set_y(ui_pnlInvPower4, 6);
    // lv_obj_set_align(ui_pnlInvPower4, LV_ALIGN_CENTER);
    // lv_obj_add_flag(ui_pnlInvPower4, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_FLOATING);     /// Flags
    // lv_obj_clear_flag(ui_pnlInvPower4, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    // lv_obj_set_style_radius(ui_pnlInvPower4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_pnlInvPower4, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(ui_pnlInvPower4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlInvPower4, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_opa(ui_pnlInvPower4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_width(ui_pnlInvPower4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);



    // lv_obj_set_style_bg_color(ui_pnlMainPower1, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlMainPower1, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_pnlMainPower2, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlMainPower2, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_pnlMainPower3, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlMainPower3, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    // lv_obj_set_style_bg_color(ui_pnlConvPower1, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlConvPower1, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_pnlConvPower2, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlConvPower2, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_pnlConvPower3, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlConvPower3, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);

    // lv_obj_set_style_bg_color(ui_pnlInvPower, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_color(ui_pnlInvPower, lv_color_hex(ONLINE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void lockLogTextArea(lv_obj_t *ta)
{
    if (ta == NULL) {
        return;
    }
    lv_obj_clear_flag(ta, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_PRESS_LOCK |
                      LV_OBJ_FLAG_CLICK_FOCUSABLE | LV_OBJ_FLAG_SCROLLABLE |
                      LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM |
                      LV_OBJ_FLAG_SCROLL_CHAIN | LV_OBJ_FLAG_SCROLL_ON_FOCUS);
    lv_obj_set_scrollbar_mode(ta, LV_SCROLLBAR_MODE_OFF);
    lv_textarea_set_cursor_click_pos(ta, false);
}

/* SquareLine은 TabView 본체 플래그만 건드린다. 스와이프는 자식 content가 담당.
 * 탭하면 elastic/snap 때문에 TextArea 글자가 잠깐 흔들린다. */
void myui_scrAlarm1_lock_scroll(void)
{
    lv_obj_t *cont = lv_tabview_get_content(ui_TabView2);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);

    lockLogTextArea(ui_alarmTextArea);
    lockLogTextArea(ui_eventTextArea);
}