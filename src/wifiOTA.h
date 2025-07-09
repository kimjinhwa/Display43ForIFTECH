#include <Arduino.h>
#ifndef _WIFIOTA_H
#define  _WIFIOTA_H

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

extern const char* host;

void wifiOTAsetup(bool isUpdate) ;
void wifiOtaloop(void) ;
#endif

