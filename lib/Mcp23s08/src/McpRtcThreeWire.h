/**
 * DS1302 3-wire: RTC /CE on MCP23S08 GP0 (not ESP32 GPIO).
 *
 * MOSI/SCK are shared with MCP23S08 SPI and DS1302 3-wire.
 * CE must be set via MCP SPI while the bus is in SPI mode, then SPI.end()
 * before bit-banging IO/CLK for DS1302.
 */
#pragma once

#include <Arduino.h>
#include <SPI.h>
#include "Mcp23s08.h"

const uint8_t MCP_RTC_THREEWIRE_READFLAG = 0x01;

/* DS1302 tCC/tCWH minimum is 4us; GP0 via MCP23S08 SPI needs extra settle margin. */
static constexpr uint32_t kMcpRtcCeSetupUs = 20;
static constexpr uint32_t kMcpRtcCeHoldUs = 20;

class McpRtcThreeWire
{
public:
  McpRtcThreeWire(uint8_t ioPin, uint8_t clkPin) : _ioPin(ioPin), _clkPin(clkPin) {}

  void configureSpiBus(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t touchCs)
  {
    _spiSck = sck;
    _spiMiso = miso;
    _spiMosi = mosi;
    _touchCs = touchCs;
    _spiConfigured = true;
  }

  void begin()
  {
    ensureSpiForMcp();
    Mcp23s08_RtcCe(false);
    releaseSpiForBitBang();
    resetPins();
  }

  void end()
  {
    ensureSpiForMcp();
    Mcp23s08_RtcCe(false);
    releaseSpiForBitBang();
    resetPins();
  }

  void beginTransmission(uint8_t command)
  {
    ensureSpiForMcp();
    Mcp23s08_RtcCe(false);
    Mcp23s08_RtcCe(true);
    delayMicroseconds(kMcpRtcCeSetupUs);

    releaseSpiForBitBang();

    write(command, (command & MCP_RTC_THREEWIRE_READFLAG) == MCP_RTC_THREEWIRE_READFLAG);
  }

  void endTransmission()
  {
    delayMicroseconds(kMcpRtcCeHoldUs);
    ensureSpiForMcp();
    Mcp23s08_RtcCe(false);
  }

  void write(uint8_t value, bool isDataRequestCommand = false)
  {
    for (uint8_t bit = 0; bit < 8; bit++)
    {
      digitalWrite(_ioPin, value & 0x01);
      delayMicroseconds(1);

      digitalWrite(_clkPin, HIGH);
      delayMicroseconds(1);

      if (bit == 7 && isDataRequestCommand)
        pinMode(_ioPin, INPUT);

      digitalWrite(_clkPin, LOW);
      delayMicroseconds(1);

      value >>= 1;
    }
  }

  uint8_t read()
  {
    uint8_t value = 0;

    for (uint8_t bit = 0; bit < 8; bit++)
    {
      value |= (digitalRead(_ioPin) << bit);

      digitalWrite(_clkPin, HIGH);
      delayMicroseconds(1);

      digitalWrite(_clkPin, LOW);
      delayMicroseconds(1);
    }

    return value;
  }

private:
  const uint8_t _ioPin;
  const uint8_t _clkPin;
  uint8_t _spiSck = 0;
  uint8_t _spiMiso = 0;
  uint8_t _spiMosi = 0;
  uint8_t _touchCs = 0xFF;
  bool _spiConfigured = false;
  bool _spiBusActive = false;

  void ensureSpiForMcp()
  {
    if (!_spiConfigured || _spiBusActive)
      return;
    SPI.begin(_spiSck, _spiMiso, _spiMosi, _touchCs);
    if (_touchCs != 0xFF)
      digitalWrite(_touchCs, HIGH);
    _spiBusActive = true;
  }

  void releaseSpiForBitBang()
  {
    if (!_spiBusActive)
      return;
    SPI.end();
    _spiBusActive = false;
    if (_touchCs != 0xFF)
      digitalWrite(_touchCs, HIGH);
    digitalWrite(_clkPin, LOW);
    pinMode(_clkPin, OUTPUT);
    pinMode(_ioPin, OUTPUT);
  }

  void resetPins()
  {
    pinMode(_clkPin, INPUT);
    pinMode(_ioPin, INPUT);
  }
};
