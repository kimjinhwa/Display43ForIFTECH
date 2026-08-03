# UPS43D1P 펌웨어 업그레이드

## 개요

1. 빌드 PC에서 `pio run -e IFTECH_DISP43` → Version.h patch +1 후 IIS에 배포
2. 앱(UPS43D1P)에서 BLE로 연결 → Wi-Fi 프리셋 선택 → 전송 → 펌웨어 업그레이드
3. 단말기는 재부팅 후 BLE보다 먼저 WiFi OTA 수행

## 서버

- Base URL: `http://ift.iptime.org:81/Esp32UploadFirmware`
- Meta: `IFTECH_DISP43.json`
- 물리 경로: `C:\inetpub\ftproot\Esp32UploadFirmware`

JSON 예:

```json
{
  "latest": "3.0.2",
  "filename": "firmware_3.0.2_IFTECH_DISP43.bin"
}
```

## 앱 사용 (UPS43D1P)

1. BLE 검색 → `IFT_UPS…` 장비 연결
2. Wi-Fi 프리셋 선택 (타이핑 불필요)
   - **iftech** / iftech0273
   - **iptime_mbhong** / 오픈
   - **AndroidHotspot** / 87654321
   - 그 외는 **직접 입력**
3. `SSID / PASS 장비로 전송`
4. `펌웨어 업그레이드` (또는 CLI `update`)
5. 단말기 재부팅 → WiFi로 서버 최신 펌웨어 플래시

## CLI (시리얼/BLE)

```
ssid          # 조회
ssid NAME     # 설정
pass none     # 오픈 AP
pass SECRET   # 비밀번호
version
update        # isUpdate 플래그 저장 후 재부팅 (BLE 중 WiFi 연결 안 함)
```

## 주의

- BLE 연결 중에 WiFi OTA를 동시에 시작하지 않음
- 매 빌드마다 Version.h patch가 +1 되므로 불필요한 빌드에 주의
