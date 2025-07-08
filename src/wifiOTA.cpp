#include "wifiOTA.h"
#include "esp32SelfUploder.h"
#include <Arduino_GFX_Library.h>

const char *host= "ups1p1p";
const char *ssid = "iptime_mbhong";
//const char *ssid = "iftech";
//const char *password = "iftech0273";
const char *password = "";

extern ESP32SelfUploder selfUploder;
IPAddress ipaddress(192, 168, 0, 202);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnetmask(255, 255, 255, 0);
IPAddress dns1(164, 124, 101, 2);
IPAddress dns2(8, 8, 8, 8);

extern Arduino_RPi_DPI_RGBPanel *gfx ;

void wifiOTAsetup(void)
{
  Serial.begin(BAUDRATEDEF);

  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  //Loop count
  int loopCount=5;
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
    gfx->println(".");
    loopCount--;
    if(loopCount <= 0)break;
  }

  Serial.println("");
  gfx->println("");
  Serial.print("Connected to ");
  gfx->println("Connected to ");
  Serial.println(ssid);
  gfx->println(ssid);
  Serial.print("IP address: ");
  gfx->println("IP address: ");
  Serial.println(WiFi.localIP());
  gfx->println(WiFi.localIP());
  selfUploder.begin(ssid, password, "https://raw.githubusercontent.com/kimjinhwa/IP-Fineder-For-ESP32/main/dist/ups1p1p");
  Serial.printf("Free heap before SSL: %d\n", ESP.getFreeHeap());
  if(selfUploder.checkNewVersion(selfUploder.update_url)){
    if (selfUploder.tryAutoUpdate(selfUploder.updateFile_url.c_str()))
    {
      Serial.println("Update success");
      gfx->println("Update success");
      ESP.restart();
    }
  }
  else
  {
    Serial.println("Already on latest version");
    gfx->println("Already on latest version");
  }
}
