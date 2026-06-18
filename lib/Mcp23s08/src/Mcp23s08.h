/**
 * @file Mcp23s08.h
 * @brief MCP23S08-E/SS (A2=A1=A0=GND).
 *
 * Pin map:
 *   GP0 — DS1302 RTC /CE (HIGH=selected)
 *   GP1 — Buzzer (HIGH=ON)
 *
 * SPI.begin() must run before Mcp23s08_begin().
 */
#pragma once

#include <Arduino.h>
#include <stdint.h>

void Mcp23s08_begin(uint8_t mcpCsPin, uint32_t spiClockHz);
void Mcp23s08_end(void);
void Mcp23s08_initOutputsAll(void);
void Mcp23s08_writeReg(uint8_t reg, uint8_t val);
uint8_t Mcp23s08_readReg(uint8_t reg);
void Mcp23s08_setOutput(uint8_t pattern);

void Mcp23s08_RtcCe(bool high);
bool Mcp23s08_RtcCeIsHigh(void);

void Mcp23s08_BuzzerControl(bool on);
void Mcp23s08_BuzzerToggle(void);
bool Mcp23s08_BuzzerIsOn(void);
