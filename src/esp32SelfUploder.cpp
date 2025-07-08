#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>
#include <Update.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "esp32SelfUploder.h"
#include "../../../version.h"
#include <esp_task_wdt.h>
#include <Arduino_GFX_Library.h>

// 버전 비교 함수 정의
bool isNewerVersion(const char* currentVersion, const char* serverVersion) {
    // 버전을 점(.)으로 분리
    int currentMajor = 0, currentMinor = 0, currentPatch = 0;
    int serverMajor = 0, serverMinor = 0, serverPatch = 0;
    
    // 현재 버전 파싱 (예: "1.1.9" 또는 "1.1.111")
    if (sscanf(currentVersion, "%d.%d.%d", &currentMajor, &currentMinor, &currentPatch) != 3) {
        Serial.printf("현재 버전 파싱 실패: %s\n", currentVersion);
        return false;
    }
    
    // 서버 버전 파싱 (예: "2.0.0" 또는 "2.1.111")
    if (sscanf(serverVersion, "%d.%d.%d", &serverMajor, &serverMinor, &serverPatch) != 3) {
        Serial.printf("서버 버전 파싱 실패: %s\n", serverVersion);
        return false;
    }
    
    Serial.printf("Current: %d.%d.%d, Server: %d.%d.%d\n", 
                  currentMajor, currentMinor, currentPatch,
                  serverMajor, serverMinor, serverPatch);
    
    // 메이저 버전 비교
    if (serverMajor > currentMajor) return true;
    if (serverMajor < currentMajor) return false;
    
    // 마이너 버전 비교
    if (serverMinor > currentMinor) return true;
    if (serverMinor < currentMinor) return false;
    
    // 패치 버전 비교
    if (serverPatch > currentPatch) return true;
    
    return false;  // 서버 버전이 현재 버전보다 낮거나 같음
}

extern Arduino_RPi_DPI_RGBPanel *gfx ;

// 정적 함수 추가
void printProgress(size_t prg, size_t sz) {
    static int lastProgress = -1;
    int progress = (prg * 100) / sz;
    esp_task_wdt_reset();
    if (progress != lastProgress) {
        lastProgress = progress;
        Serial.printf("Progress: %d%%  ", progress);
        Serial.printf("Bytes: %d/%d\n", prg, sz);
        
        // 화면 지우고 진행률 표시
        gfx->fillScreen(BLACK);
        gfx->setCursor(0, 10);
        gfx->print("Progress:" + String(progress) + "%");
        gfx->setCursor(0, 30);
        gfx->print("Bytes: " + String(prg) + "/" + String(sz));
        
        digitalWrite(selfUploder.ledPin, !digitalRead(selfUploder.ledPin) );
    }
}

// SSL 클라이언트를 PSRAM에 할당하는 함수
WiFiClientSecure* createSSLClientInPSRAM() {
    Serial.printf("PSRAM 상태: %s\n", psramFound() ? "사용 가능" : "사용 불가");
    if (psramFound()) {
        Serial.printf("PSRAM 사용 가능 메모리: %d bytes\n", ESP.getFreePsram());
        
        // PSRAM에 SSL 클라이언트 할당
        WiFiClientSecure* client = (WiFiClientSecure*)ps_malloc(sizeof(WiFiClientSecure));
        if (client) {
            Serial.println("PSRAM에 SSL 클라이언트 할당 성공");
            new(client) WiFiClientSecure();  // 생성자 호출
            
            // SSL 연결에 필요한 최소 메모리: ~30KB
            if (ESP.getFreeHeap() < 30000) {
                Serial.printf("메모리 부족 (Free: %d) - SSL 연결 건너뛰기\n", ESP.getFreeHeap());
                return nullptr;
            }
            
            return client;
        } else {
            Serial.println("PSRAM 할당 실패, 일반 메모리 사용");
        }
    } else {
        Serial.println("PSRAM 없음, 일반 메모리 사용");
    }
    
    // 일반 메모리에 할당
    WiFiClientSecure* client = new WiFiClientSecure();
    Serial.printf("일반 메모리에 SSL 클라이언트 할당: %p\n", client);
    return client;
}

// SSL 클라이언트 해제 함수
void freeSSLClient(WiFiClientSecure* client) {
    if (client) {
        Serial.printf("SSL 클라이언트 해제: %p\n", client);
        if (psramFound()) {
            client->~WiFiClientSecure();  // 소멸자 호출
            free(client);  // PSRAM도 free로 해제 가능
        } else {
            delete client;  // 일반 메모리 해제
        }
    }
}

// 버전 체크 및 자동 업데이트 함수 추가

bool ESP32SelfUploder::tryAutoUpdate(const char* firmware_url) {
    //WiFiClientSecure* client = createSSLClientInPSRAM();
    WiFiClientSecure client ;//= createSSLClientInPSRAM();
    HTTPClient http;
    
    // SSL 버퍼 크기 줄이기
    Serial.printf("Checking for firmware at: %s\n", firmware_url);
    gfx->println("Checking for firmware at: " + String(firmware_url));
    // SSL 인증서 검증 건너뛰기
    client.setInsecure();
    
    // 타임아웃 설정 추가
    client.setTimeout(30000);  // 12초
    http.setTimeout(30000);    // 12초
    
    // 리다이렉션 허용
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    
    http.begin(client, firmware_url);
    
    // 일반 HTTP 요청을 위한 헤더
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

        // 진행률 표시 설정
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
                    //freeSSLClient(client);
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
        if (httpCode == HTTPC_ERROR_CONNECTION_REFUSED) {
            Serial.println("Connection refused");
            gfx->println("Connection refused");
        } else if (httpCode == HTTPC_ERROR_SEND_HEADER_FAILED) {
            Serial.println("Send header failed");
            gfx->println("Send header failed");
        } else if (httpCode == HTTPC_ERROR_SEND_PAYLOAD_FAILED) {
            Serial.println("Send payload failed");
            gfx->println("Send payload failed");
        } else if (httpCode == HTTPC_ERROR_NOT_CONNECTED) {
            Serial.println("Not connected");
            gfx->println("Not connected");
        } else if (httpCode == HTTPC_ERROR_CONNECTION_LOST) {
            Serial.println("Connection lost");
            gfx->println("Connection lost");
        } else if (httpCode == HTTPC_ERROR_NO_HTTP_SERVER) {
            Serial.println("No HTTP server");
            gfx->println("No HTTP server");
        }
    }
    
    http.end();
    //freeSSLClient(client);  // 메모리 해제 추가
    return false;
}

bool ESP32SelfUploder::checkNewVersion(const char* update_url) {
    // 메모리 상태 확인
    Serial.printf("시작 메모리: Free=%d, Largest=%d\n", 
                  ESP.getFreeHeap(), ESP.getMaxAllocHeap());
    WiFiClientSecure client;
    client.setInsecure();
    //WiFiClient client;
    HTTPClient http;
    
    Serial.printf("1.Free heap before SSL: %d\n", ESP.getFreeHeap());
    
    // SSL 설정
    
    Serial.printf("2.Free heap after SSL setup: %d\n", ESP.getFreeHeap());
    String version_url = String(update_url) + "/version.json";

    Serial.printf("version_url: %s\n", version_url.c_str());
    // 타임아웃 설정
    client.setTimeout(10000);  // 10초
    http.setTimeout(10000);    // 10초
    
    if( http.begin(client, version_url) == false) {
        Serial.println("HTTP 연결 실패");
        gfx->println("HTTP connection failed");
        http.end();
        return false;
    }

    Serial.printf("3.Free heap after http.begin: %d\n", ESP.getFreeHeap());
    
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
    Serial.printf("4.Free heap after http.GET: %d\n", ESP.getFreeHeap());
    Serial.printf("HTTP Response code: %d\n", httpCode);
    Serial.printf("Total heap: %d\n", ESP.getHeapSize() );
    Serial.printf("Largest free block: %d\n", ESP.getMaxAllocHeap());

    if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();

        Serial.println("Received JSON:");
        Serial.println(payload);
 
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload);
        
        if (!error) {
            const char* latest_version = doc["latest"];
            const char* filename = doc["filename"];
            
            Serial.printf("latest_version pointer: %p\n", latest_version);
            gfx->println("version : " + String(latest_version));
            Serial.printf("filename pointer: %p\n", filename);
 
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
            // 버전 비교
            if (isNewerVersion(String(VERSION).c_str(), latest_version)) {
                Serial.println("새로운 버전이 있습니다");
                gfx->println("New version is found");
                // updateFile_url 생성
                // GitHub rate limiting을 고려한 지연 추가
                Serial.println("GitHub rate limiting을 고려하여 5초 대기...");
                delay(1000);
                updateFile_url = String(update_url) + "/" + String(filename);
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
        // 상세한 에러 정보
        if (httpCode == HTTPC_ERROR_CONNECTION_REFUSED) {
            Serial.println("Connection refused - server not responding");
            gfx->println("Connection refused - server not responding");
        } else if (httpCode == HTTPC_ERROR_SEND_HEADER_FAILED) {
            Serial.println("Send header failed");
            gfx->println("Send header failed");
        } else if (httpCode == HTTPC_ERROR_SEND_PAYLOAD_FAILED) {
            Serial.println("Send payload failed");
            gfx->println("Send payload failed");
        } else if (httpCode == HTTPC_ERROR_NOT_CONNECTED) {
            Serial.println("Not connected");
        } else if (httpCode == HTTPC_ERROR_CONNECTION_LOST) {
            Serial.println("Connection lost");
        } else if (httpCode == HTTPC_ERROR_NO_HTTP_SERVER) {
            Serial.println("No HTTP server");
        }
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

// void ESP32SelfUploder::loop() {
//     static unsigned long lastCheckTime = 0;
//     const unsigned long CHECK_INTERVAL = 3600000; // 1시간 (밀리초)
    
//     httpServer.handleClient();
//     delay(1);
// }