# 아이에프텍 UPS용 4.3" 펌웨어 
## Basic Function & Enviroment 
- CPU : ESP32 S3 32BIT 2Code 16M Flash
- Platform : ESP32, Arduion,  platformio
- Language : 한국어 및 영어 지원
## Test순서 
  - RS232통신 테스트를 위해 USB to RS232 을 연결한다.
  - C:\DevWork\pythonWork\modBusSlave\rtuslave_example\dist\rtuslave_ex2.exe 를 실행하여 에뮬레이션을 연다.
  - 개발 보드에 USB를 연결하여 POWER를 인가한다.
  - 통신에러가 나오면 에뮬레이터의 포트를 OPEN하여 준다.

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
  
``` sh
 lv_i18n compile -t 'C:/DevWork/99.SquireLineWave/IFTECH_UI/UPS1P1P_V001/export/SquareLine_Project/libraries/ui/translations/*.yml' -o 'C:\DevWork\4.IFTechWork\1.UPS1P1P\Display4.3\lib\lv_i18n\src'
```
  
## 중요 변경사항.
- 회로의 패턴을 끊지 않고 처리하기 위하여 SerialPort의 IO를 19,20으로 변경하자. 
- 리셋버튼을 3초이상 누르고 있으면 로그파일을 초기하 한다.