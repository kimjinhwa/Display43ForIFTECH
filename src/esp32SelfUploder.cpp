#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>
#include <Update.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "esp32SelfUploder.h"
#include "../../../version.h"
#include <esp_task_wdt.h>
#include <Arduino_GFX_Library.h>

#ifndef FW_UPDATE_META
#define FW_UPDATE_META "version.json"
#endif

bool isNewerVersion(const char* currentVersion, const char* serverVersion) {
    int currentMajor = 0, currentMinor = 0, currentPatch = 0;
    int serverMajor = 0, serverMinor = 0, serverPatch = 0;
    
    if (sscanf(currentVersion, "%d.%d.%d", &currentMajor, &currentMinor, &currentPatch) != 3) {
        Serial.printf("현재 버전 파싱 실패: %s\n", currentVersion);
        return false;
    }
    
    if (sscanf(serverVersion, "%d.%d.%d", &serverMajor, &serverMinor, &serverPatch) != 3) {
        Serial.printf("서버 버전 파싱 실패: %s\n", serverVersion);
        return false;
    }
    
    Serial.printf("Current: %d.%d.%d, Server: %d.%d.%d\n", 
                  currentMajor, currentMinor, currentPatch,
                  serverMajor, serverMinor, serverPatch);
    
    if (serverMajor > currentMajor) return true;
    if (serverMajor < currentMajor) return false;
    if (serverMinor > currentMinor) return true;
    if (serverMinor < currentMinor) return false;
    if (serverPatch > currentPatch) return true;
    
    return false;
}

extern Arduino_GFX *gfx ;

void printProgress(size_t prg, size_t sz) {
    static int lastProgress = -1;
    int progress = (prg * 100) / sz;
    esp_task_wdt_reset();
    if (progress != lastProgress) {
        lastProgress = progress;
        Serial.printf("Progress: %d%%  ", progress);
        Serial.printf("Bytes: %d/%d\n", prg, sz);
        
        gfx->fillScreen(BLACK);
        gfx->setCursor(0, 10);
        gfx->print("Progress:" + String(progress) + "%");
        gfx->setCursor(0, 30);
        gfx->print("Bytes: " + String(prg) + "/" + String(sz));
        
        digitalWrite(selfUploder.ledPin, !digitalRead(selfUploder.ledPin) );
    }
}

bool ESP32SelfUploder::tryAutoUpdate(const char* firmware_url) {
    WiFiClient client;
    HTTPClient http;
    
    Serial.printf("Checking for firmware at: %s\n", firmware_url);
    gfx->println("Checking for firmware at: " + String(firmware_url));
    gfx->println("Firmware Version: " + String(VERSION));
    
    client.setTimeout(30000);
    http.setTimeout(30000);
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    
    http.begin(client, firmware_url);
    http.addHeader("User-Agent", "ESP32");
    
    Serial.println("Starting HTTP GET request...");
    gfx->println("Starting HTTP GET request...");
    int httpCode = http.GET();
    Serial.printf("HTTP Response code: %d\n", httpCode);
    gfx->println("HTTP Response code: " + String(httpCode));
    if (httpCode == HTTP_CODE_OK) {
        Serial.println("New firmware found! Starting update...");
        gfx->println("New firmware found! Starting update...");
        int contentLength = http.getSize();
        Serial.printf("contentLength: %d\n", contentLength);
        gfx->println("contentLength: " + String(contentLength));

        Update.onProgress(printProgress);
   
        if (Update.begin(contentLength)) {
            Serial.printf("Starting OTA: %d bytes\n", contentLength);
            gfx->println("Starting OTA: " + String(contentLength) + " bytes");
            size_t written = Update.writeStream(http.getStream());
            
            if (written == contentLength) {
                Serial.println("Written : " + String(written) + " successfully");
                gfx->println("Written : " + String(written) + " successfully");
                if (Update.end()) {
                    Serial.println("OTA update complete!");
                    gfx->println("OTA update complete!");
                    http.end();
                    ESP.restart();
                    return true;
                } else {
                    Serial.println("Error Occurred: " + String(Update.getError()));
                    gfx->println("Error Occurred: " + String(Update.getError()));
                }
            } else {
                Serial.println("Write Error Occurred. Written only : " + String(written) + "/" + String(contentLength));
                gfx->println("Write Error Occurred. Written only : " + String(written) + "/" + String(contentLength));
            }
        } else {
            Serial.println("Not enough space to begin OTA");
            gfx->println("Not enough space to begin OTA");
        }
    } else {
        Serial.printf("Firmware not found, HTTP error: %d\n", httpCode);
        gfx->println("Firmware not found, HTTP error: " + String(httpCode));
        Serial.println(http.errorToString(httpCode));
        gfx->println(http.errorToString(httpCode));
    }
    
    http.end();
    return false;
}

bool ESP32SelfUploder::checkNewVersion(const char* update_url) {
    Serial.printf("시작 메모리: Free=%d, Largest=%d\n", 
                  ESP.getFreeHeap(), ESP.getMaxAllocHeap());
    WiFiClient client;
    HTTPClient http;
    
    Serial.printf("1.Free heap before HTTP: %d\n", ESP.getFreeHeap());
    
    String base = String(update_url);
    while (base.endsWith("/"))
      base.remove(base.length() - 1);
    String version_url = base + "/" + String(FW_UPDATE_META);

    Serial.printf("version_url: %s\n", version_url.c_str());
    client.setTimeout(10000);
    http.setTimeout(10000);
    
    if( http.begin(client, version_url) == false) {
        Serial.println("HTTP 연결 실패");
        gfx->println("HTTP connection failed");
        http.end();
        return false;
    }

    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK) {
        Serial.println("HTTP_CODE_OK not found");
        gfx->println("HTTP_CODE_OK not found");
        vTaskDelay(1000);
        httpCode = http.GET();
    }
    if (httpCode != HTTP_CODE_OK) {
        Serial.println("HTTP_CODE_OK not found");
        gfx->println("HTTP_CODE_OK not found");
        vTaskDelay(1000);
        httpCode = http.GET();
    }
    Serial.printf("HTTP Response code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();

        Serial.println("Received JSON:");
        Serial.println(payload);
        
        if (payload.length() >= 3 && 
            (uint8_t)payload[0] == 0xEF && 
            (uint8_t)payload[1] == 0xBB && 
            (uint8_t)payload[2] == 0xBF) {
            payload = payload.substring(3);
        }
        payload.trim();
 
        DynamicJsonDocument doc(2048);
        DeserializationError error = deserializeJson(doc, payload);
        
        if (!error) {
            const char* latest_version = doc["latest"];
            const char* filename = doc["filename"];
            
            gfx->println("version : " + String(latest_version));
 
            if (!latest_version || !filename) {
                Serial.println("버전 정보나 파일명이 없습니다");
                gfx->println("Version or filename is not found");
                http.end();
                return false;
            }
            Serial.printf("Current version: %s\n", VERSION);
            gfx->println("Current version: " + String(VERSION));
            Serial.printf("Server version: %s\n", latest_version);
            gfx->println("Server version: " + String(latest_version));
            Serial.printf("Update file: %s\n", filename);
            gfx->println("Update file: " + String(filename));
            if (isNewerVersion(String(VERSION).c_str(), latest_version)) {
                Serial.println("새로운 버전이 있습니다");
                gfx->println("New version is found");
                updateFile_url = base + "/" + String(filename);
                http.end();
                return true;
            } else {
                Serial.println("Already on latest version");
                gfx->println("Already on latest version");
            }
        } else {
            Serial.print("JSON 파싱 실패: ");
            Serial.println(error.c_str());
            gfx->println("JSON parsing failed: " + String(error.c_str()));
        }
    } else {
        Serial.printf("HTTP GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        gfx->println("HTTP GET failed, error: " + String(http.errorToString(httpCode).c_str()));
    }
    
    http.end();
    return false;
}

void ESP32SelfUploder::begin(const char* ssid, const char* password, const char* update_url) {
    strncpy(this->ssid, ssid, sizeof(this->ssid) - 1);
    this->ssid[sizeof(this->ssid) - 1] = '\0';
    strncpy(this->password, password, sizeof(this->password) - 1);
    this->password[sizeof(this->password) - 1] = '\0';
    strncpy(this->update_url, update_url, sizeof(this->update_url) - 1);
    this->update_url[sizeof(this->update_url) - 1] = '\0';
    Serial.begin(115200);
    Serial.println();
}
