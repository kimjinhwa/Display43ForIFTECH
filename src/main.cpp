#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <RtcDS1302.h>
#include "esp32-hal-gpio.h"
#include "mainGrobal.h"
#include "myBlueTooth.h"
#include "fileSystem.h"
#include "../Version.h"
#include "esp_task_wdt.h"
#include "soc/timer_group_reg.h"
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
#include "freertos/semphr.h"
#include "display.h"

/* Board: MCP23S08 GP1=buzzer CS=GPIO17. DS1302 CE=GPIO43 (UART0 TX). */
#define SERIAL_RX2 19
#define SERIAL_TX2 20
#define A23S08_CS 17

#include <Mcp23s08.h>
#include "rtcCeGpioWire.h"
#include "spiBusCs.h"
#include "driver/uart.h"

#define WDT_TIMEOUT 60 
#define TFT_BL DISPLAY_BL_PIN

#define BUTTON_ERASE 0 

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
TaskHandle_t *h_pxsystemControllTask;

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
static char TAG[] = "main";

/** DS1302 CE=GPIO43. SCK/MOSI는 터치·MCP와 공유 — RTC 구간에서는 터치 읽기 금지 */


extern LittleFileSystem lsFile;
extern jobCommant_t systemControllJob;

upsLog upslogEvent("/spiffs/eventLog.hex", EVENT_TYPE);
upsLog upslogAlarm(FAULT_TYPE); //

int enalbeTouchEdit=0;
void mainScrUpdata();
void GetSetEventData();
void systemControllTask(void *parameter);
void scrSettingScreen();
void scrMeasureLoad();
void toggleBuzzer();
void RebootSystem(uint16_t afterTime);

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
  /* RGB DMA가 PSRAM을 쓰는 동안 코어가 플래시에 묶이지 않게 양보 */
  taskYIELD();
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
  if (enalbeTouchEdit){
    Serial.print(".");
    return;
  } 
  else{
    //Serial.print("*");
  }
  if (touch_has_signal())
  {
    //Serial.print(".");
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
        //touch_last_x = map(p.x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, gfx->width() - 1);
        //touch_last_y = map(p.y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, gfx->height() - 1);
        data->point.x = touch_last_x = p.x;
        data->point.y = touch_last_y = p.y;
        ledcWrite(0, nvsSystemEEPRom.lcdBright);
        lcdOntime = 1;
        //ESP_LOGI("TOUCH", "Data (x,y,z)(%d,%d,%d),brightness %d", data->point.x, data->point.y, p.z, nvsSystemEEPRom.lcdBright  );
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
}

void setMemoryDataToLCD()
{

}
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


  //메인화면 //battery 용량
  snprintf(buf, sizeof(buf), "#ff0000 BAT# %u #ff0000 %%#",
           (unsigned)upsModbusData.battery_capacity);
  lv_label_set_text(ui_lbaBatCapacity, buf);
  snprintf(buf, sizeof(buf), "#ff0000 LOAD# %u #ff0000 %%#",
           (unsigned)upsModbusData.load_percentage);
  lv_label_set_text(ui_lblLoadCapacity, buf);

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

void stopTouchSpi(void)
{
  ts.penirqControl(0x93);
  rtcCeGpioLow();
  Mcp23s08_RtcCe(false);
  enalbeTouchEdit=1;
  lv_indev_enable(NULL, false);
  digitalWrite(TOUCH_XPT2046_CS, HIGH);
  digitalWrite(A23S08_CS, HIGH);
}
void startTouchSpi(void) {

  enalbeTouchEdit=0;
  delay(300);
  digitalWrite(TOUCH_XPT2046_CS, LOW);
  // static lv_indev_drv_t indev_drv;
  // lv_indev_drv_init(&indev_drv);
  // indev_drv.type = LV_INDEV_TYPE_POINTER;
  // indev_drv.read_cb = my_touchpad_read;
  // lv_indev_drv_register(&indev_drv);
  // enalbeTouchEdit = 0;
  // Serial.println("SPI.begin");
  lv_indev_enable(NULL, true);
  touch_init();
}

static bool g_rtcBootSynced = false;
static bool g_rtcHardwareMissing = false;

/** DS1302 RAM 왕복. 칩이 없으면 스탬프가 돌아오지 않는다. */
template<typename TWire>
static bool probeRtcRam(RtcDS1302<TWire> &rtc)
{
  bool wasProtected = rtc.GetIsWriteProtected();
  if (wasProtected)
    rtc.SetIsWriteProtected(false);

  const uint8_t stamp[4] = {0xA5, 0x5A, 0x3C, (uint8_t)~0x3C};
  bool ok = false;
  for (int i = 0; i < 2 && !ok; i++) {
    if (rtc.SetMemory(stamp, 4) != 4) {
      vTaskDelay(pdMS_TO_TICKS(10));
      continue;
    }
    uint8_t check[4] = {0};
    if (rtc.GetMemory(check, 4) == 4 &&
        check[0] == stamp[0] && check[1] == stamp[1] &&
        check[2] == stamp[2] && check[3] == stamp[3]) {
      ok = true;
      break;
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }

  if (wasProtected)
    rtc.SetIsWriteProtected(true);
  return ok;
}

/** Burst read @p samples times; true if all valid and TotalSeconds span <= @p maxSpreadSec. */
template<typename TWire>
static bool readRtcBurstConsistent(RtcDS1302<TWire> &rtc, RtcDateTime &out,
                                   int samples = 3, uint32_t maxSpreadSec = 1)
{
  if (samples < 2)
    samples = 2;

  RtcDateTime readings[5];
  if (samples > (int)(sizeof(readings) / sizeof(readings[0])))
    samples = (int)(sizeof(readings) / sizeof(readings[0]));

  uint32_t minTot = UINT32_MAX;
  uint32_t maxTot = 0;

  for (int i = 0; i < samples; i++) {
    readings[i] = rtc.GetDateTime();
    if (!readings[i].IsValid())
      return false;

    uint32_t tot = readings[i].TotalSeconds();
    if (tot < minTot)
      minTot = tot;
    if (tot > maxTot)
      maxTot = tot;

    if (i + 1 < samples)
      vTaskDelay(pdMS_TO_TICKS(50));
  }

  if (maxTot - minTot > maxSpreadSec)
    return false;

  out = readings[samples - 1];
  return true;
}

/** RAM stamp + burst consistency + seconds register cross-check (SPI glitch filter). */
template<typename TWire>
static bool readRtcTrusted(RtcDS1302<TWire> &rtc, RtcDateTime &out,
                           int maxAttempts = 5, int burstSamples = 3,
                           uint32_t maxSpreadSec = 1)
{
  if (maxAttempts < 1)
    maxAttempts = 1;

  for (int attempt = 0; attempt < maxAttempts; attempt++) {
    const uint8_t seq = (uint8_t)(millis() & 0xFF);
    const uint8_t stamp[4] = {0xA5, 0x5A, seq, (uint8_t)~seq};
    bool wasProtected = rtc.GetIsWriteProtected();
    if (wasProtected)
      rtc.SetIsWriteProtected(false);

    bool ramWriteOk = (rtc.SetMemory(stamp, 4) == 4);
    if (!ramWriteOk) {
      if (wasProtected)
        rtc.SetIsWriteProtected(true);
      continue;
    }

    if (!readRtcBurstConsistent(rtc, out, burstSamples, maxSpreadSec)) {
      if (wasProtected)
        rtc.SetIsWriteProtected(true);
      continue;
    }

    const uint8_t secReg = rtc.GetSecondsRegister();
    if (!RtcDS1302<TWire>::IsBcdSecondsByteValid(secReg)) {
      if (wasProtected)
        rtc.SetIsWriteProtected(true);
      continue;
    }

    const uint8_t secDirect = BcdToUint8(secReg & 0x7F);
    int secDiff = (int)secDirect - (int)out.Second();
    if (secDiff > 30)
      secDiff -= 60;
    else if (secDiff < -30)
      secDiff += 60;
    if (secDiff < 0)
      secDiff = -secDiff;
    if (secDiff > 1) {
      printf("RTC sec mismatch burst=%u reg=%u raw=0x%02X\r\n",
             (unsigned)out.Second(), (unsigned)secDirect, (unsigned)secReg);
      if (wasProtected)
        rtc.SetIsWriteProtected(true);
      continue;
    }

    uint8_t check[4] = {0};
    if (rtc.GetMemory(check, 4) != 4 ||
        check[0] != stamp[0] || check[1] != stamp[1] || check[2] != stamp[2] ||
        check[3] != stamp[3]) {
      if (wasProtected)
        rtc.SetIsWriteProtected(true);
      continue;
    }

    if (wasProtected)
      rtc.SetIsWriteProtected(true);
    return true;
  }
  return false;
}

static void finishRtcBus(void)
{
  rtcCeGpioLow();
  digitalWrite(TOUCH_XPT2046_CS, HIGH);
  digitalWrite(A23S08_CS, HIGH);
  spiBusRecoverFromBitBang();
  rtcCeGpioLow();
}

void initSetRtc(){
  vTaskDelay(pdMS_TO_TICKS(80));
  finishRtcBus();
  Mcp23s08_RtcCe(false);

  RtcCeGpioWire myWire(TOUCH_XPT2046_MOSI, TOUCH_XPT2046_SCK, RTC_CE_GPIO);
  RtcDS1302<RtcCeGpioWire> Rtc(myWire);
  Rtc.Begin();
  rtcCeGpioLow();

  if (!probeRtcRam(Rtc)) {
    g_rtcBootSynced = false;
    g_rtcHardwareMissing = true;
    printf("RTC not present — keep ESP time\r\n");
    finishRtcBus();
    return;
  }

  if (Rtc.GetIsWriteProtected())
    printf("RTC is write protected\r\n");
  if (!Rtc.GetIsRunning()) {
    printf("RTC was not actively running, starting now\r\n");
    Rtc.SetIsRunning(true);
  }

  RtcDateTime now;
  bool rtcOk = false;
  const int kBootOuterRounds = 4;
  const int kTrustedAttempts = 20;
  const int kBurstSamples = 5;

  for (int round = 0; round < kBootOuterRounds && !rtcOk; round++) {
    if (round > 0) {
      printf("RTC boot read round %d/%d\r\n", round + 1, kBootOuterRounds);
      vTaskDelay(pdMS_TO_TICKS(120));
    }
    rtcOk = readRtcTrusted(Rtc, now, kTrustedAttempts, kBurstSamples, 1);
  }

  if (!rtcOk) {
    g_rtcBootSynced = false;
    printf("RTC boot read failed after %d rounds — keep ESP time, no settimeofday\r\n",
           kBootOuterRounds);
    finishRtcBus();
    return;
  }

  if (now.IsValid()) {
    g_rtcBootSynced = true;
    printf("\r\nnow RTC Time is %04u-%02u-%02u %02u:%02u:%02u (tot=%u)\r\n",
           (unsigned)now.Year(), (unsigned)now.Month(), (unsigned)now.Day(),
           (unsigned)now.Hour(), (unsigned)now.Minute(), (unsigned)now.Second(),
           (unsigned)now.TotalSeconds());

    struct timeval tmv;
    tmv.tv_sec = now.TotalSeconds();
    tmv.tv_usec = 0;
    settimeofday(&tmv, NULL);
    gettimeofday(&tmv, NULL);
    RtcDateTime systemRtc(tmv.tv_sec);
    printf("\r\nnow sys Time is %04u-%02u-%02u %02u:%02u:%02u (tot=%u)\r\n",
           (unsigned)systemRtc.Year(), (unsigned)systemRtc.Month(),
           (unsigned)systemRtc.Day(), (unsigned)systemRtc.Hour(),
           (unsigned)systemRtc.Minute(), (unsigned)systemRtc.Second(),
           (unsigned)systemRtc.TotalSeconds());
  } else {
    g_rtcBootSynced = false;
    printf("\r\nnow RTC Time INVALID after trusted read — skip settimeofday\r\n");
    vTaskDelay(50);
  }
  finishRtcBus();
  printf("RTC CE GPIO43 LOW after boot\r\n");
}

RtcDateTime setRtc(bool write, const RtcDateTime *newTime = new RtcDateTime(0))
{
  stopTouchSpi();
  finishRtcBus();
  Mcp23s08_RtcCe(false);

  RtcCeGpioWire myWire(TOUCH_XPT2046_MOSI, TOUCH_XPT2046_SCK, RTC_CE_GPIO);
  RtcDS1302<RtcCeGpioWire> Rtc(myWire);
  Rtc.Begin();
  rtcCeGpioLow();
  vTaskDelay(10);

  if (Rtc.GetIsWriteProtected())
    printf("RTC is write protected\r\n");
  if (!Rtc.GetIsRunning()) {
    printf("RTC was not actively running, starting now\r\n");
    Rtc.SetIsRunning(true);
  }
  if (write) {
    bool wasProtected = Rtc.GetIsWriteProtected();
    if (wasProtected) {
      printf("RTC write protect on — clearing for SetDateTime\r\n");
      Rtc.SetIsWriteProtected(false);
    }
    Rtc.SetDateTime(*newTime);
    if (wasProtected)
      Rtc.SetIsWriteProtected(true);
  }

  RtcDateTime now;
  if (!readRtcTrusted(Rtc, now)) {
    printf("\r\nRTC read rejected — keep system time (no settimeofday)\r\n");
    now = RtcDateTime(0);
  }

  if (now.IsValid()) {
    printf("\r\nnow RTC Time is %04u-%02u-%02u %02u:%02u:%02u (tot=%u)\r\n",
           (unsigned)now.Year(), (unsigned)now.Month(), (unsigned)now.Day(),
           (unsigned)now.Hour(), (unsigned)now.Minute(), (unsigned)now.Second(),
           (unsigned)now.TotalSeconds());

    struct timeval tmv;
    tmv.tv_sec = now.TotalSeconds();
    tmv.tv_usec = 0;
    settimeofday(&tmv, NULL);
    gettimeofday(&tmv, NULL);
    RtcDateTime systemRtc(tmv.tv_sec);
    printf("\r\nnow sys Time is %04u-%02u-%02u %02u:%02u:%02u (tot=%u)\r\n",
           (unsigned)systemRtc.Year(), (unsigned)systemRtc.Month(),
           (unsigned)systemRtc.Day(), (unsigned)systemRtc.Hour(),
           (unsigned)systemRtc.Minute(), (unsigned)systemRtc.Second(),
           (unsigned)systemRtc.TotalSeconds());
  } else if (write) {
    printf("\r\nnow RTC Time INVALID after write — skip settimeofday\r\n");
    vTaskDelay(50);
  }
  finishRtcBus();
  startTouchSpi();
  return now;
}

int16_t isEventLogChanged = 0;
int16_t isAlarmLogChanged = 0;
void isGetSetEventData(){
  if (isEventLogChanged)
  {
    String retStr = upslogEvent.readCurrentLogExt(CURRENTLOG,true);
    //ESP_LOGW("UI Event", "ui_eventTextArea %s", retStr.c_str());
    lv_textarea_set_text(ui_eventTextArea, retStr.c_str());
    while (lv_textarea_get_cursor_pos(ui_eventTextArea))
    {
      //ESP_LOGW("UI Event", "lv_textarea_cursor_up%d", lv_textarea_get_cursor_pos(ui_eventTextArea));
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
    //ESP_LOGW("UI Alarm", "ui_alarmTextArea %s", retStr.c_str());
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

  //ESP_LOGI("MODBUS","Receive Event Data %d %d %d", isEventLogChanged, isAlarmLogChanged,millis());
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
      Mcp23s08_BuzzerToggle();
    }
    else
      Mcp23s08_BuzzerControl(false);

    lv_opa_t current_opa = lv_obj_get_style_bg_img_opa(ui_btnAlarm, LV_PART_MAIN);
    if (current_opa == 0)
      lv_obj_set_style_bg_img_opa(ui_btnAlarm, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    else
      lv_obj_set_style_bg_img_opa(ui_btnAlarm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  else  //알람상태가 아니다 
  {
    Mcp23s08_BuzzerControl(false);
    lv_obj_set_style_bg_img_opa(ui_btnAlarm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  }
}

int modbusEventSendLoop(int token);
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
void initialEEPROM()
{
  EEPROM.begin(sizeof(nvsSystemSet_t) + 2);
  if (EEPROM.read(0) != 0x55 || EEPROM.read(sizeof(nvsSystemSet_t) + 1) != 0x55)
  {
    Serial.println("Memory Initialized first booting....");
    nvsSystemEEPRom.isUpdate = false;
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
    strncpy(nvsSystemEEPRom.ssid, "iftech", 6);
    strncpy(nvsSystemEEPRom.password, "iftech0273", 10);
    strncpy(nvsSystemEEPRom.deviceName, "UPS1P1P", 9);

    EEPROM.writeByte(0, 0x55);
    // EEPROM.commit();
    EEPROM.writeBytes(1, (const byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
    EEPROM.writeByte(sizeof(nvsSystemSet_t) + 1, 0x55);
    EEPROM.commit();
  }
  else
  {
    Serial.println("Using EEPROM....");
  }
  EEPROM.readBytes(1, (byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
}
#include "lv_tc.h"
#include "lv_tc_screen.h"
#include "esp_nvs_tc.h"
void calibrationTouchFinish_cb(lv_event_t *e)
{
  //lv_obj_t *tCScreen = lv_event_get_target(e);
  //lv_tc_coeff_t *coeff = lv_tc_get_coeff();
  lv_disp_load_scr(ui_MainScreen);
  ESP_LOGI("TOUCH", "calibrationTouchFinish_cb");
}
void calibrationTouchInit()
{
  bool is_initNvs = esp_nvs_tc_coeff_init();
  //P_LOGI("TOUCH", "esp_nvs_tc_coeff_init %d", is_initNvs);
  static lv_indev_drv_t indev_drv;
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = my_touchpad_read;
  //P_LOGI("TOUCH", "calib  Init");
  lv_tc_indev_drv_init(&indev_drv, my_touchpad_read);
  //P_LOGI("TOUCH", "lv_tc_indev_drv_init");
  lv_indev_drv_register(&indev_drv);
  //P_LOGI("TOUCH", "lv_indev_drv_register");
  lv_tc_register_coeff_save_cb(esp_nvs_tc_coeff_save_cb);
  /* 캘리브가 이미 있으면 화면을 만들지 않는다.
   * RGB 패널은 첫 프레임 전에 위젯을 더 만들면 TG1 인터럽트 WDT가 걸린다. */
  if(esp_nvs_tc_is_valid_cb())
  {
    ESP_LOGI("TOUCH", "esp_nvs_tc_is_valid_cb");
    return;
  }
  lv_obj_t *tCScreen = lv_tc_screen_create();
  lv_obj_add_event_cb(tCScreen, calibrationTouchFinish_cb, LV_EVENT_READY, NULL);
  lv_disp_load_scr(tCScreen);
  ESP_LOGI("TOUCH", "lv_tc_screen_start");
  lv_tc_screen_start(tCScreen);
}
#ifndef TIMG_WDT_WKEY_VALUE
#define TIMG_WDT_WKEY_VALUE 0x50D83AA1
#endif

/* RGB 패널 프레임버퍼는 PSRAM. 한글 폰트/PNG를 플래시에서 읽으면 캐시가 끊겨
 * LCD DMA가 굶고 TG1 인터럽트 WDT(기본 300ms)가 리셋한다. */
static void stretchRgbIntWdt(void)
{
#if defined(DISPLAY_43)
  const uint32_t stg0 = REG_READ(TIMG_WDTCONFIG2_REG(1));
  const uint32_t stg1 = REG_READ(TIMG_WDTCONFIG3_REG(1));
  WRITE_PERI_REG(TIMG_WDTWPROTECT_REG(1), TIMG_WDT_WKEY_VALUE);
  WRITE_PERI_REG(TIMG_WDTCONFIG2_REG(1), stg0 * 10);
  WRITE_PERI_REG(TIMG_WDTCONFIG3_REG(1), stg1 * 10);
  WRITE_PERI_REG(TIMG_WDTWPROTECT_REG(1), 0);
  ESP_LOGI("WDT", "IWDT stg0 %u -> %u", (unsigned)stg0, (unsigned)(stg0 * 10));
#endif
}
void gpioInit(){
  pinMode(BUTTON_ERASE , INPUT);
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  pinMode(TOUCH_XPT2046_CS, OUTPUT);
  digitalWrite(TOUCH_XPT2046_CS, HIGH);
  pinMode(A23S08_CS, OUTPUT);
  digitalWrite(A23S08_CS, HIGH);

#if defined(DISPLAY_43)
  /* GPIO43은 UART0 TX이자 DS1302 CE. 이 줄 이후 시리얼은 CE 파형을 깨뜨린다. */
  Serial.println("이제부터 RTC에 제어권을 넘깁니다.");
  Serial.flush();
  esp_log_level_set("*", ESP_LOG_NONE);
  Serial.end();
#endif
  rtcCeGpioTakeFromUart0();

  pinMode(SERIAL_TX2 , OUTPUT);
  pinMode(SERIAL_RX2 , INPUT);
}
void setup()
{
  Serial.begin(BAUDRATEDEF);
  stretchRgbIntWdt();
  gpioInit();
  wifiPrepareBeforeBle();

  digitalWrite(TOUCH_XPT2046_CS, HIGH);
  digitalWrite(A23S08_CS, HIGH);
  Mcp23s08_begin(A23S08_CS, 1000000);
  Mcp23s08_RtcCe(false);
  printf("MCP OLAT=0x%02X GPIO=0x%02X CE43=%d\r\n",
         (unsigned)Mcp23s08_readReg(0x0A), (unsigned)Mcp23s08_readReg(0x09),
         (int)digitalRead(RTC_CE_GPIO));
  Mcp23s08_bootBeep(1);
  initSetRtc();
  spiBusRecoverFromBitBang();
  rtcCeGpioLow();
  if (!g_rtcBootSynced) {
    printf("RTC late resync (before touch_init)\r\n");
    vTaskDelay(pdMS_TO_TICKS(100));
    initSetRtc();
    spiBusRecoverFromBitBang();
    rtcCeGpioLow();
  }
  Mcp23s08_bootBeep(2);
  printf("after RTC MCP OLAT=0x%02X CE43=%d\r\n",
         (unsigned)Mcp23s08_readReg(0x0A), (int)digitalRead(RTC_CE_GPIO));

  initialEEPROM();
  /* 0 = 화면 꺼짐 없음. 예전에는 10분 미만을 10으로 덮어 0이 유지되지 않았다. */

  // Init Display
  // Add
  displayCreate();
  gfx->begin();
  gfx->fillScreen(BLACK);


  ledcSetup(0, 300, 8);
  ledcAttachPin(TFT_BL, 0);
  if(nvsSystemEEPRom.lcdBright < 80) nvsSystemEEPRom.lcdBright = 80;
  if(nvsSystemEEPRom.lcdBright > 255) nvsSystemEEPRom.lcdBright = 255;
  ledcWrite(0, nvsSystemEEPRom.lcdBright);

  gfx->fillScreen(RED); delay(100);
  gfx->fillScreen(GREEN); delay(100);
  gfx->fillScreen(BLUE); delay(100);
  gfx->fillScreen(BLACK); delay(100);
  gfx->setCursor(0, 10);


  touch_init();

  // update를 할것인지 확인한다. 이것은 bluetooth에서 설정한다.
  if(nvsSystemEEPRom.isUpdate)
  {
    nvsSystemEEPRom.isUpdate = false;
    EEPROM.writeBytes(1, (const byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
    EEPROM.writeByte(sizeof(nvsSystemSet_t) + 1, 0x55);
    EEPROM.commit();
    Serial.println("Update....");
    gfx->println("Update....");
    //#ifdef USEWIFI
    wifiOTAsetup(true);
    //#endif
  }
  else
  {
    Serial.println("No Update....");
  }

  // 로그파일 기록을 위해 파일시스템을 초기화 한다.
  lsFile.littleFsInitFast(0);
  lsFile.setOutputStream(&Serial);

  upslogEvent.getFileSize();
//모드버스를 위한 serial2 포트를 초기화 한다
  Serial.println("Setup done");
  Serial2.begin(nvsSystemEEPRom.BAUDRATE, SERIAL_8N1, SERIAL_RX2  /* RX */, SERIAL_TX2  /* TX*/);
  Serial2.println("Serial 1 started");
  //ts.penirqControl(0xD0);
  modbusSetup();
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
  //touch_init();

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
    // 터치 캘리브레이션을 위한 드라이버 초기화


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
    myui_scrAlarm1_lock_scroll();
    lv_obj_scroll_to_view_recursive(ui_alarmTextArea,LV_ANIM_OFF);
    lv_obj_scroll_to_view_recursive(ui_eventTextArea,LV_ANIM_OFF);
    lv_tabview_set_act(ui_TabView2,1,LV_ANIM_OFF); 
    lv_tabview_set_act(ui_TabView2,0,LV_ANIM_OFF); 
    // lv_obj_add_flag(ui_TabView2,LV_OBJ_FLAG_GESTURE_BUBBLE);
    myui_MainScreen_screen_init();
    // setTime();

    /* System setting screen*/
    setTimeText();
    lv_disp_load_scr(ui_InitScreen);
    calibrationTouchInit();
    uiBindLoopTask();
    /* 첫 페인트(배경 PNG·폰트=플래시) 동안 BLE/모드버스 태스크를 켜지 않는다. */
    for (int i = 0; i < 5; ++i) {
      lv_timer_handler();
      vTaskDelay(pdMS_TO_TICKS(30));
    }
  }
  wifiPrepareBeforeBle();
  bleSetup();
  // Modbus는 내부적으로 task를 사용하고 있다.
  xTaskCreatePinnedToCore(systemControllTask, "systemControllTask", 5000, NULL, 1, h_pxsystemControllTask, 0);  

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
  //     delay(200);
  //     Serial.println("buzzer test");
  // }
};
static int interval = 1000;
static unsigned long previous300mills = 0;
static unsigned long previous1000mills = 0;
static unsigned long previous10000mills = 0;
static int everySecondInterval = 1000;
static int every300ms = 300;

static unsigned long previous500mills = 0;
static int every500ms = 500;
static unsigned long now;
uint16_t incTime = 0;
// int modbusEventSendLoop(int timeout);
// int modbusEventGetLoop();
void showMessageLabel(const char *message);
void drainPendingUiUpdates(void);
//extern lv_obj_t * ui_MainScreen;
uint16_t pressedResetButton=0;
void loop()
{
  void *parameters;
  bleCheck();
  now = millis();
  drainPendingUiUpdates();
  #ifndef DONOTUSECOMM
  if(modbusErrorCounter>2)
  {
    if (modbusErrorCounter % 5 == 0)
    {
      //ESP_LOGW("MODBUS", "%d Error %s count %d", millis(), _("Comm_Error"), modbusErrorCounter);
      showMessageLabel(_("Comm_Error"));
      vTaskDelay(10);
    }
  }
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
          esp_nvs_tc_reset_cb();
          ESP_LOGI("IO","Now On file format...Do not Turn Off system");
          lsFile.rm("eventLog.hex");
          upslogEvent.getFileSize();
          upslogEvent.readCurrentLogExt(CURRENTLOG,true);
          nvsSystemEEPRom.systemLedOffTime = 10;
          nvsSystemEEPRom.lcdBright= 255;
          EEPROM.writeBytes(1, (const byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
          EEPROM.commit();
          //upsLog upslogEvent("/spiffs/eventLog.hex", EVENT_TYPE);
          // upslogEvent.init();
          // ui_init();
          pressedResetButton =0;
          ESP_LOGI("IO","Turn Off system in 3 seconds");
          delay(3000);
          esp_restart();
        }
      }
    }
    else{
      pressedResetButton =0;
    }

    if (nvsSystemEEPRom.systemLedOffTime != 0 && lcdOntime >= nvsSystemEEPRom.systemLedOffTime*60) // lv_led_off(led);
    {
      _ui_screen_change(&ui_InitScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, &ui_InitScreen_screen_init);
      ledcWrite(0, 0);
    }
  }
  if(now - previous10000mills > 3000)
  {
    previous10000mills = now;
    /* 설정 화면에서는 터치 SPI와 RTC 비트뱅이 겹치지 않게 주기적 RTC 읽기를 하지 않음 */
    if (lv_scr_act() != ui_SettingScreen)
    {
      //setRtc(false, nullptr);
    }
  }
  lv_timer_handler(); /* let the GUI do its work */
  vTaskDelay(10);     // Every 50ms
}

int isReceiveEventData = 0;
QueueHandle_t modbusCmdQueue;

void systemControllTask(void *parameter)
{
  modbusCmdQueue = xQueueCreate(10, sizeof(ModbusCommand));
  for (;;)
  {
    /* 설정 화면도 'A' 폴링을 유지한다. 키보드가 열린 동안만
     * drainPendingUiUpdates()가 설정 칸 갱신을 미룬다. */
    isReceiveEventData = modbusEventSendLoop(100);
    if(isReceiveEventData == 'E'){
      GetSetEventData();
    }
    vTaskDelay(300);
  };
}