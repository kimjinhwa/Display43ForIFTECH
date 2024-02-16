## UPS용 4.3" Display
- ESP32 S3
- platformio
- 한국어 및 영어 지원
``` sh
 lv_i18n compile -t 'C:/DevWork/99.SquireLineWave/IFTECH_UI/UPS1P1P_V001/export/SquareLine_Project/libraries/ui/translations/*.yml' -o 'C:\DevWork\4.IFTechWork\1.UPS1P1P\Display4.3\lib\lv_i18n\src'
```
## LVGL의 lv_conf.h 파일 변경
- #define LV_MEM_CUSTOM 1 로 변경을 한다.  
  메모리문제를 해결한다.
- 화면 깜빡임 문제 해결
  ``` C
  Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
  bus,
  480 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
  272 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
  1 /* pclk_active_neg */, 9000000 /* prefer_speed */, true /* auto_flush */);
  ``` 
  Speed는 최대 1200000까지이다.
  
  
