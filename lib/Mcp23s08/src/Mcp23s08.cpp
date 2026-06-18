#include "Mcp23s08.h"

#include <SPI.h>

static constexpr uint8_t kOpWrite = 0x40u;
static constexpr uint8_t kOpRead = 0x41u;
static constexpr uint8_t kRegIODIR = 0x00u;
static constexpr uint8_t kRegOLAT = 0x0Au;
static constexpr uint8_t kRtcCeMask = 0x01u;   /* GP0 */
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
  pinMode(s_mcpCs, OUTPUT);
  digitalWrite(s_mcpCs, HIGH);
  s_olatShadow = 0x00u;
  s_rtcCeOn = false;
  s_buzzerOn = false;
  s_ready = true;
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
  digitalWrite(s_mcpCs, LOW);
  SPI.beginTransaction(SPISettings(s_spiHz, MSBFIRST, SPI_MODE0));
  SPI.transfer(kOpWrite);
  SPI.transfer(reg);
  SPI.transfer(val);
  SPI.endTransaction();
  digitalWrite(s_mcpCs, HIGH);
}

uint8_t Mcp23s08_readReg(uint8_t reg)
{
  if (!s_ready)
    return 0;
  digitalWrite(s_mcpCs, LOW);
  SPI.beginTransaction(SPISettings(s_spiHz, MSBFIRST, SPI_MODE0));
  SPI.transfer(kOpRead);
  SPI.transfer(reg);
  uint8_t val = SPI.transfer(0x00u);
  SPI.endTransaction();
  digitalWrite(s_mcpCs, HIGH);
  return val;
}

void Mcp23s08_initOutputsAll(void)
{
  Mcp23s08_writeReg(kRegIODIR, 0x00u);
}

void Mcp23s08_RtcCe(bool high)
{
  s_rtcCeOn = high;
  if (high)
    s_olatShadow |= kRtcCeMask;
  else
    s_olatShadow &= (uint8_t)~kRtcCeMask;
  Mcp23s08_writeReg(kRegOLAT, s_olatShadow);
}

bool Mcp23s08_RtcCeIsHigh(void)
{
  return s_rtcCeOn;
}

void Mcp23s08_BuzzerControl(bool on)
{
  s_buzzerOn = on;
  if (on)
    s_olatShadow |= kBuzzerMask;
  else
    s_olatShadow &= (uint8_t)~kBuzzerMask;
  Mcp23s08_writeReg(kRegOLAT, s_olatShadow);
}

void Mcp23s08_BuzzerToggle(void)
{
  s_buzzerOn = !s_buzzerOn;
  s_olatShadow ^= kBuzzerMask;
  Mcp23s08_writeReg(kRegOLAT, s_olatShadow);
}

bool Mcp23s08_BuzzerIsOn(void)
{
  return s_buzzerOn;
}

void Mcp23s08_setOutput(uint8_t pattern)
{
  s_olatShadow = pattern;
  syncShadowFlags();
  Mcp23s08_writeReg(kRegOLAT, pattern);
}
