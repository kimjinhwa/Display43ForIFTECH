#include "wifiOTA.h"
#include "esp32SelfUploder.h"
#include <Arduino_GFX_Library.h>
#include "mainGrobal.h"
#include "myBlueTooth.h"

#ifndef FW_UPDATE_BASE
#define FW_UPDATE_BASE "http://ift.iptime.org:81/Esp32UploadFirmware"
#endif

extern ESP32SelfUploder selfUploder;
IPAddress ipaddress(192, 168, 0, 202);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnetmask(255, 255, 255, 0);
IPAddress dns1(164, 124, 101, 2);
IPAddress dns2(8, 8, 8, 8);
extern Arduino_GFX *gfx ;

void wifiPrepareBeforeBle(void)
{
  /* WIFI_OFF 후 BLE만 켜면 S3에서 검색은 되고 GATT 연결은 timeout 난다.
     드라이버는 STA로 올려 두고 AP에는 붙이지 않는다. */
  WiFi.persistent(false);
  WiFi.setAutoReconnect(false);
  if (WiFi.getMode() == WIFI_OFF) {
    WiFi.mode(WIFI_STA);
    delay(100);
  }
  WiFi.setSleep(WIFI_PS_MIN_MODEM);
  WiFi.disconnect(false);
}

void wifiOTAsetup(bool isUpdate)
{
  wifiPrepareBeforeBle();

  if (WiFi.status() != WL_CONNECTED) {
    if (nvsSystemEEPRom.password[0] == '\0')
      WiFi.begin(nvsSystemEEPRom.ssid);
    else
      WiFi.begin(nvsSystemEEPRom.ssid, nvsSystemEEPRom.password);
    Serial.println("");
    Serial.printf("WiFi connect %s ...\n", nvsSystemEEPRom.ssid);
    int loopCount = 40;
    gfx->fillScreen(BLACK);
    delay(100);
    gfx->setCursor(0, 10);
    gfx->println("Connecting to WiFi..");
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
      gfx->print(".");
      loopCount--;
      if(loopCount <= 0) break;
    }
  } else {
    Serial.printf("WiFi already connected: %s\n", WiFi.localIP().toString().c_str());
  }

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("Failed to connect to WiFi");
    gfx->println("Failed to connect to WiFi");
    delay(2000);
    return;
  }

  Serial.println("");
  gfx->println("");
  Serial.print("Connected to ");
  gfx->println("Connected to ");
  Serial.println(nvsSystemEEPRom.ssid);
  gfx->println(nvsSystemEEPRom.ssid);
  Serial.print("IP address: ");
  gfx->println("IP address: ");
  Serial.println(WiFi.localIP());
  gfx->println(WiFi.localIP());
  if(!isUpdate) return;

  selfUploder.begin(nvsSystemEEPRom.ssid, nvsSystemEEPRom.password, FW_UPDATE_BASE);
  Serial.printf("Free heap before HTTP: %d\n", ESP.getFreeHeap());
  if(selfUploder.checkNewVersion(selfUploder.update_url)){
    if (selfUploder.tryAutoUpdate(selfUploder.updateFile_url.c_str()))
    {
      Serial.println("Update success");
      gfx->println("Update success");
      delay(2000);
      ESP.restart();
    }
    else
    {
      Serial.println("Update failed");
      gfx->println("Update failed");
      delay(3000);
    }
  }
  else
  {
    Serial.println("Already on latest version");
    gfx->println("Already on latest version");
    delay(2000);
  }
}
