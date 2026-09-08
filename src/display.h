#ifndef DISPLAY_HAL_H
#define DISPLAY_HAL_H

#include <Arduino_GFX_Library.h>

/** Panel driver — RGB(4.3) 또는 HSPI(3.2). env build_flags: -DDISPLAY_43 / -DDISPLAY_32 */
extern Arduino_GFX *gfx;

#if defined(DISPLAY_32)
#define DISPLAY_BL_PIN 27
#elif defined(DISPLAY_43)
#define DISPLAY_BL_PIN 2
#else
#error "Define DISPLAY_43 or DISPLAY_32 in platformio.ini build_flags"
#endif

/** Create panel + gfx. Call once in setup() before gfx->begin(). */
void displayCreate(void);

#endif
