#pragma once

#include <Arduino.h>
#include "spiBusCs.h"

#ifndef RTC_CE_GPIO
#define RTC_CE_GPIO 43
#endif

/** DS1302 3-wire. CE=GPIO43 (UART0 TX). CE는 항상 푸시풀, 유휴 Low. */
class RtcCeGpioWire
{
public:
  RtcCeGpioWire(uint8_t io, uint8_t clk, uint8_t ce)
      : _io(io), _clk(clk), _ce(ce) {}

  void begin() { holdCeLow(); }
  void end() { holdCeLow(); }

  void beginTransmission(uint8_t command)
  {
    spiBusHoldCsHigh();
    holdCeLow();
    pinMode(_clk, OUTPUT);
    digitalWrite(_clk, LOW);
    pinMode(_io, OUTPUT);
    digitalWrite(_ce, HIGH);
    delayMicroseconds(8);
    write(command, (command & 0x01) != 0);
  }

  void endTransmission()
  {
    delayMicroseconds(8);
    holdCeLow();
  }

  void write(uint8_t value, bool isDataRequestCommand = false)
  {
    for (uint8_t bit = 0; bit < 8; bit++) {
      digitalWrite(_io, value & 0x01);
      delayMicroseconds(1);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(1);
      if (bit == 7 && isDataRequestCommand)
        pinMode(_io, INPUT);
      digitalWrite(_clk, LOW);
      delayMicroseconds(1);
      value >>= 1;
    }
  }

  uint8_t read()
  {
    uint8_t value = 0;
    for (uint8_t bit = 0; bit < 8; bit++) {
      value |= (digitalRead(_io) << bit);
      digitalWrite(_clk, HIGH);
      delayMicroseconds(1);
      digitalWrite(_clk, LOW);
      delayMicroseconds(1);
    }
    return value;
  }

  void holdCeLow()
  {
    pinMode(_ce, OUTPUT);
    digitalWrite(_ce, LOW);
  }

private:
  uint8_t _io, _clk, _ce;
};

static inline void rtcCeGpioLow(void)
{
  pinMode(RTC_CE_GPIO, OUTPUT);
  digitalWrite(RTC_CE_GPIO, LOW);
}

static inline void rtcCeGpioTakeFromUart0(void)
{
  rtcCeGpioLow();
}
