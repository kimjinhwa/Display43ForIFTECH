/**
 * Shared SPI: MCP23S08 CS=GPIO17, XPT2046 CS=GPIO38.
 * Only one CS is LOW at a time.
 */
#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "driver/gpio.h"

#ifndef SPIBUS_MCP_CS
#define SPIBUS_MCP_CS 17
#endif
#ifndef SPIBUS_TOUCH_CS
#define SPIBUS_TOUCH_CS 38
#endif
#ifndef SPIBUS_SCK
#define SPIBUS_SCK 12
#define SPIBUS_MISO 13
#define SPIBUS_MOSI 11
#endif
#ifndef SPIBUS_RTC_CE
#define SPIBUS_RTC_CE 43
#endif

static inline bool &spiBusStartedFlag(void)
{
  static bool started;
  return started;
}

static inline void spiBusHoldCsHigh(void)
{
  pinMode(SPIBUS_MCP_CS, OUTPUT);
  pinMode(SPIBUS_TOUCH_CS, OUTPUT);
  digitalWrite(SPIBUS_MCP_CS, HIGH);
  digitalWrite(SPIBUS_TOUCH_CS, HIGH);
}

/** RTC CE가 High일 수 있으므로 SCK를 돌리지 않는다. MOSI는 ESP가 잡는다. */
static inline void spiBusParkSharedLines(void)
{
  pinMode(SPIBUS_SCK, OUTPUT);
  pinMode(SPIBUS_MOSI, OUTPUT);
  digitalWrite(SPIBUS_SCK, LOW);
  digitalWrite(SPIBUS_MOSI, LOW);
}

/** CE가 이미 Low일 때만 MCP 상태머신 비우기. */
static inline void spiBusFlushMcp(void)
{
  spiBusHoldCsHigh();
  spiBusParkSharedLines();
  for (int i = 0; i < 32; i++) {
    digitalWrite(SPIBUS_SCK, HIGH);
    delayMicroseconds(2);
    digitalWrite(SPIBUS_SCK, LOW);
    delayMicroseconds(2);
  }
}

static inline void spiBusCsInit(void)
{
  spiBusHoldCsHigh();
}

static inline void spiBusEnsureSpi(void)
{
  spiBusHoldCsHigh();
  if (!spiBusStartedFlag()) {
    SPI.begin(SPIBUS_SCK, SPIBUS_MISO, SPIBUS_MOSI, -1);
    spiBusStartedFlag() = true;
  }
  digitalWrite(SPIBUS_TOUCH_CS, HIGH);
  digitalWrite(SPIBUS_MCP_CS, HIGH);
}

/** MCP CS가 SPI.end() 동안 떠서 슬레이브가 잠기지 않게 고정한다. */
static inline void spiBusEndKeepMcpCs(void)
{
  spiBusHoldCsHigh();
  gpio_hold_en((gpio_num_t)SPIBUS_MCP_CS);
  SPI.end();
  spiBusStartedFlag() = false;
  spiBusHoldCsHigh();
  gpio_hold_dis((gpio_num_t)SPIBUS_MCP_CS);
  spiBusHoldCsHigh();
}

/** RTC 비트뱅 전에만. MCP CS는 HIGH로 붙잡아 둔다. */
static inline void spiBusReleaseToBitBang(void)
{
  spiBusEndKeepMcpCs();
}

/** 비트뱅 후 SPI 재연결. RTC CE가 아직 High일 수 있어 SCK flush 금지. */
static inline void spiBusRecoverFromBitBang(void)
{
  spiBusHoldCsHigh();
  pinMode(SPIBUS_RTC_CE, OUTPUT);
  digitalWrite(SPIBUS_RTC_CE, LOW);
  if (spiBusStartedFlag())
    spiBusEndKeepMcpCs();
  spiBusHoldCsHigh();
  pinMode(SPIBUS_RTC_CE, OUTPUT);
  digitalWrite(SPIBUS_RTC_CE, LOW);
  SPI.begin(SPIBUS_SCK, SPIBUS_MISO, SPIBUS_MOSI, -1);
  spiBusStartedFlag() = true;
  spiBusHoldCsHigh();
}

/** Both chips deselected. */
static inline void spiBusIdle(void)
{
  digitalWrite(SPIBUS_MCP_CS, HIGH);
  digitalWrite(SPIBUS_TOUCH_CS, HIGH);
}

static inline void spiBusSelectMcp(void)
{
  digitalWrite(SPIBUS_TOUCH_CS, HIGH);
  digitalWrite(SPIBUS_MCP_CS, LOW);
  delayMicroseconds(2);
}

static inline void spiBusReleaseMcp(void)
{
  digitalWrite(SPIBUS_MCP_CS, HIGH);
}

static inline void spiBusSelectTouch(void)
{
  digitalWrite(SPIBUS_MCP_CS, HIGH);
  digitalWrite(SPIBUS_TOUCH_CS, LOW);
  delayMicroseconds(2);
}

static inline void spiBusReleaseTouch(void)
{
  digitalWrite(SPIBUS_TOUCH_CS, HIGH);
}
