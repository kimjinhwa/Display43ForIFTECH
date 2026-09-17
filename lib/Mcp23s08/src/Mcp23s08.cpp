#include "Mcp23s08.h"

#include <SPI.h>
#include "../../../include/spiBusCs.h"

static constexpr uint8_t kOpWrite = 0x40u;
static constexpr uint8_t kOpRead = 0x41u;
static constexpr uint8_t kRegIODIR = 0x00u;
static constexpr uint8_t kRegOLAT = 0x0Au;
static constexpr uint8_t kRtcCeMask = 0x01u;   /* GP0 unused, keep Low */
static constexpr uint8_t kBuzzerMask = 0x02u; /* GP1 */

static uint8_t s_mcpCs;
static uint32_t s_spiHz = 1000000u;
static bool s_ready = false;
static uint8_t s_olatShadow = 0x00u;
static bool s_rtcCeOn = false;
static bool s_buzzerOn = false;

static void syncShadowFlags(void)
{
  s_rtcCeOn = (s_olatShadow & kRtcCeMask) != 0;
  s_buzzerOn = (s_olatShadow & kBuzzerMask) != 0;
}

void Mcp23s08_begin(uint8_t mcpCsPin, uint32_t spiClockHz)
{
  s_mcpCs = mcpCsPin;
  s_spiHz = spiClockHz ? spiClockHz : 1000000u;
  spiBusHoldCsHigh();
  spiBusEnsureSpi();
  s_olatShadow = 0x00u;
  s_rtcCeOn = false;
  s_buzzerOn = false;
  s_ready = true;
  Mcp23s08_writeReg(kRegIODIR, 0x00u);
  Mcp23s08_writeReg(kRegOLAT, 0x00u);
}

void Mcp23s08_end(void)
{
  if (!s_ready)
    return;
  digitalWrite(s_mcpCs, HIGH);
}

void Mcp23s08_writeReg(uint8_t reg, uint8_t val)
{
  if (!s_ready)
    return;
  spiBusEnsureSpi();
  digitalWrite(SPIBUS_TOUCH_CS, HIGH);
  digitalWrite(s_mcpCs, HIGH);
  digitalWrite(s_mcpCs, LOW);
  SPI.beginTransaction(SPISettings(s_spiHz, MSBFIRST, SPI_MODE0));
  SPI.transfer(kOpWrite);
  SPI.transfer(reg);
  SPI.transfer(val);
  SPI.endTransaction();
  digitalWrite(s_mcpCs, HIGH);
  digitalWrite(SPIBUS_TOUCH_CS, HIGH);
}

uint8_t Mcp23s08_readReg(uint8_t reg)
{
  if (!s_ready)
    return 0;
  spiBusEnsureSpi();
  digitalWrite(SPIBUS_TOUCH_CS, HIGH);
  digitalWrite(s_mcpCs, HIGH);
  digitalWrite(s_mcpCs, LOW);
  SPI.beginTransaction(SPISettings(s_spiHz, MSBFIRST, SPI_MODE0));
  SPI.transfer(kOpRead);
  SPI.transfer(reg);
  uint8_t val = SPI.transfer(0x00u);
  SPI.endTransaction();
  digitalWrite(s_mcpCs, HIGH);
  digitalWrite(SPIBUS_TOUCH_CS, HIGH);
  return val;
}

void Mcp23s08_initOutputsAll(void)
{
  Mcp23s08_writeReg(kRegIODIR, 0x00u);
}

void Mcp23s08_RtcCe(bool high)
{
  /* CE는 GPIO43. GP0는 항상 Low. */
  (void)high;
  s_rtcCeOn = false;
  s_olatShadow &= (uint8_t)~kRtcCeMask;
  Mcp23s08_writeReg(kRegOLAT, s_olatShadow);
}

bool Mcp23s08_RtcCeIsHigh(void)
{
  return s_rtcCeOn;
}

uint8_t Mcp23s08_forceRtcCeLow(void)
{
  s_rtcCeOn = false;
  s_olatShadow &= (uint8_t)~kRtcCeMask;
  Mcp23s08_writeReg(kRegIODIR, 0x00u);
  Mcp23s08_writeReg(kRegOLAT, s_olatShadow);
  return Mcp23s08_readReg(kRegOLAT);
}

void Mcp23s08_BuzzerControl(bool on)
{
#if !BUZZER
  on = false;
#endif
  const bool wantBit = on;
  const bool haveBit = (s_olatShadow & kBuzzerMask) != 0;
  if (s_buzzerOn == wantBit && haveBit == wantBit)
    return;

  s_buzzerOn = wantBit;
  if (wantBit)
    s_olatShadow |= kBuzzerMask;
  else
    s_olatShadow &= (uint8_t)~kBuzzerMask;
  Mcp23s08_writeReg(kRegOLAT, s_olatShadow);
}

void Mcp23s08_BuzzerToggle(void)
{
#if BUZZER
  s_buzzerOn = !s_buzzerOn;
  s_olatShadow ^= kBuzzerMask;
  Mcp23s08_writeReg(kRegOLAT, s_olatShadow);
#endif
}

bool Mcp23s08_BuzzerIsOn(void)
{
  return s_buzzerOn;
}

void Mcp23s08_setOutput(uint8_t pattern)
{
#if !BUZZER
  pattern &= (uint8_t)~kBuzzerMask;
#endif
  pattern &= (uint8_t)~kRtcCeMask;
  s_olatShadow = pattern;
  syncShadowFlags();
  Mcp23s08_writeReg(kRegOLAT, pattern);
}

void Mcp23s08_bootBeep(int times)
{
  if (!s_ready || times <= 0)
    return;
  Mcp23s08_writeReg(kRegIODIR, 0x00u);
  for (int i = 0; i < times; i++) {
    s_olatShadow = kBuzzerMask;
    s_rtcCeOn = false;
    s_buzzerOn = true;
    Mcp23s08_writeReg(kRegOLAT, s_olatShadow);
    Serial.printf("  beep OLAT wr=0x02 rd=0x%02X\r\n",
                  (unsigned)Mcp23s08_readReg(kRegOLAT));
    delay(200);
    s_olatShadow = 0x00u;
    s_buzzerOn = false;
    Mcp23s08_writeReg(kRegOLAT, s_olatShadow);
    delay(150);
  }
  s_olatShadow = 0x00u;
  s_rtcCeOn = false;
  s_buzzerOn = false;
  Mcp23s08_writeReg(kRegOLAT, 0x00u);
}

void Mcp23s08_bootBeep3(void)
{
  Mcp23s08_bootBeep(3);
}
