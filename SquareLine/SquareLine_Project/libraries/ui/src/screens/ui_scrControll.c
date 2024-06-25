// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.3
// Project name: SquareLine_Project

#include "../ui.h"

void ui_scrControll_screen_init(void)
{
    ui_scrControll = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_scrControll, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_ELASTIC |
                      LV_OBJ_FLAG_SCROLL_MOMENTUM);    /// Flags
    lv_obj_set_flex_flow(ui_scrControll, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_scrControll, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(ui_scrControll, lv_color_hex(0xDCD4D4), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_scrControll, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_scrControll, lv_color_hex(0x6F6262), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_scrControll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_scrControll, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_scrControll, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_scrControll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_scrControll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_scrControll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_scrControll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_scrControll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_scrControll, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Container2 = lv_obj_create(ui_scrControll);
    lv_obj_remove_style_all(ui_Container2);
    lv_obj_set_width(ui_Container2, lv_pct(98));
    lv_obj_set_height(ui_Container2, lv_pct(98));
    lv_obj_set_x(ui_Container2, 2);
    lv_obj_set_y(ui_Container2, 26);
    lv_obj_set_flex_flow(ui_Container2, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Container2, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_Container2, LV_OBJ_FLAG_GESTURE_BUBBLE | LV_OBJ_FLAG_SNAPPABLE | LV_OBJ_FLAG_SCROLLABLE |
                      LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_CHAIN);     /// Flags
    lv_obj_set_style_bg_color(ui_Container2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Container2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Container2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Container2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Container2, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Container2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Container2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Container2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Container2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Container2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Container2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_row(ui_Container2, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Container2, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    ui_Panel6 = lv_obj_create(ui_Container2);
    lv_obj_set_width(ui_Panel6, lv_pct(100));
    lv_obj_set_height(ui_Panel6, lv_pct(20));
    lv_obj_set_align(ui_Panel6, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel6, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel6, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel6, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel6, lv_color_hex(0x706D6D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel6, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel6, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel6, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel6, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_row(ui_Panel6, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel6, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    ui_Label1 = lv_label_create(ui_Panel6);
    lv_obj_set_width(ui_Label1, lv_pct(50));
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_Label1, 927);
    lv_obj_set_y(ui_Label1, -270);
    lv_obj_set_align(ui_Label1, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Label1, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Label1, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, _("UPS_SYSTEM"));
    lv_label_set_recolor(ui_Label1, "true");
    lv_obj_set_style_text_align(ui_Label1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label1, &ui_font_malgun26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Label1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnRunUps = lv_btn_create(ui_Panel6);
    lv_obj_set_width(ui_btnRunUps, 100);
    lv_obj_set_height(ui_btnRunUps, lv_pct(96));
    lv_obj_set_align(ui_btnRunUps, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_btnRunUps, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_btnRunUps, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_btnRunUps, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_btnRunUps, lv_color_hex(0xCAC8C8), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnRunUps, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_btnRunUps, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_btnRunUps, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_btnRunUps, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui_btnRunUps, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label5 = lv_label_create(ui_btnRunUps);
    lv_obj_set_width(ui_Label5, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label5, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5, "ON");
    lv_obj_set_style_text_color(ui_Label5, lv_color_hex(0x070707), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label5, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel34 = lv_obj_create(ui_Container2);
    lv_obj_set_width(ui_Panel34, lv_pct(100));
    lv_obj_set_height(ui_Panel34, lv_pct(20));
    lv_obj_set_align(ui_Panel34, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel34, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel34, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel34, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel34, lv_color_hex(0x706D6D), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel34, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel34, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel34, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel34, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel34, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_row(ui_Panel34, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel34, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    ui_Label66 = lv_label_create(ui_Panel34);
    lv_obj_set_width(ui_Label66, lv_pct(50));
    lv_obj_set_height(ui_Label66, LV_SIZE_CONTENT);    /// 100
    lv_obj_set_x(ui_Label66, 927);
    lv_obj_set_y(ui_Label66, -270);
    lv_obj_set_align(ui_Label66, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Label66, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Label66, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    lv_label_set_text(ui_Label66, _("UPS_SYSTEM"));
    lv_label_set_recolor(ui_Label66, "true");
    lv_obj_set_style_text_align(ui_Label66, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label66, &ui_font_malgun26, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Label66, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Label66, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label66, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_btnStopUps1 = lv_btn_create(ui_Panel34);
    lv_obj_set_width(ui_btnStopUps1, 100);
    lv_obj_set_height(ui_btnStopUps1, 50);
    lv_obj_set_align(ui_btnStopUps1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_btnStopUps1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_btnStopUps1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_btnStopUps1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_btnStopUps1, lv_color_hex(0xF80D29), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_btnStopUps1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_btnStopUps1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_btnStopUps1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_btnStopUps1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label68 = lv_label_create(ui_btnStopUps1);
    lv_obj_set_width(ui_Label68, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label68, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label68, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label68, "OFF");
    lv_obj_set_style_text_color(ui_Label68, lv_color_hex(0x070707), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label68, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label68, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label68, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel7 = lv_obj_create(ui_Container2);
    lv_obj_set_width(ui_Panel7, lv_pct(100));
    lv_obj_set_height(ui_Panel7, lv_pct(70));
    lv_obj_set_align(ui_Panel7, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel7, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Panel7, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_clear_flag(ui_Panel7, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel7, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel7, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_pad_left(ui_Panel7, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel7, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel7, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel7, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Panel7, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel7, 0, LV_PART_SCROLLBAR | LV_STATE_DEFAULT);

    ui_Label26 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_Label26, lv_pct(45));
    lv_obj_set_height(ui_Label26, LV_SIZE_CONTENT);    /// 20
    lv_obj_set_x(ui_Label26, 927);
    lv_obj_set_y(ui_Label26, -270);
    lv_obj_set_align(ui_Label26, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Label26, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label26, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_Label26, _("dcLink"));
    lv_obj_set_style_text_align(ui_Label26, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label26, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label26, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblDcLinkVol = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_lblDcLinkVol, lv_pct(10));
    lv_obj_set_height(ui_lblDcLinkVol, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblDcLinkVol, 927);
    lv_obj_set_y(ui_lblDcLinkVol, -270);
    lv_obj_set_align(ui_lblDcLinkVol, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_lblDcLinkVol, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_lblDcLinkVol, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_lblDcLinkVol, _("125"));
    lv_obj_set_style_text_color(ui_lblDcLinkVol, lv_color_hex(0xF40C0C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblDcLinkVol, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_lblDcLinkVol, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblDcLinkVol, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_lblDcLinkVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_lblDcLinkVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_lblDcLinkVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_lblDcLinkVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_lblDcLinkVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_lblDcLinkVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label28 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_Label28, lv_pct(30));
    lv_obj_set_height(ui_Label28, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label28, 927);
    lv_obj_set_y(ui_Label28, -270);
    lv_obj_set_align(ui_Label28, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Label28, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label28, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_Label28, _("V"));
    lv_obj_set_style_text_align(ui_Label28, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label28, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label28, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label29 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_Label29, lv_pct(45));
    lv_obj_set_height(ui_Label29, LV_SIZE_CONTENT);    /// 20
    lv_obj_set_x(ui_Label29, 927);
    lv_obj_set_y(ui_Label29, -270);
    lv_obj_set_align(ui_Label29, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Label29, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label29, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_Label29, _("Battery"));
    lv_obj_set_style_text_align(ui_Label29, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label29, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label29, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblBatteryVol = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_lblBatteryVol, lv_pct(10));
    lv_obj_set_height(ui_lblBatteryVol, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblBatteryVol, 927);
    lv_obj_set_y(ui_lblBatteryVol, -270);
    lv_obj_set_align(ui_lblBatteryVol, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_lblBatteryVol, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_lblBatteryVol, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_lblBatteryVol, _("125"));
    lv_obj_set_style_text_color(ui_lblBatteryVol, lv_color_hex(0xF40C0C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblBatteryVol, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_lblBatteryVol, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblBatteryVol, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_lblBatteryVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_lblBatteryVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_lblBatteryVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_lblBatteryVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_lblBatteryVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_lblBatteryVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label31 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_Label31, lv_pct(30));
    lv_obj_set_height(ui_Label31, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label31, 927);
    lv_obj_set_y(ui_Label31, -270);
    lv_obj_set_align(ui_Label31, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Label31, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label31, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_Label31, _("V"));
    lv_obj_set_style_text_align(ui_Label31, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label31, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label31, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label32 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_Label32, lv_pct(45));
    lv_obj_set_height(ui_Label32, LV_SIZE_CONTENT);    /// 20
    lv_obj_set_x(ui_Label32, 927);
    lv_obj_set_y(ui_Label32, -270);
    lv_obj_set_align(ui_Label32, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Label32, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label32, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_Label32, _("Inverter"));
    lv_obj_set_style_text_align(ui_Label32, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label32, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label32, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblInverterVol = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_lblInverterVol, lv_pct(10));
    lv_obj_set_height(ui_lblInverterVol, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblInverterVol, 927);
    lv_obj_set_y(ui_lblInverterVol, -270);
    lv_obj_set_align(ui_lblInverterVol, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_lblInverterVol, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_lblInverterVol, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_lblInverterVol, _("125"));
    lv_obj_set_style_text_color(ui_lblInverterVol, lv_color_hex(0xF40C0C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblInverterVol, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_lblInverterVol, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblInverterVol, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_lblInverterVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_lblInverterVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_lblInverterVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_lblInverterVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_lblInverterVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_lblInverterVol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label34 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_Label34, lv_pct(30));
    lv_obj_set_height(ui_Label34, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label34, 927);
    lv_obj_set_y(ui_Label34, -270);
    lv_obj_set_align(ui_Label34, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Label34, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label34, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_Label34, _("V"));
    lv_obj_set_style_text_align(ui_Label34, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label34, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label34, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label35 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_Label35, lv_pct(45));
    lv_obj_set_height(ui_Label35, LV_SIZE_CONTENT);    /// 20
    lv_obj_set_x(ui_Label35, 927);
    lv_obj_set_y(ui_Label35, -270);
    lv_obj_set_align(ui_Label35, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Label35, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label35, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_Label35, _("output"));
    lv_obj_set_style_text_align(ui_Label35, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label35, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label35, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblOutputV = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_lblOutputV, lv_pct(10));
    lv_obj_set_height(ui_lblOutputV, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblOutputV, 927);
    lv_obj_set_y(ui_lblOutputV, -270);
    lv_obj_set_align(ui_lblOutputV, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_lblOutputV, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_lblOutputV, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_lblOutputV, _("125"));
    lv_obj_set_style_text_color(ui_lblOutputV, lv_color_hex(0xF40C0C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblOutputV, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_lblOutputV, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblOutputV, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_lblOutputV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_lblOutputV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_lblOutputV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_lblOutputV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_lblOutputV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_lblOutputV, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label37 = lv_label_create(ui_Panel7);
    lv_obj_set_width(ui_Label37, lv_pct(30));
    lv_obj_set_height(ui_Label37, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label37, 927);
    lv_obj_set_y(ui_Label37, -270);
    lv_obj_set_align(ui_Label37, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Label37, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label37, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_START);
    lv_label_set_text(ui_Label37, _("V"));
    lv_obj_set_style_text_align(ui_Label37, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label37, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Label37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Label37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Label37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Label37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Label37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Label37, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button14 = lv_btn_create(ui_Panel7);
    lv_obj_set_height(ui_Button14, 60);
    lv_obj_set_width(ui_Button14, lv_pct(30));
    lv_obj_set_x(ui_Button14, 11);
    lv_obj_set_y(ui_Button14, 75);
    lv_obj_set_align(ui_Button14, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_flex_flow(ui_Button14, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Button14, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button14, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_FLOATING |
                    LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button14, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Button14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button14, lv_color_hex(0x22991F), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button14, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Button14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Button14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Button14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Button14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Button14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui_Button14, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Button14, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label57 = lv_label_create(ui_Button14);
    lv_obj_set_width(ui_Label57, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label57, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label57, -5);
    lv_obj_set_y(ui_Label57, 0);
    lv_obj_set_align(ui_Label57, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Label57, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Label57, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_label_set_text(ui_Label57, _("Back"));
    lv_obj_set_style_text_color(ui_Label57, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label57, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_Label57, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label57, &ui_font_malgun20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_btnRunUps, ui_event_btnRunUps, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_btnStopUps1, ui_event_btnStopUps1, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Button14, ui_event_Button14, LV_EVENT_ALL, NULL);

}
