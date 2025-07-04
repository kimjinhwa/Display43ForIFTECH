#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <TFT_eSPI.h>
#include <RtcDS1302.h>
#include "mainGrobal.h"
#include "myBlueTooth.h"
#include "fileSystem.h"
#include "../Version.h"
#include "esp_task_wdt.h"
//#include "font/FreeSansBold12pt7b.h"
// #include "fileSystem.h"

#include <modbusRtu.h>
// #include "SimpleBLE.h"
#include "ui.h"
#include "myui.h"
#include "upsLog.h"

#include <EEPROM.h>
// #include "SerialProtocalParse.h"
#include "main.h"
#include "wifiOTA.h"
#include "lv_i18n.h"
#define WDT_TIMEOUT 60 
#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define TFT_BL 2

#define BUTTON_ERASE 0 

// #define SERIAL_RX2 18
// #define SERIAL_TX2 17
// #define RTCEN 19
// #define BUZZER 20
/* Change Port */
#define SERIAL_RX2 20 
#define SERIAL_TX2 19 
#define RTCEN 17
#define BUZZER 18 


#define OFFSCR_COLOR 0xFFFFFF   /*DARK BLUE*/ 
#define ONSCR_COLOR 0xFF0000   /* YELLOW*/
#define OFFCONV_FORE_COLOR 0xFFFFFF  /* WHITE*/
//#define WHITE 0xFFFFFF  /* WHITE*/
#define ONCONV_BG_COLOR 0xFF00000 /* RED */
#define ONLINE_COLOR 0x379B57 //
#define OFFLINE_COLOR 0xD2EBF6// 
//#define ONLINE_COLOR 0xFF00000 /* RED */
//#define OFFLINE_COLOR 0xFFFFFF  // WHITE
// #define BRIGHT 80
// SimpleBLE mySerialBT;
TaskHandle_t *h_pxsystemControllTask;

ThreeWire myWire(MOSI /*11*/, SCK /*12*/, RTCEN /*19*/); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
// LittleFileSystem lsFile;

nvsSystemSet_t nvsSystemEEPRom;

static uint32_t screenWidth;
static uint32_t screenHeight;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;
static unsigned long last_ms;
// static lv_obj_t *led;
uint16_t lcdOntime = 0;
int16_t minDisMessageTime = 1;
// #define DISPLAY_7
static char TAG[] = "main";

extern LittleFileSystem lsFile;
extern jobCommant_t systemControllJob;

upsLog upslogEvent("/spiffs/eventLog.hex", EVENT_TYPE);
upsLog upslogAlarm(FAULT_TYPE); //

void mainScrUpdata();
void GetSetEventData();
void systemControllTask(void *parameter);
void scrSettingScreen();
void scrMeasureLoad();
void toggleBuzzer();
void RebootSystem(uint16_t afterTime);
#define DISPLAY_43

#ifdef DISPLAY_7
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    41 /* DE */, 40 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    14 /* R0 */, 21 /* R1 */, 47 /* R2 */, 48 /* R3 */, 45 /* R4 */,
    9 /* G0 */, 46 /* G1 */, 3 /* G2 */, 8 /* G3 */, 16 /* G4 */, 1 /* G5 */,
    15 /* B0 */, 7 /* B1 */, 6 /* B2 */, 5 /* B3 */, 4 /* B4 */
);
// option 1:
// 7寸 50PIN 800*480
Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
    bus,
    //  800 /* width */, 0 /* hsync_polarity */, 8/* hsync_front_porch */, 2 /* hsync_pulse_width */, 43/* hsync_back_porch */,
    //  480 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 2/* vsync_pulse_width */, 12 /* vsync_back_porch */,
    //  1 /* pclk_active_neg */, 16000000 /* prefer_speed */, true /* auto_flush */);

    800 /* width */, 0 /* hsync_polarity */, 210 /* hsync_front_porch */, 30 /* hsync_pulse_width */, 16 /* hsync_back_porch */,
    480 /* height */, 0 /* vsync_polarity */, 22 /* vsync_front_porch */, 13 /* vsync_pulse_width */, 10 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 12000000 /* prefer_speed */, true /* auto_flush */);
#endif
#ifdef DISPLAY_43
Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */
);
// option 1:
// ILI6485 LCD 480x272
Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
    bus,
    480 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
    272 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 9000000 /* prefer_speed */, true /* auto_flush */);

#endif

#include "touch.h"
#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf(buf);
  Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
  lv_disp_flush_ready(disp);
}

extern XPT2046_Touchscreen ts;
extern int modbusErrorCounter;
void touchTest(int loopCount)
{
  uint16_t x, y;
  uint8_t z;
  TS_Point p;
  do
  {
    if (ts.touched())
    {
      ts.readData(&x, &y, &z);
      TS_Point p = ts.getPoint();
      Serial.printf("\n%d,%d,%d", x, y, z);
      Serial.printf("\np.x %d,p.y %d", p.x, p.y);
      touch_last_x = map(p.x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, gfx->width() - 1);
      touch_last_y = map(p.y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, gfx->height() - 1);
      Serial.printf("\np.x %d,p.y %d", touch_last_x, touch_last_y);
    }
  } while (loopCount);
}
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t x, y;
  uint8_t z;
  if (touch_has_signal())
  {
    if (ts.touched())
    {
      //ESP_LOGI("TOUCH", "Touch wait ");
      vTaskDelay(11); //10   15 23 33  노이즈를 방지하기 위하여 한번 더 읽는다.
      if (ts.touched())
      {
        data->state = LV_INDEV_STATE_PR;
        //     /*Set the coordinates*/
        // ts.readData(&x, &y, &z);
        TS_Point p = ts.getPoint();
        touch_last_x = map(p.x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, gfx->width() - 1);
        touch_last_y = map(p.y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, gfx->height() - 1);
        data->point.x = touch_last_x;
        data->point.y = touch_last_y;
        long brightness = map(nvsSystemEEPRom.lcdBright, 0, 255, 0, 255);
        ledcWrite(0, brightness < 80 ? 80 : brightness);
        lcdOntime = 0;
        //ESP_LOGI("TOUCH", "Data (x,y,z)(%d,%d,%d)", data->point.x, data->point.y, p.z);
      }
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_REL;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_REL;
  }
};

void setMemoryDataToLCD()
{
}
// void setTime(){
//   tm nowTime;
//   timeval tVal;
//   nowTime.tm_year = 124;
//   nowTime.tm_mon = 2;
//   nowTime.tm_mday = 7;
//   nowTime.tm_hour = 17;
//   nowTime.tm_min = 6;
//   nowTime.tm_sec = 6;

//   time_t now = mktime(&nowTime);
//   tVal.tv_sec = now;
//   settimeofday(&tVal,NULL);
// }
void showSystemUpdate()
{
  timeval tmv;
  gettimeofday(&tmv, NULL);
  RtcDateTime nowTime = RtcDateTime(tmv.tv_sec);
  // tm *nowTime= gmtime(&tmv.tv_sec);
  String date;
  
  date = nowTime.Year() + ".  ";
  date += nowTime.Month() + ".  ";
  date += nowTime.Day() + "  ";
  char buf[80];
  char dayofweek[7][3] = LV_CALENDAR_DEFAULT_DAY_NAMES;
  sprintf(buf, "%d-%02d-%02d %s",
          nowTime.Year(), nowTime.Month(), nowTime.Day(), dayofweek[nowTime.DayOfWeek()]);
  lv_label_set_text(ui_lblDate, buf);

  // 셋팅에서 SAVED 메세지 출력이 열려 있는 경우 닫아주는 Function이다.
  lv_obj_flag_t flag = LV_OBJ_FLAG_HIDDEN;
  if (!lv_obj_has_flag(ui_lblMessage, flag))
  {
    if (minDisMessageTime-- == 0)
    {
      lv_obj_add_flag(ui_lblMessage, LV_OBJ_FLAG_HIDDEN);
      minDisMessageTime = 1;
    }
  }
  // lv_textarea_set_text(ui_txtYear, String(nowTime.Year()).c_str());
  // lv_textarea_set_text(ui_txtMonth, String(nowTime.Month()).c_str());
  // lv_textarea_set_text(ui_txtDay, String(nowTime.Day()).c_str());
  // lv_textarea_set_text(ui_txtHour, String(nowTime.Hour()).c_str());
  // lv_textarea_set_text(ui_txtMinute, String(nowTime.Minute()).c_str());
  // lv_textarea_set_text(ui_txtSecond, String(nowTime.Second()).c_str());
  // ESP_LOGI(TAG,"%s",buf);

  sprintf(buf, "%02d:%02d:%02d",
          nowTime.Hour(), nowTime.Minute(), nowTime.Second());
  lv_label_set_text(ui_lblTime, buf);
  // ESP_LOGI(TAG,"%s",buf);

  sprintf(buf, "%d-%02d-%02d %02d:%02d:%02d",
          nowTime.Year(), nowTime.Month(), nowTime.Day(),
          nowTime.Hour(), nowTime.Minute(), nowTime.Second());
  lv_label_set_text(ui_lblDateTime, buf);

// 초기 화면
  double Nominal_Capacity = upsModbusData.Nominal_Capacity/10.0;
  if(Nominal_Capacity == (int)Nominal_Capacity )// 10의 배수이면 
  {
    lv_label_set_text(ui_lblCapacity, String(Nominal_Capacity,0 ).c_str());
  } //10의 배수가 아니면
  else{
    lv_label_set_text(ui_lblCapacity, String(Nominal_Capacity,1).c_str());
  }
  lv_label_set_text(ui_lblBatteryCellNo, String(upsModbusData.Nominal_BatVoltage).c_str());
  lv_label_set_text(ui_lblInputVol, String(upsModbusData.Nominal_InputVoltage).c_str());
  lv_label_set_text(ui_lblOutputVol, String(upsModbusData.Nominal_OutputVoltage).c_str());


  //메인화면 //battery 용량 //sprintf(buf, "%d", rand() % (4) + 90);
  lv_label_set_text(ui_lbaBatCapacity, String(upsModbusData.battery_capacity).c_str());
  //sprintf(buf, "%d", rand() % (10) + 20);
  lv_label_set_text(ui_lblLoadCapacity, String(upsModbusData.load_percentage).c_str());

  lv_label_set_text(ui_lblDcLinkVol, String(upsModbusData.vdc_link_volt_rms).c_str());
  lv_label_set_text(ui_lblBatteryVol, String(upsModbusData.bat_volt_rms).c_str());
  lv_label_set_text(ui_lblInverterVol, String(upsModbusData.inverter_volt_rms).c_str());
  lv_label_set_text(ui_lblOutputV, String(upsModbusData.output_volt_rms).c_str());



	uint16_t selectedTab=0;
	selectedTab = lv_tabview_get_tab_act(ui_TabView1);
  if (selectedTab == 3) //탭이 활성화 되어 있다면 update를 하지 않는다.
  {
    lv_textarea_set_text(ui_txtYear, String(nowTime.Year()-2000).c_str());
    lv_textarea_set_text(ui_txtMonth, String(nowTime.Month()).c_str());
    lv_textarea_set_text(ui_txtDay, String(nowTime.Day()).c_str());
    lv_textarea_set_text(ui_txtHour, String(nowTime.Hour()).c_str());
    lv_textarea_set_text(ui_txtMinute, String(nowTime.Minute()).c_str());
    lv_textarea_set_text(ui_txtSecond, String(nowTime.Second()).c_str());
  }
}

void drawCursor(int16_t x, int16_t y, uint16_t color)
{
  int16_t r = 40;
  uint16_t w = 10;
  int16_t xx = x < r ? 0 : x - r;
  int16_t yy = y < r ? 0 : y - r;

  gfx->fillRect(x - w / 2, yy, w, r * 2, color);
  gfx->fillRect(xx, y - w / 2, r * 2, w, color);
}

void touchCalibrationInit()
{
  gfx->setTextSize(2);
  gfx->setRotation(0);
}
void setRtc()
{
  Rtc.Begin();
  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printf("\r\ncompiled time is %d/%d/%d %d:%d:%d\r\n", compiled.Year(), compiled.Month(), compiled.Day(), compiled.Hour(), compiled.Minute(), compiled.Second());
  if (!Rtc.IsDateTimeValid() )
  {
    printf("RTC lost confidence in the DateTime!\r\n");
    Rtc.SetDateTime(compiled);
  }
  if (Rtc.GetIsWriteProtected())
  {
    printf("RTC was write protected, enabling writing now\r\n");
    Rtc.SetIsWriteProtected(false);
  }
  else
    printf("RTC enabling writing \r\n");
  if (!Rtc.GetIsRunning())
  {
    printf("RTC was not actively running, starting now\r\n");
    Rtc.SetIsRunning(true);
  }
  else
    printf("RTC was actively status running \r\n");
  if (!Rtc.GetIsRunning())
  {
    printf("RTC was not actively running, starting now\r\n");
    Rtc.SetIsRunning(true);
  }
  else
    printf("RTC was actively status running \r\n");

  RtcDateTime now = Rtc.GetDateTime();
  printf("\r\nnow time is %d/%d/%d %d:%d:%d\r\n", now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
  if (now < compiled)
  {
    printf("\r\nSet data with compiled time");
    Rtc.SetDateTime(compiled);
  }
  else if (now > compiled)
  {
        Serial.println("RTC is newer than compile time. (this is expected)");
  }
  else if (now == compiled) 
  {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
  }

  struct timeval tmv;
  tmv.tv_sec = now.TotalSeconds();
  tmv.tv_usec = 0;
  // time_t toUnixTime = now.Unix32Time();
  // 시스템의 시간도 같이 맞추어 준다.
  settimeofday(&tmv, NULL);
  gettimeofday(&tmv, NULL);
  now = RtcDateTime(tmv.tv_sec);
  // now = tmv.tv_sec;
  printf("\r\nset and reread time is %d/%d/%d %d:%d:%d\r\n", now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());

  myWire.end();
}
void setRtcNewTime(RtcDateTime rtc)
{
  // digitalWrite(RTCEN , HIGH);
  //  SPI.end();
  //  Rtc.Begin();
  digitalWrite(TOUCH_XPT2046_CS, HIGH);
  SPI.end();
  myWire.begin();
  vTaskDelay(1);
  if (!Rtc.GetIsRunning())
  {
    printf("RTC was not actively running, starting now\r\n");
    Rtc.SetIsRunning(true);
  }
  else
    printf("RTC was actively status running \r\n");
  Rtc.SetDateTime(rtc);
  digitalWrite(TOUCH_XPT2046_CS, LOW);
  myWire.end();
  touch_init();

  struct timeval tmv;
  tmv.tv_sec = rtc.TotalSeconds();
  tmv.tv_usec = 0;
  // time_t toUnixTime = now.Unix32Time();
  // 시스템의 시간도 같이 맞추어 준다.
  settimeofday(&tmv, NULL);
  gettimeofday(&tmv, NULL);
  // myWire.end();
  // SPI.begin(SCK,MISO,MOSI,RTCEN );
}

int16_t isEventLogChanged = 0;
int16_t isAlarmLogChanged = 0;
void isGetSetEventData(){
  if (isEventLogChanged)
  {
    // ESP_LOGW("UI", "%d %d %d %d", isAlarmLogChanged,
    //          upsModbusData.ModuleState.status,
    //          upsModbusData.HWState.status,
    //          upsModbusData.upsOperationFault.status);
    String retStr = upslogEvent.readCurrentLogExt(CURRENTLOG,true);
    lv_textarea_set_text(ui_eventTextArea, retStr.c_str());
    while (lv_textarea_get_cursor_pos(ui_eventTextArea))
    {
      //ESP_LOGW("UI EventAlarm", "lv_textarea_cursor_up%d", lv_textarea_get_cursor_pos(ui_eventTextArea));
      lv_textarea_cursor_up(ui_eventTextArea);
    }

    lv_event_send(ui_btnAlarmPrev2,LV_EVENT_CLICKED,0);
  }

  if (isAlarmLogChanged)
  {
    upslogAlarm.setCurrentMemoryPage(0);
    String retStr = upslogAlarm.readCurrentLogExt(CURRENTLOG,true);
    if(upslogAlarm.eventHistory ==0 ) retStr =""; //알람이 없으므로 클리어 하여 준다.
    lv_textarea_set_text(ui_alarmTextArea, retStr.c_str());
    ESP_LOGW("UI EventAlarm", "ui_alarmTextArea %s", retStr.c_str());
    while (lv_textarea_get_cursor_pos(ui_alarmTextArea))
    {
      //ESP_LOGW("UI EventAlarm", "lv_textarea_cursor_up%d", lv_textarea_get_cursor_pos(ui_alarmTextArea));
      lv_textarea_cursor_up(ui_alarmTextArea);
    }
    lv_event_send(ui_btnAlarmPrev2,LV_EVENT_CLICKED,0);
  }
}
void GetSetEventData()
{
  upslog_t log;
  isEventLogChanged = upslogEvent.setEventCode(upsModbusData.ModuleState.status,
                                               upsModbusData.HWState.status,
                                               upsModbusData.upsOperationFault.status);

  isAlarmLogChanged = upslogAlarm.setEventCode(upsModbusData.ModuleState.status,
                                               upsModbusData.HWState.status,
                                               upsModbusData.upsOperationFault.status);
}

void mainScrUpdata(){
    //lv_obj_set_style_bg_img_recolor(ui_imgConvPowerLine, lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT );
  //정전일때 꺼지고, 평상시는 파워가 인가 된다.
  if (upsModbusData.upsOperationFault.Bit.utility_line_failure == 1) 
    {
      lv_obj_set_style_bg_color(ui_pnlMainPower1, lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_pnlMainPower2, lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_pnlMainPower3, lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
     
      lv_obj_set_style_bg_opa(ui_pnlMainPower1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlMainPower2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlMainPower3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    }
  else {
      lv_obj_set_style_bg_color(ui_pnlMainPower1, lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_pnlMainPower2, lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_pnlMainPower3, lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);

      lv_obj_set_style_bg_opa(ui_pnlMainPower1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlMainPower2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlMainPower3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
  }
  //바이패스모드 Change 바이패스와 인버터 SCR은 항상 반대로 
  if (upsModbusData.HWState.Bit.TRANSFER_RUN_STOP_STATE == 0 ) // 바이패스 모드
    {
      lv_obj_set_style_bg_color(ui_imgScrBypass, lv_color_hex(ONSCR_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT );
      lv_obj_set_style_img_recolor(ui_imgScrBypass, lv_color_hex(OFFCONV_FORE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_img_recolor_opa(ui_imgScrBypass, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

      lv_obj_set_style_bg_color(ui_imgScrOutput, lv_color_hex(OFFSCR_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT );
      lv_obj_set_style_img_recolor(ui_imgScrOutput, lv_color_hex(BLACK), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_img_recolor_opa(ui_imgScrOutput, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

      lv_obj_set_style_bg_color(ui_pnlInvPower1 , lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_pnlInvPower2 , lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_pnlInvPower3 , lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_pnlInvPower4 , lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlInvPower1 , 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlInvPower2 , 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlInvPower3 , 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlInvPower4 , 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      //lv_obj_set_style_bg_opa(ui_pnlInvPower1, 0, LV_PART_MAIN| LV_STATE_DEFAULT);
      // Bypass인데 정전이 아니어야 한다.  물론 이경우는 생기지는 않느다. 
      // 이미 정전을 감지 했으면 이 루틴으로 들어오지 않으며, 그것도 아니면 이미 셧다운이다. 
      // 단지 Simulation을 위해 사용한다.
    }
  else {
      lv_obj_set_style_bg_color(ui_imgScrBypass, lv_color_hex(OFFSCR_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT );
      lv_obj_set_style_img_recolor(ui_imgScrBypass, lv_color_hex(BLACK), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_img_recolor_opa(ui_imgScrBypass, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

      lv_obj_set_style_bg_color(ui_imgScrOutput, lv_color_hex(ONSCR_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT );
      lv_obj_set_style_img_recolor(ui_imgScrOutput, lv_color_hex(OFFCONV_FORE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_img_recolor_opa(ui_imgScrOutput, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

      lv_obj_set_style_bg_color(ui_pnlInvPower1 , lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_pnlInvPower2 , lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_pnlInvPower3 , lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_color(ui_pnlInvPower4 , lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);

      lv_obj_set_style_bg_opa(ui_pnlInvPower1 , 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlInvPower2 , 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlInvPower3 , 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlInvPower4 , 255, LV_PART_MAIN| LV_STATE_DEFAULT);
      //lv_obj_set_style_bg_opa(ui_pnlInvPower1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
  }
  // 충전부 운전  bg = white or red , 
  if(upsModbusData.HWState.Bit.CONVERTER_RUN_STOP_STATE){
    lv_obj_set_style_bg_color(ui_imgConvertor, lv_color_hex(ONCONV_BG_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_img_recolor(ui_imgConvertor, lv_color_hex(OFFCONV_FORE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_imgConvertor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_pnlConvPower1, lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_pnlConvPower2, lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_pnlConvPower1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_pnlConvPower2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

  }
  else{
    lv_obj_set_style_bg_color(ui_imgConvertor, lv_color_hex(OFFCONV_FORE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_img_recolor(ui_imgConvertor, lv_color_hex(BLACK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_imgConvertor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_pnlConvPower1, lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_pnlConvPower1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    if(upsModbusData.HWState.Bit.DC_DC_CONVERTER_RUN_STOP_STATE==0)
    {
      lv_obj_set_style_bg_color(ui_pnlConvPower2, lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlConvPower2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    }
    else 
    {
      lv_obj_set_style_bg_color(ui_pnlConvPower2, lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlConvPower2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    }
  }
  if(upsModbusData.HWState.Bit.DC_DC_CONVERTER_RUN_STOP_STATE)
  {
      lv_obj_set_style_bg_color(ui_pnlConvPower3, lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlConvPower3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

      lv_obj_set_style_bg_color(ui_imgBattery, lv_color_hex(ONCONV_BG_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT );
      lv_obj_set_style_img_recolor(ui_imgBattery, lv_color_hex(OFFCONV_FORE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_img_recolor_opa(ui_imgBattery, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  else{
      lv_obj_set_style_bg_color(ui_pnlConvPower3, lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_bg_opa(ui_pnlConvPower3, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

      lv_obj_set_style_bg_color(ui_imgBattery, lv_color_hex(OFFCONV_FORE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT );
      lv_obj_set_style_img_recolor(ui_imgBattery, lv_color_hex(BLACK), LV_PART_MAIN | LV_STATE_DEFAULT);
      lv_obj_set_style_img_recolor_opa(ui_imgBattery, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  //인버터 시작 ,인버터와 연결되는 라인도 같이 전원을 인가한다.
  if(upsModbusData.HWState.Bit.INVERTER_RUN_STOP_STATE ){
    //lv_obj_set_style_bg_color(ui_imgInvertor, lv_color_hex(ONCONV_BG_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_bg_color(ui_imgInvertor, lv_color_hex(ONCONV_BG_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_img_recolor(ui_imgInvertor, lv_color_hex(OFFCONV_FORE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_imgInvertor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_pnlInvPower, lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_pnlInvPower, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_color(ui_imgInvertorPowerLine, lv_color_hex(ONLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT );
  }
  else //인버터 정지,인버터와 연결되는 라인도 같이 전원을 제거 한다.
  {
    lv_obj_set_style_bg_color(ui_imgInvertor, lv_color_hex(OFFCONV_FORE_COLOR ), LV_PART_MAIN | LV_STATE_DEFAULT );
    lv_obj_set_style_img_recolor(ui_imgInvertor, lv_color_hex(BLACK), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_img_recolor_opa(ui_imgInvertor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui_pnlInvPower, lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_pnlInvPower, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
    //lv_obj_set_style_bg_color(ui_imgInvertorPowerLine, lv_color_hex(OFFLINE_COLOR), LV_PART_MAIN | LV_STATE_DEFAULT );
  }
  toggleBuzzer();
}
/* 부저는 UPS에서 설정하면 작동을 시작한다. 
*  하지만 사용자가 강제 중지하는 경우 애니매이션은 제외하고 부저음은 작동을 멈춘다.
*  다시 작동시키기 위서는 부저 버튼을 클릭한다.
*/
void toggleBuzzer()
{
  // 서버에서는 현재 알람 상태이다.
  if (upslogAlarm.eventHistory)
  {
    if (upslogAlarm.runBuzzStatus) // 이 값은 서버에서 값에 의해서도 변경된다.
    {
      digitalWrite(BUZZER, !digitalRead(BUZZER));
    }
    else 
      digitalWrite(BUZZER, LOW);

    lv_opa_t current_opa = lv_obj_get_style_bg_img_opa(ui_btnAlarm, LV_PART_MAIN);
    if (current_opa == 0)
      lv_obj_set_style_bg_img_opa(ui_btnAlarm, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    else
      lv_obj_set_style_bg_img_opa(ui_btnAlarm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  else  //알람상태가 아니다 
  {
    digitalWrite(BUZZER, LOW);
    lv_obj_set_style_bg_img_opa(ui_btnAlarm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  }
}

int modbusEventSendLoop(int timeout);
void setTimeText()
{
    timeval tmv;
    gettimeofday(&tmv, NULL);
    RtcDateTime nowTime = RtcDateTime(tmv.tv_sec);
    lv_textarea_set_text(ui_txtYear, String(nowTime.Year() - 2000).c_str());
    lv_textarea_set_text(ui_txtMonth, String(nowTime.Month()).c_str());
    lv_textarea_set_text(ui_txtDay, String(nowTime.Day()).c_str());
    lv_textarea_set_text(ui_txtHour, String(nowTime.Hour()).c_str());
    lv_textarea_set_text(ui_txtMinute, String(nowTime.Minute()).c_str());
    lv_textarea_set_text(ui_txtSecond, String(nowTime.Second()).c_str());

    lv_textarea_set_text(ui_txtOfftime, String(nvsSystemEEPRom.systemLedOffTime).c_str());
    lv_textarea_set_text(ui_txtBrigtness, String(nvsSystemEEPRom.lcdBright).c_str());

}
void setup()
{
  Serial.begin(BAUDRATEDEF);
  EEPROM.begin(sizeof(nvsSystemSet_t) + 1);

  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON_ERASE , INPUT);
  digitalWrite(BUZZER, LOW);

  lsFile.littleFsInitFast(0);
  lsFile.setOutputStream(&Serial);

  bleSetup();
  upslogEvent.getFileSize();
  // mySerialBT.begin("UPS1P1P_BLE");
  if (EEPROM.read(0) != 0x55)
  {
    nvsSystemEEPRom.systemLanguage = 1; // default Not Set Hangul
    nvsSystemEEPRom.systemModusId = 1;
    nvsSystemEEPRom.lcdBright = 255 /*0xFFFF*/;
    nvsSystemEEPRom.systemLedOffTime = 10;
    nvsSystemEEPRom.IPADDRESS = (uint32_t)IPAddress(192, 168, 0, 57);
    nvsSystemEEPRom.GATEWAY = (uint32_t)IPAddress(192, 168, 0, 1);
    nvsSystemEEPRom.SUBNETMASK = (uint32_t)IPAddress(255, 255, 255, 0);
    nvsSystemEEPRom.WEBSOCKETSERVER = (uint32_t)IPAddress(192, 168, 0, 57);
    nvsSystemEEPRom.WEBSERVERPORT = 81;
    nvsSystemEEPRom.BAUDRATE = 9600;
    nvsSystemEEPRom.alarmSetStatus = 0;
    nvsSystemEEPRom.systemModbusQInterval = 300;
    nvsSystemEEPRom.operatingMode.sysOpModeBit.modbusMode = 0; // agent
    nvsSystemEEPRom.operatingMode.sysOpModeBit.useBlueTooth = 1;
    nvsSystemEEPRom.operatingMode.sysOpModeBit.useWiFi = 1;
    nvsSystemEEPRom.operatingMode.sysOpModeBit.useWebServer = 1;

    strncpy(nvsSystemEEPRom.deviceName, "UPS1P1P", 9);

    EEPROM.writeByte(0, 0x55);
    // EEPROM.commit();
    EEPROM.writeBytes(1, (const byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
    EEPROM.commit();
    Serial.println("Memory Initialized first booting....");
  }
  EEPROM.readBytes(1, (byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
  // nvsSystemEEPRom.BAUDRATE = 9600;
  //  while (!Serial);
  nvsSystemEEPRom.systemLedOffTime = nvsSystemEEPRom.systemLedOffTime < 10 ? 10 : nvsSystemEEPRom.systemLedOffTime;
  setRtc();

  Serial.println("Setup done");
  pinMode(SERIAL_TX2 , OUTPUT);
  pinMode(SERIAL_RX2 , INPUT);
  Serial2.begin(nvsSystemEEPRom.BAUDRATE, SERIAL_8N1, SERIAL_RX2  /* RX */, SERIAL_TX2  /* TX*/);
  Serial2.println("Serial 1 started");
  modbusSetup();
  // Init Display
  // Add
  gfx->begin();
  gfx->fillScreen(BLACK);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  ledcSetup(0, 300, 8);
  ledcAttachPin(TFT_BL, 0);

  uint16_t brightness = map(nvsSystemEEPRom.lcdBright, 0, 255, 0, 255);
  ledcWrite(0, brightness);

  gfx->fillScreen(RED);
  delay(100);
  gfx->fillScreen(GREEN);
  delay(100);
  gfx->fillScreen(BLUE);
  delay(100);
  gfx->fillScreen(BLACK);
  delay(100);
  gfx->setCursor(0, 10);
  // GFXfont *f;
  // f->bitmap = (uint8_t *)&FreeSansBold12pt7bBitmaps;
  // gfx->setFont(f);
  gfx->setTextColor(WHITE);
  gfx->printf("\nVER:%s", VERSION);
  gfx->print("\nInit RTC");
  gfx->printf("\nbaud Rate %d", nvsSystemEEPRom.BAUDRATE);
  gfx->printf("\nlanguage %d", nvsSystemEEPRom.systemLanguage);
  gfx->printf("\nlcdBright language %d", nvsSystemEEPRom.lcdBright);
  gfx->printf("\nsystemLedOffTime %d", nvsSystemEEPRom.systemLedOffTime);
  gfx->println("\nmodbus started");
  gfx->println("\nCheck modbus Serial comm...");

#ifndef DONOTUSECOMM
  // while (!modbusEventSendLoop(1000))
  // {
  //   gfx->print(".");
  // }
#endif
  gfx->println("\nCheck modbus Serial OK");
  // vTaskDelay(1000);
  touchCalibrationInit();

  lv_i18n_init(lv_i18n_language_pack);

  if (nvsSystemEEPRom.systemLanguage == 1){
    lv_i18n_set_locale("ko-KR");
  }
  else //if (nvsSystemEEPRom.systemLanguage == 2)
  {
    lv_i18n_set_locale("en-GB");
  }
  lv_init();

  // led = lv_led_create(lv_scr_act());

  // Init touch devicSeconde
  // pinMode(TOUCH_GT911_RST, OUTPUT);
  // digitalWrite(TOUCH_GT911_RST, LOW);
  // delay(10);
  // digitalWrite(TOUCH_GT911_RST, HIGH);
  // delay(10);
  touch_init();

  screenWidth = gfx->width();
  screenHeight = gfx->height();

  disp_draw_buf = (lv_color_t *)malloc(sizeof(lv_color_t) * screenWidth * screenHeight / 6);

  if (!disp_draw_buf)
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  }
  else
  {
    lv_disp_draw_buf_init(&draw_buf, disp_draw_buf, NULL, screenWidth * screenHeight / 6);

    /* Initialize the display */
    lv_disp_drv_init(&disp_drv);
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    /*Initialize the (dummy) input device driver*/
    /* Initialize the (dummy) input device driver */
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;

    lv_indev_drv_register(&indev_drv);

    // lv_i18n_set_locale("ko-KR");
    // lv_i18n_set_locale("en-GB");

    // upslog_t log;

    // for (int i = 0; i < 16; i++)
    //   Serial.printf("\n%s", upslog.converter_status_eng[i]);
    // for (int i = 0; i < 16; i++)
    //   Serial.printf("\n%s", upslog.operation_falut_eng[i]);

    // for (int i = 0; i < 16; i++)
    //   Serial.printf("\n%s", upslog.converter_status_eng[i]);
    // for (int i = 0; i < 16; i++)
    //   Serial.printf("\n%s", upslog.operation_falut_eng[i]);

    ui_init();
    lv_obj_scroll_to_view_recursive(ui_alarmTextArea,LV_ANIM_OFF);
    lv_obj_scroll_to_view_recursive(ui_eventTextArea,LV_ANIM_OFF);
    lv_tabview_set_act(ui_TabView2,1,LV_ANIM_OFF); 
    lv_tabview_set_act(ui_TabView2,0,LV_ANIM_OFF); 
    // lv_obj_add_flag(ui_TabView2,LV_OBJ_FLAG_GESTURE_BUBBLE);
    myui_MainScreen_screen_init();
    // setTime();

    /* System setting screen*/
    setTimeText();

  }
  // Modbus는 내부적으로 task를 사용하고 있다.
  xTaskCreatePinnedToCore(systemControllTask, "systemControllTask", 5000, NULL, 1, h_pxsystemControllTask, 0);  
#ifdef USEWIFI
  wifiOTAsetup();
#endif
  EEPROM.readBytes(1, (byte *)&nvsSystemEEPRom, sizeof(nvsSystemEEPRom));

  // 시스템 시작시에는 바이패스로 놓는다
  // upsModbusData.ModuleState.Bit.To_Bypass_ModeChange = 1;
  // 충전기 모듈은 정지되어 있는 상태이다
  upsModbusData.ModuleState.Bit.Charger_RUN = 0;
  // 인버터 정지 상태이다;
  upsModbusData.ModuleState.Bit.Inverter_RUN = 0;
 
  setMemoryDataToLCD();
  // esp_task_wdt_init(WDT_TIMEOUT,true);
  // esp_task_wdt_add(NULL);
  // while(1){
  //     digitalWrite(BUZZER, !digitalRead(BUZZER));
  //     delay(200);
  //     Serial.println("buzzer test");
  // }
};
static int interval = 1000;
static unsigned long previous300mills = 0;
static unsigned long previous1000mills = 0;
static int everySecondInterval = 1000;
static int every300ms = 300;

static unsigned long previous500mills = 0;
static int every500ms = 500;
static unsigned long now;
uint16_t incTime = 0;
// int modbusEventSendLoop(int timeout);
// int modbusEventGetLoop();
void showMessageLabel(const char *message);
//extern lv_obj_t * ui_MainScreen;
uint16_t pressedResetButton=0;
void loop()
{
  void *parameters;
  //esp_task_wdt_reset();
  wifiOtaloop();
  bleCheck();
  now = millis();
  #ifndef DONOTUSECOMM
  if(modbusErrorCounter>2)
    showMessageLabel(_("Comm_Error"));
  #endif
  isGetSetEventData();
  if ((now - previous300mills > every300ms))
  {
    showSystemUpdate();
    mainScrUpdata();
    // 여기서 모드버스 통신을 하자
    //
    //modbusEventSendLoop(30);
    // modbusEventSendLoop(100);
    // GetSetEventData();
    previous300mills = now;
  }

  if ((now - previous500mills > every500ms))
  {
    previous500mills = now;
  }
  if ((now - previous1000mills > everySecondInterval))
  {
    previous1000mills = now;
    incTime++;
    lcdOntime++;
    if(digitalRead(BUTTON_ERASE )==0){
      ESP_LOGI("IO","Press Init button %d",digitalRead(BUTTON_ERASE ));
      pressedResetButton++;
      if(pressedResetButton>3){
        {
          showMessageLabel(_("Log_Init"));
          lsFile.rm("eventLog.hex");
          upslogEvent.getFileSize();
          upslogEvent.readCurrentLogExt(CURRENTLOG,true);

          nvsSystemEEPRom.systemLedOffTime = 10;
          nvsSystemEEPRom.lcdBright= 255;
          EEPROM.writeBytes(1, (const byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
          EEPROM.commit();
          //upsLog upslogEvent("/spiffs/eventLog.hex", EVENT_TYPE);
          upslogEvent.init();
          ui_init();
          pressedResetButton =0;
          ESP_LOGI("IO","Now On file format...Do not Turn Off system");
        }
      }
    }
    else{
      pressedResetButton =0;
    }

    if (nvsSystemEEPRom.systemLedOffTime != 0 && lcdOntime >= nvsSystemEEPRom.systemLedOffTime*60) // lv_led_off(led);
    {
      //_ui_screen_change(&ui_InitScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_InitScreen_screen_init); // lv_disp_load_scr( ui_InitScreen);
      _ui_screen_change(&ui_MainScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_MainScreen_screen_init);
      ledcWrite(0, 0);
    }
  }
  lv_timer_handler(); /* let the GUI do its work */
  vTaskDelay(10);     // Every 50ms
}

void systemControllTask(void *parameter)
{
  for (;;)
  {
    modbusEventSendLoop(100);
    GetSetEventData();
    vTaskDelay(300);
  };
}