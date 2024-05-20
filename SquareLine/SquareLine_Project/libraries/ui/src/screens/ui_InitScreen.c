// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.3
// Project name: SquareLine_Project

#include "../ui.h"

void ui_InitScreen_screen_init(void)
{
    ui_InitScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_InitScreen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_flex_flow(ui_InitScreen, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_InitScreen, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_set_style_bg_color(ui_InitScreen, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_InitScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_color(ui_InitScreen, lv_color_hex(0x6F6262), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_main_stop(ui_InitScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_stop(ui_InitScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui_InitScreen, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_InitScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_InitScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_InitScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_InitScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_InitScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_InitScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel2 = lv_obj_create(ui_InitScreen);
    lv_obj_set_width(ui_Panel2, lv_pct(100));
    lv_obj_set_height(ui_Panel2, lv_pct(50));
    lv_obj_set_align(ui_Panel2, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel2, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Panel2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblDate = lv_label_create(ui_Panel2);
    lv_obj_set_width(ui_lblDate, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblDate, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblDate, -7);
    lv_obj_set_y(ui_lblDate, 13);
    lv_obj_set_align(ui_lblDate, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblDate, "2.      8.      THU");
    lv_obj_set_style_text_font(ui_lblDate, &ui_font_digitalWatch32, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblTime = lv_label_create(ui_Panel2);
    lv_obj_set_width(ui_lblTime, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_lblTime, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblTime, -7);
    lv_obj_set_y(ui_lblTime, 13);
    lv_obj_set_align(ui_lblTime, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblTime, "14 : 30");
    lv_obj_set_style_text_color(ui_lblTime, lv_color_hex(0x3EB02C), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblTime, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblTime, &ui_font_digitalWatch56, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel1 = lv_obj_create(ui_InitScreen);
    lv_obj_set_width(ui_Panel1, lv_pct(100));
    lv_obj_set_height(ui_Panel1, lv_pct(25));
    lv_obj_set_x(ui_Panel1, 0);
    lv_obj_set_y(ui_Panel1, 12);
    lv_obj_set_align(ui_Panel1, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel1, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Panel1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel1, lv_color_hex(0xCBC4F0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblCapacityDisplay = lv_label_create(ui_Panel1);
    lv_obj_set_width(ui_lblCapacityDisplay, lv_pct(100));
    lv_obj_set_height(ui_lblCapacityDisplay, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblCapacityDisplay, -7);
    lv_obj_set_y(ui_lblCapacityDisplay, 13);
    lv_obj_set_align(ui_lblCapacityDisplay, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblCapacityDisplay, "    KVA             CELL");
    lv_obj_set_style_text_align(ui_lblCapacityDisplay, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblCapacityDisplay, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label4 = lv_label_create(ui_Panel1);
    lv_obj_set_width(ui_Label4, lv_pct(100));
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label4, -6);
    lv_obj_set_y(ui_Label4, 1);
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4, "IN           V      OUT           V");
    lv_obj_set_style_text_align(ui_Label4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label4, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblCapacity = lv_label_create(ui_Panel1);
    lv_obj_set_width(ui_lblCapacity, lv_pct(15));
    lv_obj_set_height(ui_lblCapacity, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblCapacity, -132);
    lv_obj_set_y(ui_lblCapacity, -19);
    lv_obj_set_align(ui_lblCapacity, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblCapacity, "30");
    lv_obj_add_flag(ui_lblCapacity, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_color(ui_lblCapacity, lv_color_hex(0xD47085), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblCapacity, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_lblCapacity, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblCapacity, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblBatteryCellNo = lv_label_create(ui_Panel1);
    lv_obj_set_width(ui_lblBatteryCellNo, lv_pct(15));
    lv_obj_set_height(ui_lblBatteryCellNo, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblBatteryCellNo, 14);
    lv_obj_set_y(ui_lblBatteryCellNo, -18);
    lv_obj_set_align(ui_lblBatteryCellNo, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblBatteryCellNo, "30");
    lv_obj_add_flag(ui_lblBatteryCellNo, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_color(ui_lblBatteryCellNo, lv_color_hex(0xD47085), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblBatteryCellNo, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_lblBatteryCellNo, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblBatteryCellNo, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblInputVol = lv_label_create(ui_Panel1);
    lv_obj_set_width(ui_lblInputVol, lv_pct(15));
    lv_obj_set_height(ui_lblInputVol, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblInputVol, -82);
    lv_obj_set_y(ui_lblInputVol, 18);
    lv_obj_set_align(ui_lblInputVol, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblInputVol, "220");
    lv_obj_add_flag(ui_lblInputVol, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_color(ui_lblInputVol, lv_color_hex(0xD47085), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblInputVol, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_lblInputVol, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblInputVol, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_lblOutputVol = lv_label_create(ui_Panel1);
    lv_obj_set_width(ui_lblOutputVol, lv_pct(15));
    lv_obj_set_height(ui_lblOutputVol, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_lblOutputVol, 104);
    lv_obj_set_y(ui_lblOutputVol, 18);
    lv_obj_set_align(ui_lblOutputVol, LV_ALIGN_CENTER);
    lv_label_set_text(ui_lblOutputVol, "220");
    lv_obj_add_flag(ui_lblOutputVol, LV_OBJ_FLAG_FLOATING);     /// Flags
    lv_obj_set_style_text_color(ui_lblOutputVol, lv_color_hex(0xD47085), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_lblOutputVol, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui_lblOutputVol, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_lblOutputVol, &lv_font_montserrat_26, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel3 = lv_obj_create(ui_InitScreen);
    lv_obj_set_width(ui_Panel3, lv_pct(100));
    lv_obj_set_height(ui_Panel3, lv_pct(25));
    lv_obj_set_x(ui_Panel3, 0);
    lv_obj_set_y(ui_Panel3, 12);
    lv_obj_set_align(ui_Panel3, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel3, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Panel3, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Panel3, lv_color_hex(0xCBC4F0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Panel3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button1 = lv_btn_create(ui_Panel3);
    lv_obj_set_height(ui_Button1, 50);
    lv_obj_set_width(ui_Button1, lv_pct(50));
    lv_obj_set_x(ui_Button1, 1);
    lv_obj_set_y(ui_Button1, -6);
    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Label2 = lv_label_create(ui_Button1);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "ENTER");
    lv_obj_set_style_text_font(ui_Label2, &lv_font_montserrat_32, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Button1, ui_event_Button1, LV_EVENT_ALL, NULL);

}
