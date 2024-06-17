// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.3
// Project name: SquareLine_Project

#include "../ui.h"

void ui_scrAlarm1_screen_init(void)
{
    ui_scrAlarm1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_scrAlarm1, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM);      /// Flags
    lv_obj_set_flex_flow(ui_scrAlarm1, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_scrAlarm1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_set_style_bg_color(ui_scrAlarm1, lv_color_hex(0xDCD4D4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_scrAlarm1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_scrAlarm1, lv_color_hex(0x6F6262), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_scrAlarm1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_scrAlarm1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_scrAlarm1, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_scrAlarm1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_scrAlarm1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_scrAlarm1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_scrAlarm1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_scrAlarm1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_scrAlarm1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_radius(ui_scrAlarm1, 20, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_scrAlarm1, lv_color_hex(0xEE2F2F), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_scrAlarm1, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_scrAlarm1, lv_color_hex(0xFFFFFF), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_scrAlarm1, lv_color_hex(0x000000), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_scrAlarm1, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_scrAlarm1, 15, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_scrAlarm1, LV_BORDER_SIDE_FULL, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_scrAlarm1, lv_color_hex(0x000000), LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_scrAlarm1, 255, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui_scrAlarm1, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(ui_scrAlarm1, 1, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    ui_TabView2 = lv_tabview_create(ui_scrAlarm1, LV_DIR_TOP, 50);
    lv_obj_set_width(ui_TabView2, lv_pct(100));
    lv_obj_set_height(ui_TabView2, lv_pct(83));
    lv_obj_set_align(ui_TabView2, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_TabView2, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE |
                      LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags

    lv_obj_set_style_pad_left(lv_tabview_get_tab_btns(ui_TabView2), 0,  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(lv_tabview_get_tab_btns(ui_TabView2), 0,  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(lv_tabview_get_tab_btns(ui_TabView2), 0,  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(lv_tabview_get_tab_btns(ui_TabView2), 0,  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(lv_tabview_get_tab_btns(ui_TabView2), 0,  LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(lv_tabview_get_tab_btns(ui_TabView2), 0,  LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_font(lv_tabview_get_tab_btns(ui_TabView2), &ui_font_malgun20,  LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(lv_tabview_get_tab_btns(ui_TabView2), 0,  LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(lv_tabview_get_tab_btns(ui_TabView2), lv_color_hex(0x706D6D),
                              LV_PART_ITEMS | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(lv_tabview_get_tab_btns(ui_TabView2), 255,  LV_PART_ITEMS | LV_STATE_DEFAULT);

    ui_TabPage6 = lv_tabview_add_tab(ui_TabView2, _("alarmHistory"));
    lv_obj_clear_flag(ui_TabPage6, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE |
                      LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags

    ui_alarmTextArea = lv_textarea_create(ui_TabPage6);
    lv_obj_set_width(ui_alarmTextArea, lv_pct(100));
    lv_obj_set_height(ui_alarmTextArea, lv_pct(100));
    lv_obj_set_x(ui_alarmTextArea, 0);
    lv_obj_set_y(ui_alarmTextArea, -3);
    lv_obj_set_align(ui_alarmTextArea, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_alarmTextArea, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE |
                      LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_text_letter_space(ui_alarmTextArea, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui_alarmTextArea, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_alarmTextArea, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_text_font(ui_alarmTextArea, &ui_font_malgun20, LV_PART_SELECTED | LV_STATE_DEFAULT);


    ui_TabPage1 = lv_tabview_add_tab(ui_TabView2, _("alarmStatus"));
    lv_obj_set_flex_flow(ui_TabPage1, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_TabPage1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_TabPage1, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE |
                      LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_pad_left(ui_TabPage1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_TabPage1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_TabPage1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_TabPage1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_TabPage1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_TabPage1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Container10 = lv_obj_create(ui_TabPage1);
    lv_obj_remove_style_all(ui_Container10);
    lv_obj_set_width(ui_Container10, lv_pct(100));
    lv_obj_set_height(ui_Container10, lv_pct(100));
    lv_obj_set_x(ui_Container10, 2);
    lv_obj_set_y(ui_Container10, 26);
    lv_obj_set_flex_flow(ui_Container10, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Container10, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_Container10, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_border_color(ui_Container10, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Container10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Container10, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_row(ui_Container10, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Container10, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    ui_eventTextArea = lv_textarea_create(ui_Container10);
    lv_obj_set_width(ui_eventTextArea, lv_pct(100));
    lv_obj_set_height(ui_eventTextArea, lv_pct(100));
    lv_obj_set_x(ui_eventTextArea, 0);
    lv_obj_set_y(ui_eventTextArea, -3);
    lv_obj_set_align(ui_eventTextArea, LV_ALIGN_CENTER);
    lv_textarea_set_placeholder_text(ui_eventTextArea, _("None"));
    lv_obj_clear_flag(ui_eventTextArea, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_text_letter_space(ui_eventTextArea, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui_eventTextArea, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_eventTextArea, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);



    ui_Panel61 = lv_obj_create(ui_scrAlarm1);
    lv_obj_set_width(ui_Panel61, lv_pct(100));
    lv_obj_set_height(ui_Panel61, lv_pct(17));
    lv_obj_set_x(ui_Panel61, -1);
    lv_obj_set_y(ui_Panel61, 0);
    lv_obj_set_align(ui_Panel61, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel61, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel61, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_Panel61, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel61, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel61, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel61, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel61, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel61, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel61, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_Panel61, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel61, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel61, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel61, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel61, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Panel61, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel61, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button13 = lv_btn_create(ui_Panel61);
    lv_obj_set_height(ui_Button13, lv_pct(100));
    lv_obj_set_flex_grow(ui_Button13, 1);
    lv_obj_set_x(ui_Button13, -276);
    lv_obj_set_y(ui_Button13, -1);
    lv_obj_set_align(ui_Button13, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Button13, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Button13, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button13, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button13, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Button13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button13, lv_color_hex(0xF06418), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button13, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Button13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Button13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Button13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Button13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label127 = lv_label_create(ui_Button13);
    lv_obj_set_width(ui_Label127, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label127, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label127, -8);
    lv_obj_set_y(ui_Label127, 59);
    lv_obj_set_align(ui_Label127, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Label127, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label127, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_label_set_text(ui_Label127, _("ALARM_RST"));
    lv_obj_set_style_text_color(ui_Label127, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label127, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label127, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label127, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnAlarmPrev2 = lv_btn_create(ui_Panel61);
    lv_obj_set_height(ui_btnAlarmPrev2, lv_pct(100));
    lv_obj_set_flex_grow(ui_btnAlarmPrev2, 1);
    lv_obj_set_x(ui_btnAlarmPrev2, -276);
    lv_obj_set_y(ui_btnAlarmPrev2, -1);
    lv_obj_set_align(ui_btnAlarmPrev2, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_btnAlarmPrev2, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_btnAlarmPrev2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(ui_btnAlarmPrev2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_btnAlarmPrev2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_btnAlarmPrev2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_btnAlarmPrev2, lv_color_hex(0xF06418), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnAlarmPrev2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_btnAlarmPrev2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_btnAlarmPrev2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_btnAlarmPrev2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_btnAlarmPrev2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label122 = lv_label_create(ui_btnAlarmPrev2);
    lv_obj_set_width(ui_Label122, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label122, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label122, -8);
    lv_obj_set_y(ui_Label122, 59);
    lv_obj_set_align(ui_Label122, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Label122, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label122, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_label_set_text(ui_Label122, _("Prev"));
    lv_obj_set_style_text_color(ui_Label122, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label122, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label122, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label122, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnAlarmNext2 = lv_btn_create(ui_Panel61);
    lv_obj_set_height(ui_btnAlarmNext2, lv_pct(100));
    lv_obj_set_flex_grow(ui_btnAlarmNext2, 1);
    lv_obj_set_x(ui_btnAlarmNext2, -276);
    lv_obj_set_y(ui_btnAlarmNext2, -1);
    lv_obj_set_align(ui_btnAlarmNext2, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_btnAlarmNext2, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_btnAlarmNext2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(ui_btnAlarmNext2, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_btnAlarmNext2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_btnAlarmNext2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_btnAlarmNext2, lv_color_hex(0xF06418), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnAlarmNext2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_btnAlarmNext2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_btnAlarmNext2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_btnAlarmNext2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_btnAlarmNext2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label124 = lv_label_create(ui_btnAlarmNext2);
    lv_obj_set_width(ui_Label124, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label124, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label124, -8);
    lv_obj_set_y(ui_Label124, 59);
    lv_obj_set_align(ui_Label124, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Label124, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label124, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_label_set_text(ui_Label124, _("Next"));
    lv_obj_set_style_text_color(ui_Label124, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label124, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label124, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label124, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button9 = lv_btn_create(ui_Panel61);
    lv_obj_set_height(ui_Button9, lv_pct(100));
    lv_obj_set_flex_grow(ui_Button9, 1);
    lv_obj_set_align(ui_Button9, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Button9, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Button9, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button9, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button9, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Button9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button9, lv_color_hex(0xF06418), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Button9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Button9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Button9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Button9, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label125 = lv_label_create(ui_Button9);
    lv_obj_set_width(ui_Label125, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label125, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label125, -8);
    lv_obj_set_y(ui_Label125, 59);
    lv_obj_set_align(ui_Label125, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Label125, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label125, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_label_set_text(ui_Label125, _("Back"));
    lv_obj_set_style_text_color(ui_Label125, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label125, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label125, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label125, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Button13, ui_event_Button13, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_btnAlarmPrev2, ui_event_btnAlarmPrev2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_btnAlarmNext2, ui_event_btnAlarmNext2, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button9, ui_event_Button9, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_scrAlarm1, ui_event_scrAlarm1, LV_EVENT_ALL, NULL);

}
