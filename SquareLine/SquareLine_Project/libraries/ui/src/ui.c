// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.3
// Project name: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
void aniMainPower_Animation(lv_obj_t * TargetObject, int delay);
void fadeOnOff_Animation(lv_obj_t * TargetObject, int delay);


// SCREEN: ui_InitScreen
void ui_InitScreen_screen_init(void);
lv_obj_t * ui_InitScreen;
lv_obj_t * ui_Panel2;
lv_obj_t * ui_lblDate;
lv_obj_t * ui_lblTime;
lv_obj_t * ui_Panel1;
lv_obj_t * ui_lblCapacityDisplay;
lv_obj_t * ui_Label4;
lv_obj_t * ui_lblCapacity;
lv_obj_t * ui_lblBatteryCellNo;
lv_obj_t * ui_lblInputVol;
lv_obj_t * ui_lblOutputVol;
lv_obj_t * ui_Panel3;
void ui_event_Button1(lv_event_t * e);
lv_obj_t * ui_Button1;
lv_obj_t * ui_Label2;


// SCREEN: ui_MainScreen
void ui_MainScreen_screen_init(void);
lv_obj_t * ui_MainScreen;
lv_obj_t * ui_Container5;
lv_obj_t * ui_lblDateTime;
lv_obj_t * ui_Container4;
void ui_event_btnAlarm(lv_event_t * e);
lv_obj_t * ui_btnAlarm;
lv_obj_t * ui_Image1;
lv_obj_t * ui_pnlMainPower1;
lv_obj_t * ui_pnlMainPower2;
lv_obj_t * ui_pnlMainPower3;
lv_obj_t * ui_imgConvertor;
lv_obj_t * ui_pnlConvPower1;
lv_obj_t * ui_pnlConvPower2;
lv_obj_t * ui_pnlConvPower3;
lv_obj_t * ui_lbaBatCapacity;
lv_obj_t * ui_lblLoadCapacity;
lv_obj_t * ui_imgInvertor;
lv_obj_t * ui_imgBattery;
lv_obj_t * ui_imgScrBypass;
lv_obj_t * ui_imgScrOutput;
lv_obj_t * ui_imgOutputLine;
lv_obj_t * ui_imgOutputLineOn;
lv_obj_t * ui_pnlInvPower;
void ui_event_Panel16(lv_event_t * e);
lv_obj_t * ui_Panel16;
void ui_event_Button12(lv_event_t * e);
lv_obj_t * ui_Button12;
lv_obj_t * ui_Label7;
lv_obj_t * ui_Container1;
lv_obj_t * ui_Panel22;
void ui_event_Button7(lv_event_t * e);
lv_obj_t * ui_Button7;
lv_obj_t * ui_Label23;
lv_obj_t * ui_Panel23;
void ui_event_Button6(lv_event_t * e);
lv_obj_t * ui_Button6;
lv_obj_t * ui_Label24;
lv_obj_t * ui_Panel24;
void ui_event_Button5(lv_event_t * e);
lv_obj_t * ui_Button5;
lv_obj_t * ui_Label25;
lv_obj_t * ui_Panel4;
void ui_event_Button8(lv_event_t * e);
lv_obj_t * ui_Button8;
lv_obj_t * ui_Label3;


// SCREEN: ui_SettingScreen
void ui_SettingScreen_screen_init(void);
lv_obj_t * ui_SettingScreen;
lv_obj_t * ui_TabView1;
void ui_event_TapPage1(lv_event_t * e);
lv_obj_t * ui_TapPage1;
lv_obj_t * ui_Container9;
lv_obj_t * ui_Panel19;
lv_obj_t * ui_Label6;
void ui_event_txtBatVolSet(lv_event_t * e);
lv_obj_t * ui_txtBatVolSet;
lv_obj_t * ui_Panel18;
lv_obj_t * ui_Label18;
void ui_event_txtBatCurrSet(lv_event_t * e);
lv_obj_t * ui_txtBatCurrSet;
lv_obj_t * ui_Panel35;
lv_obj_t * ui_Label38;
void ui_event_txtOutputVolSet(lv_event_t * e);
lv_obj_t * ui_txtOutputVolSet;
lv_obj_t * ui_Panel21;
lv_obj_t * ui_Label21;
void ui_event_chkHFMode(lv_event_t * e);
lv_obj_t * ui_chkHFMode;
lv_obj_t * ui_Panel17;
void ui_event_Label83(lv_event_t * e);
lv_obj_t * ui_Label83;
void ui_event_batSaveSetting(lv_event_t * e);
lv_obj_t * ui_batSaveSetting;
lv_obj_t * ui_Label17;
void ui_event_Button4(lv_event_t * e);
lv_obj_t * ui_Button4;
lv_obj_t * ui_Label22;
void ui_event_TabGainSETUP1(lv_event_t * e);
lv_obj_t * ui_TabGainSETUP1;
lv_obj_t * ui_Container11;
lv_obj_t * ui_Label76;
lv_obj_t * ui_Panel5;
lv_obj_t * ui_txtInputVoltGainLBL;
void ui_event_txtInputVoltGain(lv_event_t * e);
lv_obj_t * ui_txtInputVoltGain;
lv_obj_t * ui_Panel25;
lv_obj_t * ui_txtInputCurrGainLBL;
void ui_event_txtInputCurrGain(lv_event_t * e);
lv_obj_t * ui_txtInputCurrGain;
lv_obj_t * ui_Label9;
lv_obj_t * ui_Panel27;
lv_obj_t * ui_txtInvVoltGainLBL;
void ui_event_txtInvVoltGain(lv_event_t * e);
lv_obj_t * ui_txtInvVoltGain;
lv_obj_t * ui_Panel28;
lv_obj_t * ui_txtInvCurrGainLBL;
void ui_event_txtInvCurrGain(lv_event_t * e);
lv_obj_t * ui_txtInvCurrGain;
lv_obj_t * ui_Container12;
lv_obj_t * ui_Panel10;
void ui_event_Button10(lv_event_t * e);
lv_obj_t * ui_Button10;
lv_obj_t * ui_Label19;
lv_obj_t * ui_Panel11;
void ui_event_Button11(lv_event_t * e);
lv_obj_t * ui_Button11;
lv_obj_t * ui_Label20;
lv_obj_t * ui_Panel12;
void ui_event_Button15(lv_event_t * e);
lv_obj_t * ui_Button15;
lv_obj_t * ui_Label27;
lv_obj_t * ui_Panel13;
void ui_event_Button16(lv_event_t * e);
lv_obj_t * ui_Button16;
lv_obj_t * ui_Label30;
void ui_event_TabGainSETUP2(lv_event_t * e);
lv_obj_t * ui_TabGainSETUP2;
lv_obj_t * ui_Container13;
lv_obj_t * ui_Label33;
lv_obj_t * ui_Panel14;
lv_obj_t * ui_txtVbatVoltGainLBL;
void ui_event_txtVbatVoltGain(lv_event_t * e);
lv_obj_t * ui_txtVbatVoltGain;
lv_obj_t * ui_Panel15;
lv_obj_t * ui_txtBatCurrGainLBL;
void ui_event_txtBatCurrGain(lv_event_t * e);
lv_obj_t * ui_txtBatCurrGain;
lv_obj_t * ui_Label40;
lv_obj_t * ui_Label67;
lv_obj_t * ui_Panel20;
lv_obj_t * ui_txtOutputCurrGainLBL;
void ui_event_txtOutputCurrGain(lv_event_t * e);
lv_obj_t * ui_txtOutputCurrGain;
lv_obj_t * ui_Panel36;
lv_obj_t * ui_txtInputVdcLinkGainLBL;
void ui_event_txtInputVdcLinkGain(lv_event_t * e);
lv_obj_t * ui_txtInputVdcLinkGain;
lv_obj_t * ui_Container14;
lv_obj_t * ui_Panel37;
void ui_event_Button17(lv_event_t * e);
lv_obj_t * ui_Button17;
lv_obj_t * ui_Label45;
lv_obj_t * ui_Panel38;
void ui_event_Button18(lv_event_t * e);
lv_obj_t * ui_Button18;
lv_obj_t * ui_Label47;
lv_obj_t * ui_Panel39;
void ui_event_Button19(lv_event_t * e);
lv_obj_t * ui_Button19;
lv_obj_t * ui_Label48;
lv_obj_t * ui_Panel40;
void ui_event_Button20(lv_event_t * e);
lv_obj_t * ui_Button20;
lv_obj_t * ui_Label51;
void ui_event_TabOffsetSETUP3(lv_event_t * e);
lv_obj_t * ui_TabOffsetSETUP3;
lv_obj_t * ui_Container15;
lv_obj_t * ui_Panel30;
lv_obj_t * ui_Label62;
void ui_event_txtInputVoltOffset(lv_event_t * e);
lv_obj_t * ui_txtInputVoltOffset;
void ui_event_txtInputCurrentOffset(lv_event_t * e);
lv_obj_t * ui_txtInputCurrentOffset;
lv_obj_t * ui_Panel31;
lv_obj_t * ui_Label36;
void ui_event_txtVdcLinkVoltOffset(lv_event_t * e);
lv_obj_t * ui_txtVdcLinkVoltOffset;
void ui_event_txtInputResrved(lv_event_t * e);
lv_obj_t * ui_txtInputResrved;
lv_obj_t * ui_Panel32;
lv_obj_t * ui_Label39;
void ui_event_txtBatVoltOffset(lv_event_t * e);
lv_obj_t * ui_txtBatVoltOffset;
void ui_event_txtBatCurrentOffset(lv_event_t * e);
lv_obj_t * ui_txtBatCurrentOffset;
lv_obj_t * ui_Panel33;
lv_obj_t * ui_Label41;
void ui_event_txtInvVoltOffset(lv_event_t * e);
lv_obj_t * ui_txtInvVoltOffset;
void ui_event_txtInvCurrentOffset(lv_event_t * e);
lv_obj_t * ui_txtInvCurrentOffset;
lv_obj_t * ui_Panel56;
lv_obj_t * ui_Label43;
void ui_event_txtInputVdcLinkGain9(lv_event_t * e);
lv_obj_t * ui_txtInputVdcLinkGain9;
void ui_event_txtOutCurrentOffset(lv_event_t * e);
lv_obj_t * ui_txtOutCurrentOffset;
lv_obj_t * ui_Container16;
lv_obj_t * ui_Panel42;
void ui_event_Button21(lv_event_t * e);
lv_obj_t * ui_Button21;
lv_obj_t * ui_Label52;
lv_obj_t * ui_Panel43;
lv_obj_t * ui_Button22;
lv_obj_t * ui_Label54;
lv_obj_t * ui_Panel54;
lv_obj_t * ui_Button23;
lv_obj_t * ui_Label55;
lv_obj_t * ui_Panel55;
void ui_event_Button24(lv_event_t * e);
lv_obj_t * ui_Button24;
lv_obj_t * ui_Label56;
void ui_event_TabSystem(lv_event_t * e);
lv_obj_t * ui_TabSystem;
lv_obj_t * ui_Container7;
lv_obj_t * ui_pnlYear;
lv_obj_t * ui_Label10;
void ui_event_txtYear(lv_event_t * e);
lv_obj_t * ui_txtYear;
lv_obj_t * ui_pnlMonth;
lv_obj_t * ui_Label14;
void ui_event_txtMonth(lv_event_t * e);
lv_obj_t * ui_txtMonth;
lv_obj_t * ui_pnlDay;
lv_obj_t * ui_Label11;
void ui_event_txtDay(lv_event_t * e);
lv_obj_t * ui_txtDay;
lv_obj_t * ui_pnlHour;
lv_obj_t * ui_Label12;
void ui_event_txtHour(lv_event_t * e);
lv_obj_t * ui_txtHour;
lv_obj_t * ui_pnlMinute;
lv_obj_t * ui_Label13;
void ui_event_txtMinute(lv_event_t * e);
lv_obj_t * ui_txtMinute;
lv_obj_t * ui_pnlSecond;
lv_obj_t * ui_Label15;
void ui_event_txtSecond(lv_event_t * e);
lv_obj_t * ui_txtSecond;
lv_obj_t * ui_Panel8;
lv_obj_t * ui_Label16;
void ui_event_Button2(lv_event_t * e);
lv_obj_t * ui_Button2;
lv_obj_t * ui_Label8;
void ui_event_Button25(lv_event_t * e);
lv_obj_t * ui_Button25;
lv_obj_t * ui_Label58;
lv_obj_t * ui_Panel9;
void ui_event_txtOfftime(lv_event_t * e);
lv_obj_t * ui_txtOfftime;
lv_obj_t * ui_Panel41;
void ui_event_txtBrigtness(lv_event_t * e);
lv_obj_t * ui_txtBrigtness;
lv_obj_t * ui_pnlKeyBoard;
lv_obj_t * ui_txtInputArea;
void ui_event_Keyboard1(lv_event_t * e);
lv_obj_t * ui_Keyboard1;
void ui_event_lblMessage(lv_event_t * e);
lv_obj_t * ui_lblMessage;


// SCREEN: ui_scrControll
void ui_scrControll_screen_init(void);
lv_obj_t * ui_scrControll;
lv_obj_t * ui_Container2;
lv_obj_t * ui_Panel6;
lv_obj_t * ui_Label1;
void ui_event_btnRunUps(lv_event_t * e);
lv_obj_t * ui_btnRunUps;
lv_obj_t * ui_Label5;
lv_obj_t * ui_Panel34;
lv_obj_t * ui_Label66;
void ui_event_btnStopUps1(lv_event_t * e);
lv_obj_t * ui_btnStopUps1;
lv_obj_t * ui_Label68;
lv_obj_t * ui_Panel7;
lv_obj_t * ui_Label26;
lv_obj_t * ui_lblDcLinkVol;
lv_obj_t * ui_Label28;
lv_obj_t * ui_Label29;
lv_obj_t * ui_lblBatteryVol;
lv_obj_t * ui_Label31;
lv_obj_t * ui_Label32;
lv_obj_t * ui_lblInverterVol;
lv_obj_t * ui_Label34;
lv_obj_t * ui_Label35;
lv_obj_t * ui_lblOutputV;
lv_obj_t * ui_Label37;
void ui_event_Button14(lv_event_t * e);
lv_obj_t * ui_Button14;
lv_obj_t * ui_Label57;


// SCREEN: ui_scrMeasure
void ui_scrMeasure_screen_init(void);
void ui_event_scrMeasure(lv_event_t * e);
lv_obj_t * ui_scrMeasure;
lv_obj_t * ui_TabView3;
lv_obj_t * ui_TapPage2;
lv_obj_t * ui_Container6;
lv_obj_t * ui_Panel26;
lv_obj_t * ui_Label42;
lv_obj_t * ui_Panel29;
lv_obj_t * ui_lblInputVoltRms;
lv_obj_t * ui_lblInputCurrRms;
lv_obj_t * ui_lblInputFreq;
lv_obj_t * ui_Panel53;
lv_obj_t * ui_Label92;
lv_obj_t * ui_Panel52;
lv_obj_t * ui_lblInvVoltRms;
lv_obj_t * ui_lblInvCurrentRms;
lv_obj_t * ui_lblInvFreq;
lv_obj_t * ui_Panel51;
lv_obj_t * ui_Label53;
lv_obj_t * ui_Panel44;
lv_obj_t * ui_lblOutputVoltRms;
lv_obj_t * ui_lblOutputCurrRms;
lv_obj_t * ui_lblOutputFreq;
lv_obj_t * ui_Panel48;
lv_obj_t * ui_Label46;
lv_obj_t * ui_Panel50;
lv_obj_t * ui_lblBatVoltRms;
lv_obj_t * ui_lblBatCurrentRms;
lv_obj_t * ui_TapPage3;
lv_obj_t * ui_Container3;
lv_obj_t * ui_Panel45;
lv_obj_t * ui_Label85;
lv_obj_t * ui_Panel46;
lv_obj_t * ui_Label87;
lv_obj_t * ui_Panel47;
lv_obj_t * ui_Label89;
lv_obj_t * ui_lblBypassFreq;
lv_obj_t * ui_Panel60;
lv_obj_t * ui_Label91;
lv_obj_t * ui_lblBattCapacity;
lv_obj_t * ui_Panel62;
lv_obj_t * ui_Label128;
lv_obj_t * ui_lblUpsInnerTemp;
lv_obj_t * ui_TabPage5;
lv_obj_t * ui_Container8;
lv_obj_t * ui_Panel68;
lv_obj_t * ui_Label102;
lv_obj_t * ui_lblInputVoltGain;
lv_obj_t * ui_Panel69;
lv_obj_t * ui_Label104;
lv_obj_t * ui_lblInputCurrGain;
lv_obj_t * ui_Panel70;
lv_obj_t * ui_Label106;
lv_obj_t * ui_lblVdcLinkVoltGain;
lv_obj_t * ui_Panel71;
lv_obj_t * ui_Label108;
lv_obj_t * ui_lblVbatVoltGain;
lv_obj_t * ui_Panel72;
lv_obj_t * ui_Label110;
lv_obj_t * ui_lblBatCurrentGain;
lv_obj_t * ui_Panel73;
lv_obj_t * ui_Label112;
lv_obj_t * ui_lblInverterVoltGain;
lv_obj_t * ui_Panel74;
lv_obj_t * ui_Label114;
lv_obj_t * ui_lblInvCurrGain;
lv_obj_t * ui_Panel75;
lv_obj_t * ui_Label116;
lv_obj_t * ui_lblOutputCurrGain;
void ui_event_Button3(lv_event_t * e);
lv_obj_t * ui_Button3;
lv_obj_t * ui_Label123;
lv_obj_t * ui_TabPage4;
lv_obj_t * ui_Panel49;
lv_obj_t * ui_Label49;
lv_obj_t * ui_Label50;
lv_obj_t * ui_Label44;
lv_obj_t * ui_lblVdcLinkVoltRms;


// SCREEN: ui_scrAlarm1
void ui_scrAlarm1_screen_init(void);
void ui_event_scrAlarm1(lv_event_t * e);
lv_obj_t * ui_scrAlarm1;
lv_obj_t * ui_TabView2;
lv_obj_t * ui_TabAlarmPage;
lv_obj_t * ui_alarmTextArea;
lv_obj_t * ui_TabEventHistoryPage1;
lv_obj_t * ui_eventTextArea;
lv_obj_t * ui_Panel61;
void ui_event_Button13(lv_event_t * e);
lv_obj_t * ui_Button13;
lv_obj_t * ui_Label127;
void ui_event_btnAlarmPrev2(lv_event_t * e);
lv_obj_t * ui_btnAlarmPrev2;
lv_obj_t * ui_Label122;
void ui_event_btnAlarmNext2(lv_event_t * e);
lv_obj_t * ui_btnAlarmNext2;
lv_obj_t * ui_Label124;
void ui_event_Button9(lv_event_t * e);
lv_obj_t * ui_Button9;
lv_obj_t * ui_Label125;
lv_obj_t * ui____initial_actions0;
const lv_img_dsc_t * ui_imgset_alarm[1] = {&ui_img_alarm1_png};
const lv_img_dsc_t * ui_imgset_alarm_[3] = {&ui_img_alarm25_25_png, &ui_img_alarm30_30_png, &ui_img_alarm40_40_png};
const lv_img_dsc_t * ui_imgset_convpwrline_[2] = {&ui_img_convpwrline_1_png, &ui_img_convpwrline_2_png};
const lv_img_dsc_t * ui_imgset_flowmain[1] = {&ui_img_flowmain1_png};

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////
void aniMainPower_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in_out);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity);
    lv_anim_start(&PropertyAnimation_0);

}
void fadeOnOff_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 2000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 150, 255);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 500);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 1000);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 500);
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 100);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity);
    lv_anim_start(&PropertyAnimation_0);

}

///////////////////// FUNCTIONS ////////////////////
void ui_event_Button1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_MainScreen_screen_init);
    }
}
void ui_event_btnAlarm(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        btnAlarmRunStop(e);
    }
}
void ui_event_Panel16(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        _ui_screen_change(&ui_InitScreen, LV_SCR_LOAD_ANIM_NONE, 500, 0, &ui_InitScreen_screen_init);
    }
}
void ui_event_Button12(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_InitScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_InitScreen_screen_init);
    }
}
void ui_event_Button7(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_scrControll, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_scrControll_screen_init);
    }
}
void ui_event_Button6(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_scrMeasure, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_scrMeasure_screen_init);
    }
}
void ui_event_Button5(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        scrSettingScreenLoaded(e);
    }
}
void ui_event_Button8(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_scrAlarm1, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_scrAlarm1_screen_init);
    }
}
void ui_event_TapPage1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_RELEASED) {
        evtTabUpsSetup(e);
    }
}
void ui_event_txtBatVolSet(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtBatVolSet(e);
    }
}
void ui_event_txtBatCurrSet(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtBatCurrset(e);
    }
}
void ui_event_txtOutputVolSet(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtOutputVolSet(e);
    }
}
void ui_event_chkHFMode(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        hfModeValueChangedEvent(e);
    }
}
void ui_event_Label83(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TabEventSetupVolClick(e);
    }
}
void ui_event_batSaveSetting(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TabEventSetupTimeClick(e);
    }
}
void ui_event_Button4(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_RELEASED) {
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_MainScreen_screen_init);
    }
}
void ui_event_TabGainSETUP1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        evtTabGainSetup1(e);
    }
}
void ui_event_txtInputVoltGain(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVoltGain(e);
    }
}
void ui_event_txtInputCurrGain(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputCurrGain(e);
    }
}
void ui_event_txtInvVoltGain(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInvVoltGain(e);
    }
}
void ui_event_txtInvCurrGain(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInvCurrGain(e);
    }
}
void ui_event_Button10(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_MainScreen_screen_init);
    }
}
void ui_event_Button11(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TabEvtNextClick(e);
    }
}
void ui_event_Button15(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TabEvtOffsetClick(e);
    }
}
void ui_event_Button16(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TabEvtESCClick(e);
    }
}
void ui_event_TabGainSETUP2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        evtTabGainSetup1(e);
    }
}
void ui_event_txtVbatVoltGain(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtVbatVoltGain(e);
    }
}
void ui_event_txtBatCurrGain(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtBatCurrGain(e);
    }
}
void ui_event_txtOutputCurrGain(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtOutputCurrGain(e);
    }
}
void ui_event_txtInputVdcLinkGain(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_Button17(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_MainScreen_screen_init);
    }
}
void ui_event_Button18(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TabEvtPrevClick(e);
    }
}
void ui_event_Button19(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TabEvtOffsetClick(e);
    }
}
void ui_event_Button20(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TabEvtESCClick(e);
    }
}
void ui_event_TabOffsetSETUP3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_RELEASED) {
        evtTabGainSetup2(e);
    }
}
void ui_event_txtInputVoltOffset(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_txtInputCurrentOffset(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_txtVdcLinkVoltOffset(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_txtInputResrved(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_txtBatVoltOffset(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_txtBatCurrentOffset(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_txtInvVoltOffset(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_txtInvCurrentOffset(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_txtInputVdcLinkGain9(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_txtOutCurrentOffset(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtInputVdcLinkGain(e);
    }
}
void ui_event_Button21(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_MainScreen_screen_init);
    }
}
void ui_event_Button24(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TabEvtESCClick(e);
    }
}
void ui_event_TabSystem(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_PRESSED) {
        evtSystemTabClicked(e);
    }
}
void ui_event_txtYear(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtYear(e);
    }
}
void ui_event_txtMonth(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtMonth(e);
    }
}
void ui_event_txtDay(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtDay(e);
    }
}
void ui_event_txtHour(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtHour(e);
    }
}
void ui_event_txtMinute(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtMinute(e);
    }
}
void ui_event_txtSecond(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtSecond(e);
    }
}
void ui_event_Button2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        ChangeLanguage(e);
    }
}
void ui_event_Button25(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        TabEvtESCClick(e);
    }
}
void ui_event_txtOfftime(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtOfftime(e);
    }
}
void ui_event_txtBrigtness(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventTxtBrigtness(e);
    }
}
void ui_event_Keyboard1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_VALUE_CHANGED) {
        keyBoardValueChangedEvent(e);
    }
}
void ui_event_lblMessage(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        evtMessageLblClick(e);
    }
}
void ui_event_btnRunUps(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        btnEventRunUps(e);
    }
}
void ui_event_btnStopUps1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        btnEventStopUps(e);
    }
}
void ui_event_Button14(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_RELEASED) {
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_MainScreen_screen_init);
    }
}
void ui_event_scrMeasure(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        scrMeasureLoadEvent(e);
    }
}
void ui_event_Button3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_RELEASED) {
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_MainScreen_screen_init);
    }
}
void ui_event_scrAlarm1(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_SCREEN_LOADED) {
        evtLogScreenLoaded(e);
    }
}
void ui_event_Button13(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        btnAlarmRunStopAtLog(e);
    }
}
void ui_event_btnAlarmPrev2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventLogPrev(e);
    }
}
void ui_event_btnAlarmNext2(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_CLICKED) {
        EventLogNext(e);
    }
}
void ui_event_Button9(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if(event_code == LV_EVENT_RELEASED) {
        _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_MainScreen_screen_init);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_InitScreen_screen_init();
    ui_MainScreen_screen_init();
    ui_SettingScreen_screen_init();
    ui_scrControll_screen_init();
    ui_scrMeasure_screen_init();
    ui_scrAlarm1_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_disp_load_scr(ui_InitScreen);
}
