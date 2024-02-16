#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <TFT_eSPI.h>
#include "ui.h"
#include "upsLog.h"

#include <EEPROM.h>
//#include "SerialProtocalParse.h"
#include "main.h"
#include "wifiOTA.h"
#include "lv_i18n.h"
#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define TFT_BL 2
//#define BRIGHT  80 

nvsSystemSet ipAddress_struct;
static uint32_t screenWidth;
static uint32_t screenHeight;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;
static unsigned long last_ms;
//static lv_obj_t *led;
uint16_t lcdOntime=0;
//#define DISPLAY_7

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
void my_print(const char * buf)
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
void touchTest(int loopCount){
  uint16_t x, y;
  uint8_t  z;
  TS_Point p;
  do{
    if (ts.touched())
    {
      ts.readData(&x, &y, &z);
      TS_Point p = ts.getPoint();
      Serial.printf("\n%d,%d,%d",x,y,z);
      Serial.printf("\np.x %d,p.y %d",p.x,p.y);
      touch_last_x = map(p.x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, gfx->width() - 1);
      touch_last_y = map(p.y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, gfx->height() - 1);
      Serial.printf("\np.x %d,p.y %d",touch_last_x ,touch_last_y );
    }
  }while(loopCount);
}
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
  uint16_t x, y;
  uint8_t z;
  if (touch_has_signal())
  {
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
      ledcWrite(0, ipAddress_struct.bright );
      lcdOntime = 0;
      Serial.print("Data xx ");
      Serial.println(data->point.x);
      Serial.print("Data yy ");
      Serial.println(data->point.y);
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

void setMemoryDataToLCD(){
}
void setTime(){
  tm nowTime;
  timeval tVal;
  nowTime.tm_year = 124;
  nowTime.tm_mon = 2;
  nowTime.tm_mday = 7;
  nowTime.tm_hour = 17;
  nowTime.tm_min = 6;
  nowTime.tm_sec = 6;

  time_t now = mktime(&nowTime);
  tVal.tv_sec = now;
  settimeofday(&tVal,NULL);
}
void showSystemDateTime(){
  tm nowTime;
  getLocalTime(&nowTime); 
  String date;
  date  = nowTime.tm_year + ".  ";
  date  += nowTime.tm_mon + ".  ";
  date  += nowTime.tm_mday+ "  ";
  char buf[80];
  char *dayofweek[7] = LV_CALENDAR_DEFAULT_DAY_NAMES;
  sprintf(buf,"%d-%02d-%02d %s",
     nowTime.tm_year+1900,nowTime.tm_mon,nowTime.tm_mday,dayofweek[nowTime.tm_wday]);
  lv_label_set_text(ui_lblDate,buf);

  

  sprintf(buf,"%02d:%02d:%02d",
     nowTime.tm_hour,nowTime.tm_min,nowTime.tm_sec);
  lv_label_set_text(ui_lblTime,buf);

  sprintf(buf,"%d-%02d-%02d %02d:%02d:%02d",
     nowTime.tm_year+1900,nowTime.tm_mon,nowTime.tm_mday,
     nowTime.tm_hour,nowTime.tm_min,nowTime.tm_sec);
  lv_label_set_text(ui_lblDateTime,buf);
  //Serial.printf("\n%s",buf);
  // Serial.printf("\nNow Time is %d-%d-%d %d:%d:%d",
  //    nowTime.tm_year+1900,nowTime.tm_mon,nowTime.tm_mday,
  //    nowTime.tm_hour,nowTime.tm_min,nowTime.tm_sec);

  sprintf(buf,"%d",rand()%(4)+90 );
  lv_label_set_text(ui_lbaBatCapacity ,buf);
  sprintf(buf,"%d",rand()%(10)+20 );
  lv_label_set_text(ui_lblLoadCapacity,buf);

}
void your_tc_finish_cb(
        lv_event_t *event
    ) {
    /*
        Load the application
    */
   loop();
    //your_start_application(); /* Implement this */
}

    
void touchCalibrationInit()
{
}
void setup()
{
  Serial.begin(BAUDRATEDEF);
  Serial1.begin(BAUDRATEDEF,134217756U,18,17);
  EEPROM.begin(120);
  //if (EEPROM.read(0) != 0x55)
  {
    ipAddress_struct.bright = 80;
    ipAddress_struct.LED_OFF_TIME= 600;
    ipAddress_struct.IPADDRESS = (uint32_t)IPAddress(192, 168, 0, 57);
    ipAddress_struct.GATEWAY = (uint32_t)IPAddress(192, 168, 0, 1);
    ipAddress_struct.SUBNETMASK = (uint32_t)IPAddress(255, 255, 255, 0);
    ipAddress_struct.WEBSOCKETSERVER = (uint32_t)IPAddress(192, 168, 0, 57);
    ipAddress_struct.DNS1 = (uint32_t)IPAddress(8, 8, 8, 8);
    ipAddress_struct.DNS2 = (uint32_t)IPAddress(164, 124, 101, 2);
    ipAddress_struct.WEBSERVERPORT = 81;
    ipAddress_struct.NTP_1 = (uint32_t)IPAddress(203, 248, 240, 140); //(203, 248, 240, 140);
    ipAddress_struct.NTP_2 = (uint32_t)IPAddress(13, 209, 84, 50);
    ipAddress_struct.ntpuse = false;

    ipAddress_struct.HighVoltage = 36500;
    ipAddress_struct.LowVoltage = 26000;
    ipAddress_struct.HighImp = 8000;
    ipAddress_struct.HighTemp = 70;
    ipAddress_struct.alarmSetStatus = 0;
    strncpy(ipAddress_struct.deviceName,"BAT RACK1",9);

    EEPROM.writeByte(0,0x55);
    EEPROM.commit();
    EEPROM.writeBytes(1, (const byte *)&ipAddress_struct, sizeof(nvsSystemSet));
    EEPROM.commit();
    Serial.println("Memory Initialized first booting....");
  }
  ipAddress_struct.HighVoltage = 0;
  ipAddress_struct.LowVoltage = 0;
  ipAddress_struct.HighImp = 0;
  ipAddress_struct.HighTemp = 0;
  EEPROM.readBytes(1, (byte *)&ipAddress_struct, sizeof(ipAddress_struct));
  Serial.printf("\ninit data \n%d %d %d %d",ipAddress_struct.HighVoltage,ipAddress_struct.LowVoltage,ipAddress_struct.HighTemp,ipAddress_struct.HighImp);
  // while (!Serial);
  Serial.println("LVGL Benchmark Demo");

  // Init Display
  // Add
  gfx->begin();
  gfx->fillScreen(BLACK);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  ledcSetup(0, 300, 8);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, ipAddress_struct.bright); /* Screen brightness can be modified by adjusting this parameter. (0-255) */

  gfx->fillScreen(RED);
  delay(500);
  gfx->fillScreen(GREEN);
  delay(500);
  gfx->fillScreen(BLUE);
  delay(500);
  gfx->fillScreen(BLACK);
  delay(500);
  
  lv_i18n_init(lv_i18n_language_pack);
  lv_init();

  //led = lv_led_create(lv_scr_act());

  // Init touch device
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

    lv_i18n_set_locale("ko-KR");

    upsLog upslog;
    for (int i = 0; i < 16; i++)
      Serial.printf("\n%s", upslog.converter_status_eng[i]);
    for (int i = 0; i < 16; i++)
      Serial.printf("\n%s", upslog.operation_falut_eng[i]);

    for (int i = 0; i < 16; i++)
      Serial.printf("\n%s", upslog.converter_status_eng[i]);
    for (int i = 0; i < 16; i++)
      Serial.printf("\n%s", upslog.operation_falut_eng[i]);

    ui_init();
    touchCalibrationInit();
    setTime();

    /* System setting screen*/
    tm nowTime;
    getLocalTime(&nowTime);
    lv_label_set_text(ui_lblYearSet, String(nowTime.tm_year + 1900).c_str());
    lv_label_set_text(ui_lblMonthSet, String(nowTime.tm_mon).c_str());
    lv_label_set_text(ui_lblDaySet, String(nowTime.tm_mday).c_str());
    lv_label_set_text(ui_lblSetHour, String(nowTime.tm_hour).c_str());
    lv_label_set_text(ui_lblSetMinute, String(nowTime.tm_min).c_str());
    lv_label_set_text(ui_lblSetSecond, String(nowTime.tm_sec).c_str());

    lv_slider_set_value(ui_SliderYear, nowTime.tm_year + 1900, LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderMonth, nowTime.tm_mon, LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderDay, nowTime.tm_mday, LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderHour, nowTime.tm_hour, LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderMinute, nowTime.tm_min, LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderSecond, nowTime.tm_sec, LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderBrightness, ipAddress_struct.bright, LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderLedOffTime, ipAddress_struct.LED_OFF_TIME, LV_ANIM_OFF);
    lv_label_set_text(ui_lblOffTime, String(ipAddress_struct.LED_OFF_TIME).c_str());
    // lv_label_set_text(ui_NiceLabel,_("RunAniButton"));
    Serial.println("Setup done");
  }
#ifdef USEWIFI
  wifiOTAsetup() ;
#endif
  EEPROM.readBytes(1, (byte *)&ipAddress_struct, sizeof(ipAddress_struct));
  setMemoryDataToLCD();
};
static int interval = 1000;
static unsigned long previousmills = 0;
static int everySecondInterval = 1000;
static int every100ms= 100;
static unsigned long now;
unsigned long incTime=1;
void loop()
{
  void *parameters;
  wifiOtaloop();
  now = millis();
  if ((now - previousmills > every100ms))
  {
    previousmills = now;
    incTime++;
  }
  if ((incTime % 10) == 0) // 100*10 = 1S
  {
    lcdOntime++;
    incTime++;
    showSystemDateTime();
    if(lcdOntime >= ipAddress_struct.LED_OFF_TIME) //lv_led_off(led);
      ledcWrite(0,0);
  }
  //touchTest(0);
  lv_timer_handler(); /* let the GUI do its work */
  vTaskDelay(5);
}
  // struct tm tm;
  // tm.tm_year = 2023 - 1900;
  // tm.tm_mon = 11;
  // tm.tm_mday = 13;
  // tm.tm_hour = 15;
  // tm.tm_min = 13;
  // tm.tm_sec = 00;
  // struct timeval tv;
  // tv.tv_sec = mktime(&tm);
  // tv.tv_usec = 0;
  // settimeofday(&tv, NULL);