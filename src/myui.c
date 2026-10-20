
#include "ui.h"
#include "myui.h"
#include "ui_helpers.h"
#include "../Version.h"
#include "esp_mac.h"
#include <stdio.h>
//lv_obj_t * ui_pnlInvPower1;
//lv_obj_t * ui_pnlInvPower2;
//lv_obj_t * ui_pnlInvPower3;
//lv_obj_t * ui_pnlInvPower4;

#define POWER_LINE_THICK 20

static void thicken_hline(lv_obj_t *obj)
{
    if (obj == NULL) return;
    lv_obj_set_height(obj, POWER_LINE_THICK);
}

static lv_obj_t *ui_lblBatPct;
static lv_obj_t *ui_lblLoadPct;

static void style_capacity_label(lv_obj_t *obj)
{
    if (obj == NULL) return;
    lv_label_set_recolor(obj, true);
    lv_obj_set_style_text_letter_space(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

/* 이름 칸은 왼쪽, 숫자 칸은 오른쪽. 두 줄이 같은 폭이라 시작과 % 끝이 맞는다. */
static lv_obj_t *add_pct_label(lv_obj_t *name, const char *title, lv_coord_t name_w, lv_coord_t pct_w)
{
    if (name == NULL) return NULL;
    lv_obj_set_x(name, lv_obj_get_style_x(name, LV_PART_MAIN) - 10);
    lv_obj_set_width(name, name_w);
    lv_obj_set_style_text_align(name, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_text(name, title);

    lv_obj_t *pct = lv_label_create(lv_obj_get_parent(name));
    lv_obj_set_width(pct, pct_w);
    lv_obj_set_height(pct, LV_SIZE_CONTENT);
    lv_obj_set_align(pct, LV_ALIGN_CENTER);
    lv_obj_set_x(pct, lv_obj_get_style_x(name, LV_PART_MAIN) + (name_w + pct_w) / 2);
    lv_obj_set_y(pct, lv_obj_get_style_y(name, LV_PART_MAIN));
    lv_obj_add_flag(pct, LV_OBJ_FLAG_IGNORE_LAYOUT | LV_OBJ_FLAG_FLOATING);
    lv_obj_set_style_text_font(pct, lv_obj_get_style_text_font(name, LV_PART_MAIN), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(pct, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(pct, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_label_set_recolor(pct, true);
    lv_label_set_text(pct, "0#ff0000 %#");
    return pct;
}

void myui_set_capacity(unsigned bat, unsigned load)
{
    char buf[24];
    if (ui_lblBatPct) {
        snprintf(buf, sizeof(buf), "%u#ff0000 %%#", bat);
        lv_label_set_text(ui_lblBatPct, buf);
    }
    if (ui_lblLoadPct) {
        snprintf(buf, sizeof(buf), "%u#ff0000 %%#", load);
        lv_label_set_text(ui_lblLoadPct, buf);
    }
}

/* 그림의 전력선 대신 패널 보더로 관을 그린다.
 * 가로는 위·아래만, 세로는 좌·우만 남겨 이음 끝의 막힌 선을 없앤다. */
static void style_power_border(lv_obj_t *obj, lv_border_side_t side)
{
    if (obj == NULL) return;
    lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_all(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0xA0A0A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(obj, lv_color_hex(0xA0A0A0), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(obj, side, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void add_setting_unit(lv_obj_t *box, const char *unit)
{
    if (box == NULL || unit == NULL) return;
    lv_obj_t *lbl = lv_label_create(lv_obj_get_parent(box));
    lv_label_set_text(lbl, unit);
    lv_obj_set_style_translate_x(lbl, -7, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(lbl, lv_obj_get_style_text_font(box, LV_PART_MAIN), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(lbl, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
}

static void style_hline_border(lv_obj_t *obj)
{
    style_power_border(obj, LV_BORDER_SIDE_TOP | LV_BORDER_SIDE_BOTTOM);
}

static void style_vline_border(lv_obj_t *obj)
{
    style_power_border(obj, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_RIGHT);
}

void myui_MainScreen_screen_init(void){
    style_hline_border(ui_pnlMainPower1);
    style_vline_border(ui_pnlMainPower2); /* 아래(와 위) 이음은 비움 */
    style_hline_border(ui_pnlMainPower3); /* 왼쪽 이음은 비움 */

    style_hline_border(ui_pnlConvPower1); /* 오른쪽 이음은 비움 */
    style_hline_border(ui_pnlConvPower2); /* 왼쪽 이음은 비움 */
    style_vline_border(ui_pnlConvPower3); /* 위쪽 이음은 비움 */

    style_hline_border(ui_pnlInvPower);
    style_hline_border(ui_pnlInvPower1);
    style_hline_border(ui_pnlInvPower2);
    style_vline_border(ui_pnlInvPower3);
    style_hline_border(ui_pnlInvPower4);

    thicken_hline(ui_pnlMainPower1);
    thicken_hline(ui_pnlMainPower3);
    thicken_hline(ui_pnlConvPower1);
    thicken_hline(ui_pnlConvPower2);
    /* 위 가로는 14→20이라 아래끝이 3px 내려온다. 가운데 정렬이라 높이만 줄이면 아래도 짧아지므로 y를 1 내리고 높이를 4 줄여 위끝만 맞춘다. */
    lv_obj_set_y(ui_pnlConvPower3, lv_obj_get_style_y(ui_pnlConvPower3, LV_PART_MAIN) + 1);
    lv_obj_set_height(ui_pnlConvPower3, lv_obj_get_style_height(ui_pnlConvPower3, LV_PART_MAIN) - 4);
    thicken_hline(ui_pnlInvPower);
    thicken_hline(ui_pnlInvPower1);
    thicken_hline(ui_pnlInvPower2);
    thicken_hline(ui_pnlInvPower4);

    /* 상단 가로선만 좌우로 늘려 둔다. 좌우 모서리 세로는 아래에서 폭 20으로 맞춘다. */
    lv_obj_set_width(ui_pnlMainPower3, lv_obj_get_width(ui_pnlMainPower3) + 6);
    /* 상단만 두꺼워져 왼쪽 세로 위끝이 모서리보다 짧다. 이 선만 위로 늘린다. */
    lv_obj_set_height(ui_pnlMainPower2, lv_obj_get_height(ui_pnlMainPower2) + 6);
    /* 가운데 정렬이라 폭 14→20이면 왼쪽 끝이 3px 더 나가 상단선 왼쪽과 맞는다. */
    lv_obj_set_width(ui_pnlMainPower2, POWER_LINE_THICK);
    /* 오른쪽 세로는 왼쪽 정렬이다. 배치 전 lv_obj_get_x 는 0이라 쓰면 선이 사라진다.
     * 스타일 x(419)에서 늘어난 폭만큼만 왼쪽으로 당겨 오른쪽 끝을 유지한다. */
    {
        lv_coord_t x = lv_obj_get_style_x(ui_pnlInvPower3, LV_PART_MAIN);
        lv_coord_t w = lv_obj_get_style_width(ui_pnlInvPower3, LV_PART_MAIN);
        lv_obj_set_x(ui_pnlInvPower3, x - (POWER_LINE_THICK - w));
        lv_obj_set_width(ui_pnlInvPower3, POWER_LINE_THICK);
    }
    /* 세로를 왼쪽으로 6px 굵힌 뒤에도 아래 가로는 x=419라 오른쪽에서 시작한다. 왼쪽 끝을 맞추고 오른쪽 끝은 유지한다. */
    {
        lv_coord_t x3 = lv_obj_get_style_x(ui_pnlInvPower3, LV_PART_MAIN);
        lv_coord_t x4 = lv_obj_get_style_x(ui_pnlInvPower4, LV_PART_MAIN);
        lv_coord_t w4 = lv_obj_get_style_width(ui_pnlInvPower4, LV_PART_MAIN);
        lv_obj_set_width(ui_pnlInvPower4, w4 + (x4 - x3));
        lv_obj_set_x(ui_pnlInvPower4, x3);
    }
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
    add_setting_unit(ui_txtBatVolSet, "V");
    add_setting_unit(ui_txtBatCurrSet, "A");
    add_setting_unit(ui_txtOutputVolSet, "V");
    add_setting_unit(ui_txtHFMnBatFirstFaultUV, "V");

    style_capacity_label(ui_lbaBatCapacity);
    style_capacity_label(ui_lblLoadCapacity);
    {
        const lv_font_t *font = lv_obj_get_style_text_font(ui_lbaBatCapacity, LV_PART_MAIN);
        lv_coord_t gap = lv_txt_get_width(" ", 1, font, 0, LV_TEXT_FLAG_NONE);
        lv_coord_t name_w = lv_txt_get_width("LOAD", 4, font, 0, LV_TEXT_FLAG_NONE);
        lv_coord_t pct_w = lv_txt_get_width("100%", 4, font, 0, LV_TEXT_FLAG_NONE);
        ui_lblBatPct = add_pct_label(ui_lbaBatCapacity, "#ff0000 BAT#", name_w, pct_w + gap);
        ui_lblLoadPct = add_pct_label(ui_lblLoadCapacity, "#ff0000 LOAD#", name_w, pct_w + gap);
    }
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

/* 초기화면 오른쪽 아래. 생성 화면을 다시 만들지 않으므로 ui_init 이후 한 번이면 남는다. */
void myui_InitScreen_version(void)
{
    uint8_t mac[6];
    char text[40];
    lv_obj_t *lbl;

    if (ui_InitScreen == NULL)
        return;
    if (esp_read_mac(mac, ESP_MAC_WIFI_STA) != ESP_OK) {
        mac[0] = mac[1] = mac[2] = mac[3] = mac[4] = mac[5] = 0;
    }
    snprintf(text, sizeof(text), "V%s\n%02X%02X%02X%02X%02X%02X",
             VERSION, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    lbl = lv_label_create(ui_InitScreen);
    lv_label_set_text(lbl, text);
    lv_obj_set_style_text_font(lbl, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(lbl, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(lbl, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(lbl, LV_OBJ_FLAG_FLOATING | LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_clear_flag(lbl, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_align(lbl, LV_ALIGN_BOTTOM_RIGHT, -6, -4);
}