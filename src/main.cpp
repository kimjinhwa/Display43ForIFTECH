#include <Arduino.h>
#include <lvgl.h>
#include <Arduino_GFX_Library.h>
#include <TFT_eSPI.h>
#include <RtcDS1302.h>
#include "mainGrobal.h"

#include <modbusRtu.h>

#include "ui.h"
#include "upsLog.h"

#include <EEPROM.h>
//#include "SerialProtocalParse.h"
#include "main.h"
#include "wifiOTA.h"
#include "lv_i18n.h"
#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define TFT_BL 2
#define RTCEN 19 
//#define BRIGHT  80 

TaskHandle_t *h_pxModbusTask;

ThreeWire myWire(MOSI, SCK /*12*/ , RTCEN ); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

nvsSystemSet_t nvsSystemEEPRom;


static uint32_t screenWidth;
static uint32_t screenHeight;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t *disp_draw_buf;
static lv_disp_drv_t disp_drv;
static unsigned long last_ms;
//static lv_obj_t *led;
uint16_t lcdOntime=0;
//#define DISPLAY_7
static char TAG[] = "main";


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
      ledcWrite(0, nvsSystemEEPRom.lcdBright );
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
void showSystemDateTime(){
  timeval tmv;
  gettimeofday(&tmv,NULL);
  RtcDateTime nowTime = RtcDateTime(tmv.tv_sec);
  //tm *nowTime= gmtime(&tmv.tv_sec); 
  String date;
  date  = nowTime.Year() + ".  ";
  date  += nowTime.Month()+ ".  ";
  date  += nowTime.Day()+ "  ";
  char buf[80];
  char dayofweek[7][3] = LV_CALENDAR_DEFAULT_DAY_NAMES;
  sprintf(buf,"%d-%02d-%02d %s",
     nowTime.Year(),nowTime.Month(),nowTime.Day(),dayofweek[nowTime.DayOfWeek()]);
  lv_label_set_text(ui_lblDate,buf);
  //ESP_LOGI(TAG,"%s",buf);

  sprintf(buf,"%02d:%02d:%02d",
     nowTime.Hour(),nowTime.Minute(),nowTime.Second());
  lv_label_set_text(ui_lblTime,buf);
  //ESP_LOGI(TAG,"%s",buf);

  sprintf(buf,"%d-%02d-%02d %02d:%02d:%02d",
     nowTime.Year(),nowTime.Month(),nowTime.Day(),
     nowTime.Hour(),nowTime.Minute(),nowTime.Second());
  lv_label_set_text(ui_lblDateTime,buf);

  sprintf(buf,"%d",rand()%(4)+90 );
  lv_label_set_text(ui_lbaBatCapacity ,buf);
  sprintf(buf,"%d",rand()%(10)+20 );
  lv_label_set_text(ui_lblLoadCapacity,buf);

}
void drawCursor(int16_t x, int16_t y, uint16_t color)
{
  int16_t r = 40;
  uint16_t w = 10;
  int16_t xx = x < r ? 0 : x - r;
  int16_t yy = y < r ? 0 : y - r;

  gfx->fillRect(x - w/2, yy, w, r * 2, color);
  gfx->fillRect(xx, y - w/2, r * 2, w, color);
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
  printf("\r\ncompiled time is %d/%d/%d %d:%d:%d\r\n",compiled.Year(),compiled.Month(),compiled.Day(),compiled.Hour(),compiled.Minute(),compiled.Second());
  if (!Rtc.IsDateTimeValid())
  {
    printf("RTC lost confidence in the DateTime!\r\n");
    //Rtc.SetDateTime(compiled);
  }
  else 
    printf("RTC available in the DateTime!\r\n");
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
  printf("\r\nnow time is %d/%d/%d %d:%d:%d\r\n",now.Year(),now.Month(),now.Day(),now.Hour(),now.Minute(),now.Second());
  if (now < compiled)
  {
    printf("\r\nSet data with compiled time"); //Rtc.SetDateTime(compiled);
  }
  struct timeval tmv;
  tmv.tv_sec = now.TotalSeconds();
  tmv.tv_usec = 0;
  //time_t toUnixTime = now.Unix32Time();
  //시스템의 시간도 같이 맞추어 준다.
  settimeofday(&tmv, NULL);
  gettimeofday(&tmv, NULL);
  now = RtcDateTime(tmv.tv_sec);
  //now = tmv.tv_sec;
  printf("\r\nset and reread time is %d/%d/%d %d:%d:%d\r\n",now.Year(),now.Month(),now.Day(),now.Hour(),now.Minute(),now.Second());

  myWire.end();
}
void setRtcNewTime(RtcDateTime rtc){
  digitalWrite(RTCEN , HIGH);
  // SPI.end();
  // Rtc.Begin();
  vTaskDelay(1);
  if (!Rtc.GetIsRunning())
  {
    printf("RTC was not actively running, starting now\r\n");
    Rtc.SetIsRunning(true);
  }
  else 
    printf("RTC was actively status running \r\n");
  Rtc.SetDateTime(rtc);
  // myWire.end();
  // SPI.begin(SCK,MISO,MOSI,RTCEN );
}
void setup()
{
  Serial.begin(BAUDRATEDEF);
  EEPROM.begin(sizeof(nvsSystemSet_t));
  //if (EEPROM.read(0) != 0x55)
  {
    nvsSystemEEPRom.systemLanguage= 0;  // default Hangul
    nvsSystemEEPRom.systemModusId= 1;
    nvsSystemEEPRom.lcdBright = 80;
    nvsSystemEEPRom.systemLedOffTime= 600;
    nvsSystemEEPRom.IPADDRESS = (uint32_t)IPAddress(192, 168, 0, 57);
    nvsSystemEEPRom.GATEWAY = (uint32_t)IPAddress(192, 168, 0, 1);
    nvsSystemEEPRom.SUBNETMASK = (uint32_t)IPAddress(255, 255, 255, 0);
    nvsSystemEEPRom.WEBSOCKETSERVER = (uint32_t)IPAddress(192, 168, 0, 57);
    nvsSystemEEPRom.WEBSERVERPORT = 81;
    nvsSystemEEPRom.BAUDRATE = 9600;
    nvsSystemEEPRom.alarmSetStatus = 0;
    nvsSystemEEPRom.systemModbusQInterval=300;
    nvsSystemEEPRom.operatingMode.sysOpModeBit.modbusMode = 0;//agent
    nvsSystemEEPRom.operatingMode.sysOpModeBit.useBlueTooth = 1;
    nvsSystemEEPRom.operatingMode.sysOpModeBit.useWiFi= 1;
    nvsSystemEEPRom.operatingMode.sysOpModeBit.useWebServer= 1;

    strncpy(nvsSystemEEPRom.deviceName,"UPS1P1P",9);

    EEPROM.writeByte(0,0x55);
    EEPROM.commit();
    EEPROM.writeBytes(1, (const byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
    EEPROM.commit();
    Serial.println("Memory Initialized first booting....");
  }
  EEPROM.readBytes(1, (byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
  // while (!Serial);
  Serial.println("Init RTC");
  setRtc();

  // Init Display
  // Add
  gfx->begin();
  gfx->fillScreen(BLACK);

  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  ledcSetup(0, 300, 8);
  ledcAttachPin(TFT_BL, 0);
  ledcWrite(0, nvsSystemEEPRom.lcdBright); /* Screen brightness can be modified by adjusting this parameter. (0-255) */

  gfx->fillScreen(RED);
  delay(500);
  gfx->fillScreen(GREEN);
  delay(500);
  gfx->fillScreen(BLUE);
  delay(500);
  gfx->fillScreen(BLACK);
  delay(500);

  touchCalibrationInit();
  
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

    // upsLog upslog;
    // for (int i = 0; i < 16; i++)
    //   Serial.printf("\n%s", upslog.converter_status_eng[i]);
    // for (int i = 0; i < 16; i++)
    //   Serial.printf("\n%s", upslog.operation_falut_eng[i]);

    // for (int i = 0; i < 16; i++)
    //   Serial.printf("\n%s", upslog.converter_status_eng[i]);
    // for (int i = 0; i < 16; i++)
    //   Serial.printf("\n%s", upslog.operation_falut_eng[i]);

    ui_init();
    //setTime();

    /* System setting screen*/
    timeval tmv;
    gettimeofday(&tmv, NULL);
    RtcDateTime nowTime = RtcDateTime(tmv.tv_sec);
    lv_label_set_text(ui_lblYearSet, String(nowTime.Year()).c_str());
    lv_label_set_text(ui_lblMonthSet, String(nowTime.Month()).c_str());
    lv_label_set_text(ui_lblDaySet, String(nowTime.Day()).c_str());
    lv_label_set_text(ui_lblSetHour, String(nowTime.Hour()).c_str());
    lv_label_set_text(ui_lblSetMinute, String(nowTime.Minute()).c_str());
    lv_label_set_text(ui_lblSetSecond, String(nowTime.Second()).c_str());

    lv_slider_set_value(ui_SliderYear, nowTime.Year(), LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderMonth, nowTime.Month(), LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderDay, nowTime.Day(), LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderHour, nowTime.Hour(), LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderMinute, nowTime.Minute(), LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderSecond, nowTime.Second(), LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderBrightness, nvsSystemEEPRom.lcdBright, LV_ANIM_OFF);
    lv_slider_set_value(ui_SliderLedOffTime, nvsSystemEEPRom.systemLedOffTime, LV_ANIM_OFF);
    lv_label_set_text(ui_lblOffTime, String(nvsSystemEEPRom.systemLedOffTime).c_str());
    // lv_label_set_text(ui_NiceLabel,_("RunAniButton"));
    Serial.println("Setup done");
    pinMode(18,INPUT);
    Serial1.begin(nvsSystemEEPRom.BAUDRATE,SERIAL_8N1 ,18 /* RX */,17 /* TX*/);
    Serial1.println("Serial 1 started");
    modbusSetup();
    Serial.println("modbus started");
    //Modbus는 내부적으로 task를 사용하고 있다.
    //xTaskCreate(modbusTask,"modbusTask",5000,NULL,1,h_pxModbusTask); 
  }
#ifdef USEWIFI
  wifiOTAsetup() ;
#endif
  EEPROM.readBytes(1, (byte *)&nvsSystemEEPRom, sizeof(nvsSystemEEPRom));
  setMemoryDataToLCD();
};
static int interval = 1000;
static unsigned long previous100mills = 0;
static unsigned long previous1000mills = 0;
static int everySecondInterval = 1000;
static int every100ms= 100;
static unsigned long now;
unsigned long incTime=1;
void loop()
{
  void *parameters;
  wifiOtaloop();
  now = millis();
  if ((now - previous100mills > every100ms))
  {
    previous100mills = now;
  }
  if ((now - previous1000mills > everySecondInterval ))
  {
    previous1000mills = now;
    incTime++;
    lcdOntime++;
    showSystemDateTime();
    if(lcdOntime >= nvsSystemEEPRom.systemLedOffTime) //lv_led_off(led);
      ledcWrite(0,0);
  }

  lv_timer_handler(); /* let the GUI do its work */
  vTaskDelay(50);  // Every 50ms
}



  //touchTest(0);
  // while(1)
  // if(Serial1.available()) { 
  //   //Serial.printf(" %02x",Serial1.read());
  //   Serial1.write(Serial1.read());
  // }