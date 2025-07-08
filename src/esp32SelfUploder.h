#ifndef ESP32SELFUPLODER_H
#define ESP32SELFUPLODER_H

struct FirmwareInfo {
    char latest[100];
    char filename[100];
};

class ESP32SelfUploder {
    private:
        FirmwareInfo currentFirmware;
        static const int FIRMWARE_INFO_ADDR = 0;
        
    public:
        char ssid[32];
        char password[32];
        char update_url[256];
        String updateFile_url;
        //WebServer httpServer;
        //HTTPUpdateServer httpUpdater;
        unsigned int ledPin;
        void setLed(unsigned int pin){
            ledPin = pin;
            pinMode(ledPin, OUTPUT);
        };
        // ESP32SelfUploder() : httpServer(80) {
        // }

        void begin(const char* ssid, const char* password, const char* update_url);
        //void loop();
        bool tryAutoUpdate(const char* firmware_url);
        bool checkNewVersion(const char* version_url);
};

extern ESP32SelfUploder selfUploder;

// 버전 비교 함수 선언
bool isNewerVersion(const char* currentVersion, const char* serverVersion);

#endif
