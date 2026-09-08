---
name: ups-display-modbus-ui
description: >-
  Maps UPS Display4.3 Modbus RTU registers/status bits to LVGL UI widgets,
  logs, and power-flow graphics. Use when changing, disabling, or relocating
  status bits; when the 작화 Excel memory map changes; when removing or extending
  NFB/MCCB/차단기, 입력라인, 전력선, converter/inverter/SCR graphics; or when the user
  mentions func04, func10, MW/MB, 모드버스 번지, mainScrUpdata, or SquareLine UI
  components.
---

# UPS Display Modbus ↔ UI

이 제품은 단상 UPS 4.3" 터치 디스플레이(ESP32 + LVGL)이다.
작화 원본은 `doc/UPS 335 단독_V6.4_작화 데이터 정리(20140228).xls` 의
시트 **사용중인 전체 메모리번지 정리(최종)** 이다.
현재 펌웨어 번지는 `src/mainGrobal.h` 의 `ups_modbus_data_t` 가 기준이다.

엑셀은 구형 3상 335 HMI(MW/MB) 맵이다. 이 펌웨어는 단상으로 축약·재배치되어 있다.
이름(입력 NFB, 정전, SCR 등)으로 요청이 오면 엑셀 이름을 `mapping.md`에서 찾고,
그다음 현재 코드 필드와 UI 위젯으로 번역한다.

## 반드시 읽을 자료

요청 유형에 따라 해당 파일만 추가로 연다.

- 번지/비트 ↔ 위젯: [mapping.md](mapping.md)
- 차단기·전력선 제거/연장: [hardware-options.md](hardware-options.md)
- SquareLine 생성물 수정 원칙: `.cursor/skills/squareline-ui/SKILL.md`
- 엑셀 변경 반영: 아래 "엑셀 정의가 바뀌면" + `scripts/dump_xls.py`
- 마지막 작화 스냅샷: `dumps/memory_map.tsv`

## 통신

- 프로토콜: Modbus RTU. 디스플레이는 마스터(`MODBUSMASTER`).
- 읽기: Function **0x04** (Input Register). 주기 폴링은 `src/modbusRtu.cpp`.
- 쓰기: Function **0x10** (Write Multiple Registers). UI 설정/운전 명령은
  `enqueueModbusCommand()` → `WriteHoldRegistor()`.
- Zero-based 주소. 워드 1개 = `ups_modbus_data_t` 필드 1개.

상태/이벤트 핵심 워드:

| 번지 | 구조체 | 용도 |
|------|--------|------|
| 6 | `upsRun` | UpsON / UpsOFF / ALARM_RESET 명령 |
| 15 | `ModuleState` | 충전기/DCDC/인버터 운전 이벤트 |
| 16 | `HWState` | H/W 고장 + 운전접점(CONV/DCDC/INV/SCR) |
| 17 | `upsOperationFault` | 운전 이상(정전, UV/OV 등) |

## 화면 갱신 위치

| 동작 | 함수 | 파일 |
|------|------|------|
| 메인 전력흐름 색/ON-OFF | `mainScrUpdata()` | `src/main.cpp` |
| 계측값 라벨 | `scrMeasureLoad()` | `src/ui_events.cpp` |
| 설정값 라벨 | `scrSettingScreen()` | `src/ui_events.cpp` |
| 초기화면 정격 | `ui_lblCapacity` 등 | `src/main.cpp` 루프 |
| 이벤트/알람 기록 | `upsLog::setEventCode()` | `lib/upsLog/src/upsLog.cpp` |
| 알람 아이콘/부저 | `toggleBuzzer()` | `src/main.cpp` |

4.3" 위젯 정의: `lib/ui_disp43/src/screens/` (`-DDISPLAY_43`).
3.2" 동일 이름 위젯: `lib/ui_disp32/` (`-DDISPLAY_32`).
좌표는 해당 `ui_MainScreen.c` 를 **읽고**, 레이아웃 변경은 squareline-ui 스킬 순서를 따른다.
에이전트는 SquareLine을 실행하지 못하므로 숨김·라인 연장은 `ui_init()` 이후 `src/` 에서 한다.
생성 `ui_*.c` 직접 수정은 긴급·사용자 지시일 때만.

## 작업 순서

상태 비트 변경·무력화, 또는 "입력 차단기를 빼 달라" 같은 요청이 오면:

1. `mapping.md`에서 비트/엑셀 이름 → 코드 필드 → UI 위젯을 찾는다.
2. 위젯이 **별도 오브젝트**인지, **배경 PNG(`ui_img_flowmain_png`)** 인지 구분한다.
3. UI만 숨길지, 로그 마스크까지 끌지, 모드버스 수신 비트까지 무시할지 범위를 정한다.
   사용자가 범위를 안 주면 **화면만** 바꾸고, 로그/통신은 건드리지 않는다.
4. 전력선 빈 공간이 생기면 [hardware-options.md](hardware-options.md) 대로
   `ui_init()` 이후 `lv_obj_set_width` / `lv_obj_set_x` 로 인접 라인을 연장한다.
   생성 `ui_MainScreen.c` 는 기본으로 고치지 않는다.
5. 비트 무력화 시:
   - 화면: `mainScrUpdata()` 조건에서 해당 비트를 빼거나 항상 정상색으로 고정
   - 로그: `upsLog` 생성자의 `mask_*` 해당 비트를 0
   - 문자열: `lib/upsLog/src/upsLog.h` 배열과 `src/resource/*.yml`
6. 4.3/3.2 위젯 이름·좌표가 다르면 각각 수정한다.

## 엑셀 정의가 바뀌면

사용자가 "작화 엑셀이 바뀌었다"고 하면:

1. `python .cursor/skills/ups-display-modbus-ui/scripts/dump_xls.py` 실행
2. 새 `dumps/memory_map.tsv` 와 기존 스냅샷·`mapping.md` 를 비교한다.
3. 바뀐 번지/비트를 `mapping.md`에 반영한다.
4. 코드 필드(`mainGrobal.h`), `mainScrUpdata()`, `scrMeasureLoad()`,
   `upsLog` 문자열/마스크를 같은 순서로 맞춘다.
5. 엑셀에만 있고 코드에 없는 항목은 **미구현**으로 표시하고, UI를 새로 만들지
   사용자에게 확인한다.

## 금지

- 엑셀 MW 번지를 현재 펌웨어 번지로 그대로 쓰지 말 것 (맵이 다르다).
- 생성 UI를 몰라도 `ui_Image1`(흐름도 PNG)만 지우고 끝내지 말 것.
  NFB가 PNG에 그려져 있으면 라인 패널 연장이 같이 필요하다.
- 사용자 요청 없이 모드버스 프로토콜(FC04/FC10) 자체를 바꾸지 말 것.
