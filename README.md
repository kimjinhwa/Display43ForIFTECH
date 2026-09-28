# UPS 4.3" 터치 디스플레이

IFTECH 단상 UPS를 4.3인치 터치 패널에서 감시·운전하는 펌웨어입니다.  
인버터/충전기 모듈과는 Modbus RTU(마스터, **FC04** 읽기 / **FC10** 쓰기)로 통신하고, 화면은 LVGL + SquareLine 작화입니다.

현재 펌웨어 버전은 `Version.h`의 `3.0.21`입니다. `IFTECH_DISP43` / `IFTECH_DISP32` 빌드마다 patch가 1 올라갑니다.

작화 메모리맵(구형 335 HMI): [doc/UPS 335 단독_V6.4_작화 데이터 정리(20140228).xls](doc/UPS%20335%20단독_V6.4_작화%20데이터%20정리(20140228).xls)  
시트 **사용중인 전체 메모리번지 정리(최종)**. 엑셀은 3상 335 HMI 맵이고, 이 펌웨어는 단상으로 재배치되어 있습니다.  
현재 번지 기준은 `src/mainGrobal.h`의 `ups_modbus_data_t`입니다. 스킬 스냅샷: [`.cursor/skills/ups-display-modbus-ui/dumps/memory_map.tsv`](.cursor/skills/ups-display-modbus-ui/dumps/memory_map.tsv)  
HMI 통신 보조: [doc/단상 HMI 통신_4_2inch.xlsx](doc/단상%20HMI%20통신_4_2inch.xlsx)

에이전트용 스킬:

- [`.cursor/skills/squareline-ui/SKILL.md`](.cursor/skills/squareline-ui/SKILL.md) — SquareLine 생성물 vs `src/` 수정 원칙
- [`.cursor/skills/ups-display-modbus-ui/SKILL.md`](.cursor/skills/ups-display-modbus-ui/SKILL.md) — 모드버스 번지·상태 비트 ↔ UI

파생: 이 작화를 충전기 맵에 맞춘 프로그램이 `1.Display4.3_ChargerForAseaENG`입니다. **4.3" 보드(ESP32-4827S043R)는 동일**합니다.

---

## 1. 목적

- UPS 입력·배터리·인버터·출력 계측과 전력흐름을 4.3" 화면에 표시한다.
- 운전/정지, 설정(이득·오프셋·시간·밝기), 알람 리셋을 터치로 모듈에 쓴다.
- 경보·이벤트 로그를 화면에 남긴다.
- BLE(`IFT_43_<MAC>`)와 Wi‑Fi OTA로 현장 펌웨어를 올린다.

---

## 2. 구조

```
src/                 펌웨어 본체 (ui_init 이후 제품 로직)
lib/ui_disp43/       SquareLine export 4.3"
lib/ui_disp32/       SquareLine export 3.2"
lib/Lvgl/            LVGL 8.3.0-dev 로컬 복사
lib/Arduino_GFX-master/  RGB / SPI 패널 드라이버
lib/upsLog/          경보·이벤트 문자열
lib/Mcp23s08/        MCP23S08 (RTC CE, 부저)
lib/lv_i18n/         한/영
IFTECH_UPS43_App/    BLE + OTA용 Flutter 앱
doc/                 사양서·회로·작화 엑셀
.cursor/skills/      Cursor 에이전트 스킬
```

| 경로 | 역할 |
|------|------|
| `src/main.cpp` | 부팅, LVGL 루프, `mainScrUpdata()` 전력흐름 |
| `src/modbusRtu.cpp` | FC04 읽기 / FC10 쓰기 |
| `src/ui_events.cpp` | 버튼·설정·숫자 키패드 (생성 스텁이 아님) |
| `src/display.cpp` | 4.3" RGB 480×272 또는 3.2" ST7789 |
| `src/touch.h` | XPT2046 저항막 |
| `src/wifiOTA.cpp` | Wi‑Fi OTA |
| `src/myBlueTooth.cpp` | BLE Nordic UART + CLI |
| `lib/ui_disp43/` | SquareLine 생성물. 통째 복사 대상 |
| `platformio.ini` | `esp32_Release`, `IFTECH_DISP43`, `IFTECH_DISP32` |

작화 원칙([squareline-ui](.cursor/skills/squareline-ui/SKILL.md)): SquareLine 생성 파일은 가급적 손대지 않고, 제품 옵션·키패드 배치는 `ui_init()` 다음 `src/`에서 바꾼다. `src/ui_events.cpp`는 SquareLine 통째 복사에서 뺀다.

SquareLine 프로젝트: `C:/DevWork/99.SquireLineWave/IFTECH_UI/UPS1P1P_V001`

---

## 3. 동작 방식

1. ESP32-S3가 RGB 패널을 직접 구동하고 LVGL로 그린다.
2. `Serial2` (RX GPIO19, TX GPIO20)로 모듈 슬레이브와 Modbus RTU를 주고받는다.
3. 읽기 FC04, 쓰기 FC10. 상태 워드 15(`ModuleState`) / 16(`HWState`) / 17(`upsOperationFault`).
4. 설정 입력은 전체화면 숫자 키패드(`changeKeyboardText()`). SquareLine 뼈대 + 런타임 재배치.

이 보드에서 쓰는 GPIO (4.3"):

| 기능 | 핀 |
|------|-----|
| RS-232 (Serial2) | RX 19, TX 20 |
| MCP23S08 CS | GPIO17 |
| MCP GP0 | DS1302 RTC CE |
| MCP GP1 | 부저 |
| 터치 SPI | SCK 12, MOSI 11, MISO 13, CS 38, INT 18 |
| LCD 백라이트 | GPIO2 |

RTC와 터치가 GPIO 11·12를 공유하므로, RTC 접근 중에는 터치를 읽지 않습니다.

---

## 4. CPU · 디스플레이 사양

이 제품(4.3")은 **ESP32-4827S043R** (4.3" TN, 저항막 XPT2046)입니다.  
사양 숫자는 `doc/Display(4.3inch)` PDF와 `sunton_s3.json`에서 가져왔습니다. 충전기 디스플레이와 같은 모듈입니다.

### CPU (ESP32-S3-WROOM-1)

| 항목 | 값 | 출처 |
|------|-----|------|
| MCU | ESP32-S3, Xtensa 32-bit LX7 **듀얼코어** | JCZN 사양서, Getting Started |
| 클럭 | 240 MHz (`board_build.f_cpu`) | `platformio.ini`, 사양서 |
| 연산 | 600 DMIPS | Getting Started PDF |
| SRAM / ROM | 512 KB / 384 KB | JCZN 사양서 |
| PSRAM | 8 MB OPI (`qio_opi`, `BOARD_HAS_PSRAM`) | 사양서, `sunton_s3.json` |
| Flash | 16 MB | 사양서, `default_16MB.csv` |
| 무선 | Wi‑Fi HT40, Classic BT / BLE 4.2 | Getting Started PDF |
| USB-UART | CH340C, USB Type-C | MCU 회로도 JPG |
| 모듈 소비전류 | 약 260 mA @ 5 V | JCZN 사양서 |

![보드 뒷면 치수](doc/Display(4.3inch)/Dimensions.png)

치수 PNG 기준: PCB **123.0 × 74.0 mm**, 고정홀 Ø3.20, 홀 간격 105.5 × 67.2 mm.

### 디스플레이 (4.3")

| 항목 | 값 | 출처 |
|------|-----|------|
| 모듈 | ESP32-4827S043N / R / C | JCZN 사양서 |
| 이 펌웨어 | **R = 저항막 XPT2046** | `src/touch.h` |
| 크기 | 4.3 inch TFT | 사양서 |
| 해상도 | **480 × 272** | 사양서, `display.cpp` |
| 유효 표시 영역 | 95.04 × 53.86 mm | 사양서·패널 사양서 |
| 모듈 크기 (사양서) | 105.5 × 74 mm | JCZN 사양서 |
| 패널(LCM) | 105.50 × 67.20 × 3.00 mm, a-Si TFT, 시야 6시 | `4.3” 480X272-TN bare screen specs.pdf` |
| 색 | RGB 65K (16-bit) | 사양서 |
| 인터페이스 | RGB parallel (`Arduino_RPi_DPI_RGBPanel`) | `src/display.cpp` |
| 패널 드라이버 | 사양서 **ILI9485**, 패널 PDF **ILI6485** (코드 주석 ILI6485) | 두 PDF |
| 백라이트 | 10 LED, GPIO2 | 패널 사양서, 회로도 |
| 밝기 (패널 사양) | 500 cd/m² typ. | 패널 사양서 |
| 동작 온도 | −20 ℃ ~ 70 ℃ | 사양서 |
| 화소 클럭 | 6 MHz (`prefer_speed`) | `src/display.cpp` |

![4.3" 패널](doc/Display(4.3inch)/4.3.png)

RGB 핀은 `src/display.cpp`와 보드 회로가 같습니다 (DE 40, VSYNC 41, HSYNC 39, PCLK 42).

### 3.2" 환경 (`IFTECH_DISP32`)

같은 펌웨어의 다른 보드입니다. ESP32-WROOM-32E + ST7789 240×320, 터치 XPT2046, `lib/ui_disp32/`.

### LVGL · SquareLine

| 항목 | 값 |
|------|-----|
| 런타임 LVGL | **8.3.0-dev** (`lib/Lvgl/lvgl.h` `LVGL_VERSION_*`) |
| SquareLine export 기준 LVGL | **8.3.6** (`ui_MainScreen.c` 헤더) |
| SquareLine Studio | **1.4.1** |
| 색 깊이 | 16-bit, `LV_COLOR_16_SWAP=0` |

문서:

- LVGL 8.3: https://docs.lvgl.io/8.3/
- SquareLine: https://docs.squareline.io/
- Espressif ESP32-S3: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/

---

## 5. 컴파일

필요: [PlatformIO](https://platformio.org/) (VS Code 확장 또는 CLI), Python 3.

보드 정의는 저장소 루트 `sunton_s3.json`입니다. Flash 16 MB, PSRAM OPI, 업로드 460800 bps.

제품 빌드 4.3" (버전 +1, IIS 배포 스크립트 포함):

```bat
pio run -e IFTECH_DISP43
```

로컬만 4.3" (버전 안 올림):

```bat
pio run -e esp32_Release
```

3.2" (`esp32dev`, 4 MB `huge_app.csv`, ST7789):

```bat
pio run -e IFTECH_DISP32
```

VS Code에서는 PlatformIO 환경 `IFTECH_DISP43`를 고른 뒤 Build.

`IFTECH_DISP43` / `IFTECH_DISP32`만 `pre_build.py`가 `Version.h` patch를 +1 합니다. 쓸데없는 빌드를 반복하지 마십시오.

산출물: `.pio/build/<env>/firmware.bin`  
`post_build.py`가 아래에도 복사합니다.

- `uploadFirmware/` (로컬, bootloader/partitions 포함)
- `C:\inetpub\ftproot\Esp32UploadFirmware` (OTA용 firmware + JSON만)

파티션 4.3" (`default_16MB.csv`): app0/app1 각 0x640000, SPIFFS 0x360000.  
3.2"는 `huge_app.csv`(4 MB)입니다.

한/영 문자열을 다시 뽑을 때:

```sh
lv_i18n extract -s "src/**/*.+(c|cpp|h|hpp)" -t "translations/*.yml"
lv_i18n compile -t "src/resource/*.yml" -o "lib/lv_i18n/src/"
```

SquareLine 번역 export를 넣을 때 예:

```sh
lv_i18n compile -t "C:/DevWork/99.SquireLineWave/IFTECH_UI/UPS1P1P_V001/export/SquareLine_Project/libraries/ui/translations/*.yml" -o "lib/lv_i18n/src"
```

---

## 6. 업로드

기본 포트는 `platformio.ini`의 **COM3**, 모니터 115200입니다.

### USB (개발)

보드 USB-C → CH340. 칩은 **ESP32-S3**입니다 (`--chip esp32` 아님).

```bat
pio run -e IFTECH_DISP43 -t upload
pio device monitor -e IFTECH_DISP43
```

전체 플래시 맵 (esptool, S3):

| 주소 | 파일 |
|------|------|
| `0x0` | bootloader.bin |
| `0x8000` | partitions.bin |
| `0xe000` | boot_app0.bin |
| `0x10000` | firmware.bin |

`uploadFirmware/`에 버전 붙은 bin이 있으면 그것으로 올립니다. Arduino 패키지의 `boot_app0.bin`은  
`%USERPROFILE%\.platformio\packages\framework-arduinoespressif32\tools\partitions\boot_app0.bin` 입니다.

### Wi‑Fi OTA (현장)

1. `pio run -e IFTECH_DISP43` → IIS `Esp32UploadFirmware`에 `firmware_*.bin`과 `IFTECH_DISP43.json` 배포
2. 앱 `IFTECH_UPS43_App`에서 BLE `IFT_43_…` 연결 → Wi‑Fi 프리셋 전송 → 펌웨어 업그레이드
3. 단말기 재부팅 후 BLE보다 먼저 OTA

서버: `http://ift.iptime.org:81/Esp32UploadFirmware`  
메타: `IFTECH_DISP43.json`

시리얼/BLE CLI: `ssid`, `pass`, `version`, `update`. 자세한 절차는 [upgradeDoc/upgradeManual.md](upgradeDoc/upgradeManual.md).

앱 실행:

```bash
cd IFTECH_UPS43_App
flutter pub get
flutter run
```

---

## 7. 도구 실행

### 버전 태그

```powershell
.\gitversiontag.ps1
.\gitversiontag.ps1 -t
```

`Version.h` 주석으로 커밋 메시지를 만듭니다.

---

## 8. 보드 · 기구 · 자료

![MCU 보드 회로](doc/Display(4.3inch)/ESP32-4827S043-MCU-V1.0.jpg)

![RGB/터치 회로](doc/Display(4.3inch)/ESP32-4827S043-1.png)

![ESP32-S3-WROOM-1 핀](doc/Display(4.3inch)/ESP32-S3-WROOM-1%20Pin%20definition.png)

로컬 원본 (`doc/Display(4.3inch)`):

| 파일 | 내용 |
|------|------|
| [ESP32-4827S043 Specifications-EN.pdf](doc/Display(4.3inch)/ESP32-4827S043%20Specifications-EN.pdf) | 모듈 사양 |
| [Getting started 4.3 Inch.pdf](doc/Display(4.3inch)/Getting%20started%204.3%20Inch.pdf) | Arduino 설치·업로드 |
| [4.3” 480X272-TN bare screen specs.pdf](doc/Display(4.3inch)/4.3”%20480X272-TN%20bare%20screen%20specs.pdf) | 패널 JC4827B043N |
| [ESP32-4827S043-MCU-V1.0_SCHEMETIC.pdf](doc/Display(4.3inch)/ESP32-4827S043-MCU-V1.0_SCHEMETIC.pdf) | MCU 회로 |
| [esp32-s3_datasheet_en.pdf](doc/Display(4.3inch)/esp32-s3_datasheet_en.pdf) | ESP32-S3 데이터시트 |
| [esp32-s3_technical_reference_manual_en.pdf](doc/Display(4.3inch)/esp32-s3_technical_reference_manual_en.pdf) | ESP32-S3 TRM |
| [esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf](doc/Display(4.3inch)/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf) | WROOM-1 |
| [XPT2046.pdf](doc/Display(4.3inch)/XPT2046.pdf) | 저항막 컨트롤러 |
| [결선도.pdf](doc/결선도.pdf) | UPS 디스플레이 결선 |
| OrCAD `doc/1PH_DISP4IN_V3.DSN`, `doc/Display(4.3inch)/1PH_DISP4IN_*.DSN` | 쪽보드 회로 |

### 제조사 클라우드 · 위키

- 자료 클라우드: https://pan.jczn1688.com/#/
- ESP32-4827S043 공유 폴더: http://pan.jczn1688.com/s/7a5wvd
- JCZN 홈: http://www.jczn1688.com/
- 자료 다운로드(zlxz): http://www.jczn1688.com/zlxz
- JCZN 위키: https://wiki.jczn1688.com/
- 커뮤니티 보드 위키 (Sunton ESP32-4827S043): https://openhasp.com/0.7.0/hardware/sunton/esp32-4827s043/
- ESP32-S3 공식 문서: https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/
- LVGL 8.3 위키/문서: https://docs.lvgl.io/8.3/
- SquareLine 문서: https://docs.squareline.io/

`wiki.jczn1688.com`은 제조사 위키 호스트입니다. 일시적으로 응답이 없으면 `zlxz`·`pan.jczn1688.com`의 PDF를 쓰면 됩니다.
