// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.4
// LVGL version: 8.3.3
// Project name: SquareLine_Project

#ifndef _SQUARELINE_PROJECT_UI_H
#define _SQUARELINE_PROJECT_UI_H

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
void aniMainPower_Animation(lv_obj_t * TargetObject, int delay);
void fadeOnOff_Animation(lv_obj_t * TargetObject, int delay);
// SCREEN: ui_InitScreen
void ui_InitScreen_screen_init(void);
extern lv_obj_t * ui_InitScreen;
extern lv_obj_t * ui_Panel2;
extern lv_obj_t * ui_lblDate;
extern lv_obj_t * ui_lblTime;
extern lv_obj_t * ui_Panel1;
extern lv_obj_t * ui_lblCapacityDisplay;
extern lv_obj_t * ui_Label4;
extern lv_obj_t * ui_lblCapacity;
extern lv_obj_t * ui_lblBatteryCellNo;
extern lv_obj_t * ui_lblInputVol;
extern lv_obj_t * ui_lblOutputVol;
extern lv_obj_t * ui_Panel3;
void ui_event_Button1(lv_event_t * e);
extern lv_obj_t * ui_Button1;
extern lv_obj_t * ui_Label2;
// SCREEN: ui_MainScreen
void ui_MainScreen_screen_init(void);
extern lv_obj_t * ui_MainScreen;
extern lv_obj_t * ui_Container5;
extern lv_obj_t * ui_lblDateTime;
extern lv_obj_t * ui_Container4;
void ui_event_btnAlarm(lv_event_t * e);
extern lv_obj_t * ui_btnAlarm;
extern lv_obj_t * ui_Image1;
extern lv_obj_t * ui_pnlMainPower1;
extern lv_obj_t * ui_pnlMainPower2;
extern lv_obj_t * ui_pnlMainPower3;
extern lv_obj_t * ui_imgConvertor;
extern lv_obj_t * ui_pnlConvPower1;
extern lv_obj_t * ui_pnlConvPower2;
extern lv_obj_t * ui_pnlConvPower3;
extern lv_obj_t * ui_lbaBatCapacity;
extern lv_obj_t * ui_lblLoadCapacity;
extern lv_obj_t * ui_imgInvertor;
extern lv_obj_t * ui_imgBattery;
extern lv_obj_t * ui_imgScrBypass;
extern lv_obj_t * ui_imgScrOutput;
extern lv_obj_t * ui_imgOutputLine;
extern lv_obj_t * ui_imgOutputLineOn;
extern lv_obj_t * ui_pnlInvPower;
extern lv_obj_t * ui_Container1;
extern lv_obj_t * ui_Panel22;
void ui_event_Button7(lv_event_t * e);
extern lv_obj_t * ui_Button7;
extern lv_obj_t * ui_Label23;
extern lv_obj_t * ui_Panel24;
void ui_event_Button5(lv_event_t * e);
extern lv_obj_t * ui_Button5;
extern lv_obj_t * ui_Label25;
extern lv_obj_t * ui_Panel4;
void ui_event_Button8(lv_event_t * e);
extern lv_obj_t * ui_Button8;
extern lv_obj_t * ui_Label3;
extern lv_obj_t * ui_Panel23;
void ui_event_Button6(lv_event_t * e);
extern lv_obj_t * ui_Button6;
extern lv_obj_t * ui_Label24;
void ui_event_Panel16(lv_event_t * e);
extern lv_obj_t * ui_Panel16;
void ui_event_Button12(lv_event_t * e);
extern lv_obj_t * ui_Button12;
extern lv_obj_t * ui_Label7;
// SCREEN: ui_SettingScreen
void ui_SettingScreen_screen_init(void);
void ui_event_SettingScreen(lv_event_t * e);
extern lv_obj_t * ui_SettingScreen;
extern lv_obj_t * ui_TabView1;
void ui_event_TapPage1(lv_event_t * e);
extern lv_obj_t * ui_TapPage1;
extern lv_obj_t * ui_Container9;
extern lv_obj_t * ui_Panel18;
extern lv_obj_t * ui_Label18;
void ui_event_txtBatCurrSet(lv_event_t * e);
extern lv_obj_t * ui_txtBatCurrSet;
extern lv_obj_t * ui_Panel19;
extern lv_obj_t * ui_Label6;
void ui_event_txtBatVolSet(lv_event_t * e);
extern lv_obj_t * ui_txtBatVolSet;
extern lv_obj_t * ui_Panel35;
extern lv_obj_t * ui_Label38;
void ui_event_txtInvVolSet(lv_event_t * e);
extern lv_obj_t * ui_txtInvVolSet;
extern lv_obj_t * ui_Panel21;
extern lv_obj_t * ui_Label21;
extern lv_obj_t * ui_chkHFMode;
void ui_event_TabGainSETUP1(lv_event_t * e);
extern lv_obj_t * ui_TabGainSETUP1;
extern lv_obj_t * ui_Container11;
extern lv_obj_t * ui_Panel5;
extern lv_obj_t * ui_Label58;
void ui_event_txtInputVoltGain(lv_event_t * e);
extern lv_obj_t * ui_txtInputVoltGain;
extern lv_obj_t * ui_Panel25;
extern lv_obj_t * ui_Label59;
void ui_event_txtInputCurrGain(lv_event_t * e);
extern lv_obj_t * ui_txtInputCurrGain;
extern lv_obj_t * ui_Panel27;
extern lv_obj_t * ui_Label60;
void ui_event_txtInputVdcLinkGain(lv_event_t * e);
extern lv_obj_t * ui_txtInputVdcLinkGain;
extern lv_obj_t * ui_Panel28;
extern lv_obj_t * ui_Label61;
void ui_event_txtVbatVoltGain(lv_event_t * e);
extern lv_obj_t * ui_txtVbatVoltGain;
void ui_event_TabGainSETUP2(lv_event_t * e);
extern lv_obj_t * ui_TabGainSETUP2;
extern lv_obj_t * ui_Panel30;
extern lv_obj_t * ui_Label62;
void ui_event_txtBatCurrGain(lv_event_t * e);
extern lv_obj_t * ui_txtBatCurrGain;
extern lv_obj_t * ui_Panel31;
extern lv_obj_t * ui_Label63;
void ui_event_txtInvVoltGain(lv_event_t * e);
extern lv_obj_t * ui_txtInvVoltGain;
extern lv_obj_t * ui_Panel32;
extern lv_obj_t * ui_Label64;
void ui_event_txtInvCurrGain(lv_event_t * e);
extern lv_obj_t * ui_txtInvCurrGain;
extern lv_obj_t * ui_Panel33;
extern lv_obj_t * ui_Label65;
void ui_event_txtOutputCurrGain(lv_event_t * e);
extern lv_obj_t * ui_txtOutputCurrGain;
void ui_event_TabSystem(lv_event_t * e);
extern lv_obj_t * ui_TabSystem;
extern lv_obj_t * ui_Container7;
extern lv_obj_t * ui_Panel8;
extern lv_obj_t * ui_Label16;
void ui_event_Button2(lv_event_t * e);
extern lv_obj_t * ui_Button2;
extern lv_obj_t * ui_Label8;
extern lv_obj_t * ui_Panel9;
extern lv_obj_t * ui_Label9;
void ui_event_txtOfftime(lv_event_t * e);
extern lv_obj_t * ui_txtOfftime;
extern lv_obj_t * ui_Panel41;
extern lv_obj_t * ui_Label76;
void ui_event_txtBrigtness(lv_event_t * e);
extern lv_obj_t * ui_txtBrigtness;
extern lv_obj_t * ui_Panel10;
extern lv_obj_t * ui_Label10;
void ui_event_txtYear(lv_event_t * e);
extern lv_obj_t * ui_txtYear;
extern lv_obj_t * ui_Panel13;
extern lv_obj_t * ui_Label14;
void ui_event_txtMonth(lv_event_t * e);
extern lv_obj_t * ui_txtMonth;
extern lv_obj_t * ui_Panel11;
extern lv_obj_t * ui_Label11;
void ui_event_txtDay(lv_event_t * e);
extern lv_obj_t * ui_txtDay;
extern lv_obj_t * ui_Panel12;
extern lv_obj_t * ui_Label12;
void ui_event_txtHour(lv_event_t * e);
extern lv_obj_t * ui_txtHour;
extern lv_obj_t * ui_Panel14;
extern lv_obj_t * ui_Label13;
void ui_event_txtMinute(lv_event_t * e);
extern lv_obj_t * ui_txtMinute;
extern lv_obj_t * ui_Panel15;
extern lv_obj_t * ui_Label15;
void ui_event_txtSecond(lv_event_t * e);
extern lv_obj_t * ui_txtSecond;
extern lv_obj_t * ui_Panel17;
void ui_event_batSaveSetting(lv_event_t * e);
extern lv_obj_t * ui_batSaveSetting;
extern lv_obj_t * ui_Label17;
void ui_event_Button4(lv_event_t * e);
extern lv_obj_t * ui_Button4;
extern lv_obj_t * ui_Label22;
extern lv_obj_t * ui_pnlKeyBoard;
extern lv_obj_t * ui_txtInputArea;
void ui_event_Keyboard1(lv_event_t * e);
extern lv_obj_t * ui_Keyboard1;
void ui_event_lblMessage(lv_event_t * e);
extern lv_obj_t * ui_lblMessage;
// SCREEN: ui_scrControll
void ui_scrControll_screen_init(void);
extern lv_obj_t * ui_scrControll;
extern lv_obj_t * ui_Container2;
extern lv_obj_t * ui_Panel6;
extern lv_obj_t * ui_Label1;
void ui_event_btnRunUps(lv_event_t * e);
extern lv_obj_t * ui_btnRunUps;
extern lv_obj_t * ui_Label5;
extern lv_obj_t * ui_Panel34;
extern lv_obj_t * ui_Label66;
void ui_event_btnStopUps1(lv_event_t * e);
extern lv_obj_t * ui_btnStopUps1;
extern lv_obj_t * ui_Label68;
extern lv_obj_t * ui_Panel7;
extern lv_obj_t * ui_Label26;
extern lv_obj_t * ui_lblDcLinkVol;
extern lv_obj_t * ui_Label28;
extern lv_obj_t * ui_Label29;
extern lv_obj_t * ui_lblBatteryVol;
extern lv_obj_t * ui_Label31;
extern lv_obj_t * ui_Label32;
extern lv_obj_t * ui_lblInverterVol;
extern lv_obj_t * ui_Label34;
extern lv_obj_t * ui_Label35;
extern lv_obj_t * ui_lblOutputV;
extern lv_obj_t * ui_Label37;
void ui_event_Button14(lv_event_t * e);
extern lv_obj_t * ui_Button14;
extern lv_obj_t * ui_Label57;
// SCREEN: ui_scrMeasure
void ui_scrMeasure_screen_init(void);
void ui_event_scrMeasure(lv_event_t * e);
extern lv_obj_t * ui_scrMeasure;
extern lv_obj_t * ui_TabView3;
extern lv_obj_t * ui_TapPage2;
extern lv_obj_t * ui_Container6;
extern lv_obj_t * ui_Panel26;
extern lv_obj_t * ui_Label42;
extern lv_obj_t * ui_lblInputVoltRms;
extern lv_obj_t * ui_Panel29;
extern lv_obj_t * ui_Label44;
extern lv_obj_t * ui_lblInputCurrRms;
extern lv_obj_t * ui_Panel48;
extern lv_obj_t * ui_Label46;
extern lv_obj_t * ui_lblVdcLinkVoltRms;
extern lv_obj_t * ui_Panel49;
extern lv_obj_t * ui_Label49;
extern lv_obj_t * ui_lblBatVoltRms;
extern lv_obj_t * ui_Panel50;
extern lv_obj_t * ui_Label50;
extern lv_obj_t * ui_lblBatCurrentRms;
extern lv_obj_t * ui_Panel51;
extern lv_obj_t * ui_Label53;
extern lv_obj_t * ui_lblInvVoltRms;
extern lv_obj_t * ui_Panel52;
extern lv_obj_t * ui_Label54;
extern lv_obj_t * ui_lblInvCurrentRms;
extern lv_obj_t * ui_Panel53;
extern lv_obj_t * ui_Label92;
extern lv_obj_t * ui_lblOutputVoltRms;
extern lv_obj_t * ui_TapPage3;
extern lv_obj_t * ui_Container3;
extern lv_obj_t * ui_Panel44;
extern lv_obj_t * ui_Label83;
extern lv_obj_t * ui_lblOutputCurrRms;
extern lv_obj_t * ui_Panel45;
extern lv_obj_t * ui_Label85;
extern lv_obj_t * ui_lblInputFreq;
extern lv_obj_t * ui_Panel46;
extern lv_obj_t * ui_Label87;
extern lv_obj_t * ui_lblInvFreq;
extern lv_obj_t * ui_Panel47;
extern lv_obj_t * ui_Label89;
extern lv_obj_t * ui_lblBypassFreq;
extern lv_obj_t * ui_Panel60;
extern lv_obj_t * ui_Label91;
extern lv_obj_t * ui_lblBattCapacity;
extern lv_obj_t * ui_Panel62;
extern lv_obj_t * ui_Label128;
extern lv_obj_t * ui_lblUpsInnerTemp;
extern lv_obj_t * ui_TabPage5;
extern lv_obj_t * ui_Container8;
extern lv_obj_t * ui_Panel68;
extern lv_obj_t * ui_Label102;
extern lv_obj_t * ui_lblInputVoltGain;
extern lv_obj_t * ui_Panel69;
extern lv_obj_t * ui_Label104;
extern lv_obj_t * ui_lblInputCurrGain;
extern lv_obj_t * ui_Panel70;
extern lv_obj_t * ui_Label106;
extern lv_obj_t * ui_lblVdcLinkVoltGain;
extern lv_obj_t * ui_Panel71;
extern lv_obj_t * ui_Label108;
extern lv_obj_t * ui_lblVbatVoltGain;
extern lv_obj_t * ui_Panel72;
extern lv_obj_t * ui_Label110;
extern lv_obj_t * ui_lblBatCurrentGain;
extern lv_obj_t * ui_Panel73;
extern lv_obj_t * ui_Label112;
extern lv_obj_t * ui_lblInverterVoltGain;
extern lv_obj_t * ui_Panel74;
extern lv_obj_t * ui_Label114;
extern lv_obj_t * ui_lblInvCurrGain;
extern lv_obj_t * ui_Panel75;
extern lv_obj_t * ui_Label116;
extern lv_obj_t * ui_lblOutputCurrGain;
void ui_event_Button10(lv_event_t * e);
extern lv_obj_t * ui_Button10;
extern lv_obj_t * ui_Label123;
// SCREEN: ui_scrAlarm1
void ui_scrAlarm1_screen_init(void);
void ui_event_scrAlarm1(lv_event_t * e);
extern lv_obj_t * ui_scrAlarm1;
extern lv_obj_t * ui_TabView2;
extern lv_obj_t * ui_TabPage1;
extern lv_obj_t * ui_Container10;
extern lv_obj_t * ui_eventTextArea;
extern lv_obj_t * ui_TabPage6;
extern lv_obj_t * ui_alarmTextArea;
extern lv_obj_t * ui_Panel61;
void ui_event_Button13(lv_event_t * e);
extern lv_obj_t * ui_Button13;
extern lv_obj_t * ui_Label127;
void ui_event_btnAlarmPrev2(lv_event_t * e);
extern lv_obj_t * ui_btnAlarmPrev2;
extern lv_obj_t * ui_Label122;
void ui_event_btnAlarmNext2(lv_event_t * e);
extern lv_obj_t * ui_btnAlarmNext2;
extern lv_obj_t * ui_Label124;
void ui_event_Button9(lv_event_t * e);
extern lv_obj_t * ui_Button9;
extern lv_obj_t * ui_Label125;
extern lv_obj_t * ui____initial_actions0;

LV_IMG_DECLARE(ui_img_alarm40_40_png);    // assets\alarm40_40.png
LV_IMG_DECLARE(ui_img_flowmain1_png);    // assets\FlowMain1.png
LV_IMG_DECLARE(ui_img_inverteracdc_png);    // assets\InverTerACDC.png
LV_IMG_DECLARE(ui_img_batterybank_png);    // assets\batteryBank.png
LV_IMG_DECLARE(ui_img_scr_png);    // assets\scr.png
LV_IMG_DECLARE(ui_img_outputpowerline_png);    // assets\outputPowerLine.png
LV_IMG_DECLARE(ui_img_outputpowerlineon_png);    // assets\outputPowerLineOn.png
LV_IMG_DECLARE(ui_img_alarm1_png);    // assets\alarm1.png
LV_IMG_DECLARE(ui_img_alarm25_25_png);    // assets\alarm25_25.png
LV_IMG_DECLARE(ui_img_alarm30_30_png);    // assets\alarm30_30.png
LV_IMG_DECLARE(ui_img_convpwrline_1_png);    // assets\convPwrLine_1.png
LV_IMG_DECLARE(ui_img_convpwrline_2_png);    // assets\convPwrLine_2.png

LV_FONT_DECLARE(ui_font_digitalWatch20);
LV_FONT_DECLARE(ui_font_digitalWatch26);
LV_FONT_DECLARE(ui_font_digitalWatch32);
LV_FONT_DECLARE(ui_font_digitalWatch56);
LV_FONT_DECLARE(ui_font_malgun14);
LV_FONT_DECLARE(ui_font_malgun20);
LV_FONT_DECLARE(ui_font_malgun26);

void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
