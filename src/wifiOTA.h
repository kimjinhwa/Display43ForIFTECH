#include <Arduino.h>
#ifndef _WIFIOTA_H
#define  _WIFIOTA_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

extern const char* host;

/** BLE 시작 전에 WiFi 드라이버를 STA로 올려 둔다 (이후 BLE+WiFi 공존). */
void wifiPrepareBeforeBle(void);
void wifiOTAsetup(bool isUpdate) ;
void wifiOtaloop(void) ;
#endif
