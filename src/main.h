#ifndef MAIN_H
#define MAIN_H
#include <Arduino.h>

class RtcDateTime;
void stopSpi(void);
RtcDateTime setRtc(bool write, const RtcDateTime *newTime);
//void setRtc(bool write, const RtcDateTime *newTime);

typedef struct {
    int index;
    int value;
    uint32_t token;
} ModbusCommand;

extern QueueHandle_t modbusCmdQueue;

void uiBindLoopTask(void);
void requestMeasureRefresh(void);
void requestSettingRefresh(void);
void drainPendingUiUpdates(void);
#endif