#include "display.h"

Arduino_GFX *gfx = nullptr;

#if defined(DISPLAY_43)

// ILI6485 RGB parallel 480x272 (Sunton / IFTECH 4.3")
static Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */);

static Arduino_RPi_DPI_RGBPanel *panel = new Arduino_RPi_DPI_RGBPanel(
    bus,
    480 /* width */, 0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 43 /* hsync_back_porch */,
    272 /* height */, 0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 12 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 6000000 /* prefer_speed */, true /* auto_flush */);

void displayCreate(void)
{
  gfx = panel;
}

#elif defined(DISPLAY_32)

// LCDWiki E32R32P 3.2" ST7789 IPS — SPI shared with XPT2046
// DC=2 CS=15 SCK=14 MOSI=13 MISO=12 RST=EN(board) BL=27
static Arduino_DataBus *bus = new Arduino_ESP32SPI(
    2 /* DC */, 15 /* CS */, 14 /* SCK */, 13 /* MOSI */, 12 /* MISO */, HSPI /* spi_num */);

static Arduino_ST7789 *panel = new Arduino_ST7789(
    bus,
    GFX_NOT_DEFINED /* RST: tied to ESP32 EN */,
    0 /* rotation */,
    true /* IPS — lcdwiki uses INVON 0x21 */,
    240 /* width */,
    320 /* height */);

void displayCreate(void)
{
  pinMode(DISPLAY_BL_PIN, OUTPUT);
  digitalWrite(DISPLAY_BL_PIN, HIGH);
  gfx = panel;
}

#endif
