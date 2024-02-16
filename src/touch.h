/*******************************************************************************
 * Touch libraries:
 * FT6X36: https://github.com/strange-v/FT6X36.git
 * GT911: https://github.com/TAMCTec/gt911-arduino.git
 * XPT2046: https://github.com/PaulStoffregen/XPT2046_Touchscreen.git
 ******************************************************************************/

/* uncomment for XPT2046 */
#define TOUCH_XPT2046
#define TOUCH_XPT2046_SCK 12
#define TOUCH_XPT2046_MISO 13
#define TOUCH_XPT2046_MOSI 11
#define TOUCH_XPT2046_CS 38
#define TOUCH_XPT2046_INT 18
#define TOUCH_XPT2046_ROTATION 0
#define TOUCH_MAP_X1 4200
#define TOUCH_MAP_X2 100
#define TOUCH_MAP_Y1 100
#define TOUCH_MAP_Y2 3700 //4000

int touch_last_x = 0, touch_last_y = 0;


#include <XPT2046_Touchscreen.h>
#include <SPI.h>
XPT2046_Touchscreen ts(TOUCH_XPT2046_CS, TOUCH_XPT2046_INT);
//T2046_Touchscreen ts(TOUCH_XPT2046_CS);  // Param 2 - NULL - No interrupts 


void touch_init()
{
  SPI.begin(TOUCH_XPT2046_SCK, TOUCH_XPT2046_MISO, TOUCH_XPT2046_MOSI, TOUCH_XPT2046_CS);
  ts.begin();
  ts.setRotation(TOUCH_XPT2046_ROTATION);
}

bool touch_has_signal()
{
  return ts.tirqTouched();
}

bool touch_touched()
{
  if (ts.touched())
  {
    TS_Point p = ts.getPoint();
    touch_last_x = map(p.x, TOUCH_MAP_X1, TOUCH_MAP_X2, 0, gfx->width() - 1);
    touch_last_y = map(p.y, TOUCH_MAP_Y1, TOUCH_MAP_Y2, 0, gfx->height() - 1);
    return true;
  }
  else
  {
    return false;
  }
  return false;
}

bool touch_released()
{
  return true;
}
