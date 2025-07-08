#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>
typedef struct {
    int index;
    int value;
    uint32_t token;
} ModbusCommand;

extern QueueHandle_t modbusCmdQueue;
#endif