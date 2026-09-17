---
name: rtc-ce-u0txd
description: >-
  Move DS1302 CE from MCP23S08 GP0 onto ESP32-S3 UART0 TX (GPIO43), same as
  Display4.3_ChargerForAseaENG after hardware rework. Use when porting to
  Display4.3 UPS, MCP RtcCe, McpRtcThreeWire, SPI.end touch death, or GPIO43 CE.
---

# DS1302 CE = UART0 TX (GPIO43)

대상: `C:\DevWork\4.IFTechWork\1.UPS1P1P\Display4.3` (같은 Sunton 4.3 보드, **시간 변경 있음**).

기준(이미 동작): `C:\DevWork\4.IFTechWork\1.UPS1P1P\1.Display4.3_ChargerForAseaENG`

복사할 파일:

- `include/rtcCeGpioWire.h`
- `include/spiBusCs.h` (SPI.begin 4번째 인자 `-1`, RTC 비트뱅 후 `spiBusRecoverFromBitBang`)

## 왜 MCP CE를 버리는가

CE를 MCP GP0에 두면, RTC를 켠 뒤 **같은 SCK/MOSI**로 MCP에 “CE 꺼라”를 보낼 수 없다. `SPI.end()`는 MCP를 잠근다(`OLAT=0xFF`, 전원 사이클). 라이브러리 `ThreeWire::begin()`은 CE를 **입력**으로 띄워 CH340 RX 풀업으로 RTC가 다시 켜진다.

해결: **CE = GPIO43 (U0TXD)**. ESP가 푸시풀 Low/High. MCP는 부저(GP1)만. GP0는 구동하지 않음(Low).

## 하드웨어 (필수)

충전기 보드와 같게:

- DS1302 CE ← GPIO43 ← CH340 RX (병렬 OK)
- **MCP GP0와 CE를 동시에 밀지 말 것** (GP0 분리 또는 MCP에서 미사용)
- 업로드는 UART0이라 대체로 됨. 앱 로그는 **BLE** (`IFT_UPS43_`+MAC). COM 모니터는 TX를 씀

## 소프트웨어 작업

1. `McpRtcThreeWire` / `Mcp23s08_RtcCe(true)` **제거**. `RtcCe(false)`는 GP0를 Low로 두는 용도만.
2. `RtcCeGpioWire(MOSI=11, SCK=12, CE=43)` + `RtcDS1302<RtcCeGpioWire>`.
3. `initSetRtc()` / `setRtc()`: CE Low → 전송 때만 High → `endTransmission`에서 Low → `spiBusRecoverFromBitBang()` (CS 17·38 High, CE Low, `SPI.end` 후 `SPI.begin(12,13,11,-1)`).
4. `ThreeWire` 기본 클래스 쓰지 말 것 (`pinMode(CE, INPUT)` 금지).
5. `gpioInit`/`setup` 앞: 터치 CS 38·MCP CS 17 **출력 High**, GPIO43 **출력 Low**. `SPI.begin(..., TOUCH_CS)` 금지 → `-1`.
6. `stopTouchSpi()`의 `SPI.end()` 제거. CE Low + CS High만.
7. 부팅: 삑1(GP0=0, OLAT `0x02`) → RTC 읽고 `settimeofday` → SPI 복구 → 삑2. UPS는 `setRtc(true)`를 **살릴 것** (화면/모드버스 시각). 충전기는 쓰기 UI 없음.
8. 모드버스 FC06 65~70: `settimeofday` + `setRtc(true, &now)` 주석이면 풀기.
9. BLE 이름 `IFT_UPS43_` 유지. UART0을 CE로 쓰므로 시리얼 모니터에 의존하지 말 것.

## 확인

- BLE: RTC 시각, `CE=0`, 터치 좌표 변화, 부저
- 스코프 GPIO43: 유휴 Low, RTC 접근 때만 High 펄스
- MCP GP0는 스코프에서 움직이지 않음

기준 구현: 충전기 `src/main.cpp` `initSetRtc` / `include/rtcCeGpioWire.h`. UPS `setRtc`는 같은 와이어로 쓰기까지 연결.
