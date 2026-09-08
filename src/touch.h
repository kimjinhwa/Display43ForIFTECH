/*******************************************************************************
 * Touch: XPT2046 (shared SPI with LCD on DISPLAY_32)
 ******************************************************************************/

#define TOUCH_XPT2046

#if defined(DISPLAY_32)
/* LCDWiki E32R32P — SPI shared with ST7789 */
#define TOUCH_XPT2046_SCK 14
#define TOUCH_XPT2046_MISO 12
#define TOUCH_XPT2046_MOSI 13
#define TOUCH_XPT2046_CS 33
#define TOUCH_XPT2046_INT 36
#define TOUCH_XPT2046_ROTATION 0
#define TOUCH_MAP_X1 4000
#define TOUCH_MAP_X2 100
#define TOUCH_MAP_Y1 150
#define TOUCH_MAP_Y2 3800
#elif defined(DISPLAY_43)
/* Sunton / IFTECH 4.3" */
#define TOUCH_XPT2046_SCK 12
#define TOUCH_XPT2046_MISO 13
#define TOUCH_XPT2046_MOSI 11
#define TOUCH_XPT2046_CS 38
#define TOUCH_XPT2046_INT 18
#define TOUCH_XPT2046_ROTATION 0
#define TOUCH_MAP_X1 4000
#define TOUCH_MAP_X2 100
#define TOUCH_MAP_Y1 150
#define TOUCH_MAP_Y2 3800
#else
#error "Define DISPLAY_43 or DISPLAY_32 for touch pins"
#endif

int touch_last_x = 0, touch_last_y = 0;

#include <XPT2046_Touchscreen.h>
#include <SPI.h>
#include "display.h"

XPT2046_Touchscreen ts(TOUCH_XPT2046_CS);

void touch_init()
{
  SPI.begin(TOUCH_XPT2046_SCK, TOUCH_XPT2046_MISO, TOUCH_XPT2046_MOSI, TOUCH_XPT2046_CS);
  ts.begin();
  ts.setRotation(TOUCH_XPT2046_ROTATION);
  ts.penirqControl(0x93);
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
  return false;
}

bool touch_released()
{
  return true;
}
