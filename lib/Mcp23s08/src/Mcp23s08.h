/**
 * @file Mcp23s08.h
 * @brief MCP23S08-E/SS (A2=A1=A0=GND).
 *
 * Pin map (board rev after CE rework):
 *   GP0 — unused, keep LOW (do not drive DS1302 CE)
 *   GP1 — Buzzer (HIGH=ON)
 * DS1302 CE = GPIO43 (UART0 TX).
 */
#pragma once

#include <Arduino.h>
#include <stdint.h>

#ifndef BUZZER
#define BUZZER 1
#endif

void Mcp23s08_begin(uint8_t mcpCsPin, uint32_t spiClockHz);
void Mcp23s08_end(void);
void Mcp23s08_initOutputsAll(void);
void Mcp23s08_writeReg(uint8_t reg, uint8_t val);
uint8_t Mcp23s08_readReg(uint8_t reg);
void Mcp23s08_setOutput(uint8_t pattern);
/** 부저 n회. GP0는 항상 LOW. */
void Mcp23s08_bootBeep(int times);
void Mcp23s08_bootBeep3(void);

void Mcp23s08_RtcCe(bool high);
bool Mcp23s08_RtcCeIsHigh(void);
uint8_t Mcp23s08_forceRtcCeLow(void);

void Mcp23s08_BuzzerControl(bool on);
void Mcp23s08_BuzzerToggle(void);
bool Mcp23s08_BuzzerIsOn(void);
