# Modbus 번지 · 상태 비트 · UI 매핑

기준 코드: `src/mainGrobal.h` `ups_modbus_data_t`.
엑셀: `doc/UPS 335 단독_V6.4_작화 데이터 정리(20140228).xls`
시트 `사용중인 전체 메모리번지 정리(최종)`.

색상 (`src/main.cpp`):

- `ONLINE_COLOR` `0x379B57` 통전
- `OFFLINE_COLOR` `0xD2EBF6` 단전
- `ONSCR_COLOR` `0xFF0000` SCR ON
- `ONCONV_BG_COLOR` 컨버터/배터리 ON 배경

읽기: FC04. 쓰기: FC10 (`enqueueModbusCommand`).

---

## 1. 워드 레지스터 → 화면

| Addr | 필드 | FC | 화면 | 위젯 | 비고 |
|------|------|----|------|------|------|
| 0 | `Nominal_Capacity` | 04 | Init | `ui_lblCapacity` | /10.0 kVA |
| 1 | `Nominal_InputVoltage` | 04 | Init | `ui_lblInputVol` | V |
| 2 | `Nominal_OutputVoltage` | 04 | Init | `ui_lblOutputVol` | V |
| 3 | `Nominal_BatVoltage` | 04 | Init | `ui_lblBatteryCellNo` | 셀/공칭 |
| 6 | `upsRun` | 04/10 | Control | `ui_btnRunUps`, `ui_btnStopUps1` | 비트 필드 |
| 8 | `Bat_Current_Ref` | 04/10 | Setting | `ui_txtBatCurrSet` | |
| 9 | `Bat_Voltage_Ref` | 04/10 | Setting | `ui_txtBatVolSet` | |
| 10 | `Output_Voltage_Ref` | 04/10 | Setting | `ui_txtOutputVolSet` | |
| 11 | `HF_MODE` | 04/10 | Setting | `ui_txtHFMnBatFirstFaultUV` | 0 Normal, 1 HF |
| 15 | `ModuleState` | 04 | Event | 로그 문자열 | 이벤트 전용 |
| 16 | `HWState` | 04 | Main+Alarm | 전력흐름 + 알람 | 아래 비트표 |
| 17 | `upsOperationFault` | 04 | Main+Alarm | 입력라인/정전 + 알람 | 아래 비트표 |
| 20 | `Input_volt_rms` | 04 | Measure/Main | `ui_lblInputVoltRms` | |
| 21 | `Input_current_rms` | 04 | Measure | `ui_lblInputCurrRms` | |
| 22 | `vdc_link_volt_rms` | 04 | Measure/Main | `ui_lblVdcLinkVoltRms`, `ui_lblDcLinkVol` | |
| 23 | `bat_volt_rms` | 04 | Measure/Main | `ui_lblBatVoltRms`, `ui_lblBatteryVol` | |
| 24 | `bat_current_rms` | 04 | Measure | `ui_lblBatCurrentRms` | signed |
| 25 | `inverter_volt_rms` | 04 | Measure/Main | `ui_lblInvVoltRms`, `ui_lblInverterVol` | |
| 26 | `inverter_current_rms` | 04 | Measure | `ui_lblInvCurrentRms` | |
| 27 | `output_volt_rms` | 04 | Measure/Main | `ui_lblOutputVoltRms`, `ui_lblOutputV` | |
| 28 | `output_current_rms` | 04 | Measure | `ui_lblOutputCurrRms` | |
| 29 | `conv_Frequency` | 04 | Measure | `ui_lblInputFreq` | /10.0 Hz |
| 30 | `inv_Frequency` | 04 | Measure | `ui_lblInvFreq` | /10.0 Hz |
| 31 | `output_Frequency` | 04 | Measure | `ui_lblOutputFreq` | /10.0 Hz |
| 32 | `battery_capacity` | 04 | Main/Measure | `ui_lbaBatCapacity`, `ui_lblBattCapacity` | % |
| 33 | `load_percentage` | 04 | Main | `ui_lblLoadCapacity` | % |
| 34 | `inv_internal_Temperature` | 04 | Measure | `ui_lblUpsInnerTemp` | |
| 38 | `input_volt_gain` | 04/10 | Setting | `ui_txtInputVoltGain` | |
| 39 | `input_current_gain` | 04/10 | Setting | `ui_txtInputCurrGain` | |
| 40 | `vdc_link_volt_gain` | 04/10 | Setting | `ui_txtInputVdcLinkGain` | |
| 41 | `vbat_volt_gain` | 04/10 | Setting | `ui_txtVbatVoltGain` | |
| 42 | `bat_current_gain` | 04/10 | Setting | `ui_txtBatCurrGain` | |
| 43 | `inverter_volt_gain` | 04/10 | Setting | `ui_txtInvVoltGain` | |
| 44 | `inverter_current_gain` | 04/10 | Setting | `ui_txtInvCurrGain` | |
| 46 | `output_current_gain` | 04/10 | Setting | `ui_txtOutputCurrGain` | |
| 50–58 | `*_offset` | 04/10 | Measure | `ui_txt*Offset` | |
| 60–65 | RTC | 04/10 | Setting | `ui_txtYear`…`ui_txtSecond` | 로컬 RTC |
| 66–84 | 통신/IP/밝기 | 04/10 | Setting | NVS `nvsSystemEEPRom` | EEPROM |

4–5, 7, 12–14, 18–19, 35–37, 45, 47–49, 59 는 reserved. UI 없음.

---

## 2. Addr 6 `upsRun` 명령 비트

쓰기 대상. 화면 제어 버튼.

| Bit | 필드 | 위젯 | 동작 |
|-----|------|------|------|
| 0 | `UpsON` | `ui_btnRunUps` | XOR 후 addr 6 FC10 |
| 1 | `UpsOFF` | `ui_btnStopUps1` | XOR 후 addr 6 FC10 |
| 2 | `ALARM_RESET` | 알람 리셋 | 알람 화면 |

버튼 색은 addr 16 운전접점으로 결정:
CONV 또는 DCDC 또는 INV ON → Run 빨강, Stop 회색. 아니면 반대.

---

## 3. Addr 15 `ModuleState` 이벤트 비트

메인 그래픽을 직접 켜지 않는다. `upslogEvent` 에만 기록.
엑셀 MW16 (충전기/인버터 운전 이벤트)에 해당.

| Bit | 필드 | 로그 키 |
|-----|------|---------|
| 0 | `Charger_RUN` | `R_C_Start_Run_Event` |
| 1 | `Charger_ReRUN` | `R_C_Re_Start_Event` |
| 2 | `Charger_STOP` | `R_C_Stop_by_Command_Event` |
| 3 | `Charger_FaultSTOP` | `R_C_Stop_by_Fault_Event` |
| 4 | `Charger_DCDC_RUN` | `DC_DC_Start_Run_Event` |
| 5 | `Charger_DCDC_ReRUN` | `DC_DC_Re_Start_Event` |
| 6 | `Charger_DCDC_STOP` | `DC_DC_Stop_by_Command_Event` |
| 7 | `Charger_DCDC_FaultSTOP` | `DC_DC_Stop_by_Fault_Event` |
| 8 | `Inverter_RUN` | `Inverter_Start_Run_Event` |
| 9 | `Inverter_ReRUN` | `Inverter_Re_Start_Run_Event` |
| 10 | `Inverter_STOP` | `Inverter_Stop_by_Command_Event` |
| 11 | `Inverter_FaultSTOP` | `Inverter_Stop_by_Fault_Event` |
| 12 | `To_Inverter_ModeChange` | `Transfer_to_Inverter_Event` |
| 13 | `To_Bypass_ModeChange` | `Transfer_to_Bypass_by_Com_Event` |
| 14 | `To_Bypass_ModeChange_ForFault` | `Transfer_to_Bypass_by_fault_Event` |
| 15 | `IInverter_over_load_over_time` | (빈 문자열) |

이벤트 마스크: `0b0111111111111111` (bit15 제외).
알람 로그는 ModuleState 마스크 0.

---

## 4. Addr 16 `HWState` — 메인 그래픽 + 알람

`mainScrUpdata()` 가 이 워드로 전력흐름을 그린다.

| Bit | 필드 | 화면 위젯 | 동작 | 로그 |
|-----|------|-----------|------|------|
| 0 | `Input_OC` | 없음 (알람만) | 1=fault | `Input_OC` |
| 1 | `Inverter_OC` | 없음 | 1=fault | `Inverter_Over_Current_fault` |
| 2 | `Vdc_OV` | 없음 | 1=fault | `DC_Link_OV_fault` |
| 3 | `CONVERTER_RUN_STOP_STATE` | `ui_imgConvertor`, `ui_pnlConvPower1`, `ui_pnlConvPower2` | 1=CONV ON: 아이콘 ON색, 라인 ONLINE. 0=OFF색. DCDC OFF면 Power2도 OFF | 로그 제외 |
| 4 | `DC_DC_CONVERTER_RUN_STOP_STATE` | `ui_imgBattery`, `ui_pnlConvPower3` | 1=DCDC ON: 배터리 ON, Power3 ONLINE | 로그 제외 |
| 5 | `Conv_GDU` | 없음 | 1=fault | `Converter_GDU_fault` |
| 6 | `Inv_GDU` | 없음 | 1=fault | `Inverter_GDU_fault` |
| 7 | `GDU_DCDC` | 없음 | 1=fault | `DC_DC_GDU_fault` |
| 8 | `Com_GDU` | 없음 | 1=fault | `Common_GDU_fault` |
| 9 | `INVERTER_RUN_STOP_STATE` | `ui_imgInvertor`, `ui_pnlInvPower` | 1=INV ON | 로그 제외 |
| 10 | `BAT_FUSE` | 없음 | 1=fault | `Battery_Fuse_fault` |
| 11 | `Module_OT` | 없음 | 1=fault | `Module_OT` |
| 12 | `Buzz_ON_OFF` | `ui_btnAlarm` + MCP 부저 | 토글 시 `runBuzzStatus` 반전 | 로그 제외 |
| 13 | `EEPROM_ERR` | 없음 | 1=fault | `EEPROM_ERR` |
| 14 | `BAT_MCCB_Fault` | 없음 (전용 NFB 위젯 없음) | 1=fault | `Battery_MCCB_Fault` |
| 15 | `TRANSFER_RUN_STOP_STATE` | `ui_imgScrBypass`, `ui_imgScrOutput`, `ui_pnlInvPower1`–`4` | **0=바이패스**: Bypass SCR ON, Output SCR OFF, Power2/3/4 ON, Power1 OFF. **1=인버터**: 반대, Power1/4 ON | 로그 제외 |

HW 알람 마스크 `0b0110110111100111` = bits 0,1,2,5,6,7,8,10,11,13,14.
운전접점 3,4,9,12,15 는 알람으로 안 남긴다.

---

## 5. Addr 17 `upsOperationFault`

| Bit | 필드 | 화면 | 로그 |
|-----|------|------|------|
| 0 | `converter_current_limit_fault` | 알람 | 동명 |
| 1 | `Vdc_link_OV_limit_fault` | 알람 | 동명 |
| 2 | `UV_limit_fault` | 알람 | `Vdc_link_UV_limit_fault` |
| 3 | `input_under_voltage_fault` | 알람 | 동명 |
| 4 | `input_over_voltage_fault` | 알람 | 동명 |
| 5 | `Input_Frequency_fault` | 알람 | 동명 |
| 6 | `Battery_UV_Primary_Fault` | 알람 | 동명 |
| 7 | `utility_line_failure` | **입력 전력선** `ui_pnlMainPower1/2/3` | `utility_line_failure` |
| 8 | `battery_current_limit` | 알람 | `battery_current_limit_fault` |
| 9 | `Battery_OV_limit_fault` | 알람 | 동명 |
| 10 | `battery_UV_limit_fault` | 알람 | 동명 |
| 11 | `inverter_output_voltage_fault` | 알람 | 동명 |
| 12 | `output_OC_overtime_fault` | 알람 | `Output_Overload` |
| 13 | `inverter_OL_STOP` | 알람 | `Inverter_Overload_Stop` |
| 14 | `OFFSET_CHECK_ERR` | 알람 | `OFFSET_Check_ERR` |
| 15 | reserved | 없음 | |

Bit7=1 이면 입력 라인 3개를 `OFFLINE_COLOR`. 0이면 `ONLINE_COLOR`.
바이패스 경로(`pnlMainPower2/3`)도 같은 비트로 같이 꺼진다.

알람 마스크 `0b0111111111111111` (bit15 제외).

---

## 6. 메인 전력흐름 위젯 (4.3" `ui_MainScreen.c`)

부모: `ui_Container4`. 배경: `ui_Image1` ← `ui_img_flowmain_png` (476×234).
NFB/MCCB 심볼이 별도 위젯이 아니면 이 PNG에 그려져 있다.

좌표는 LV_ALIGN_CENTER 기준(InvPower1–4는 LEFT_MID).

| 위젯 | 역할 | 크기(w×h) | x,y | 구동 비트 |
|------|------|-----------|-----|-----------|
| `ui_pnlMainPower1` | 입력 → 컨버터 수평선 | 73×14 | -198, 5 | 17.7 정전 |
| `ui_pnlMainPower2` | 입력 수직(바이패스 분기) | 14×76 | -205, -40 | 17.7 |
| `ui_pnlMainPower3` | 바이패스 수평선 | 306×14 | -57, -71 | 17.7 |
| `ui_imgConvertor` | 충전기 AC/DC | 이미지 | -137, 4 | 16.3 CONV |
| `ui_pnlConvPower1` | CONV → DC버스 | 60×14 | -82, 5 | 16.3 |
| `ui_pnlConvPower2` | DC버스 → INV | 75×14 | -30, 5 | 16.3, 16.4 |
| `ui_pnlConvPower3` | DC버스 → BAT 수직 | 14×32 | -59, 28 | 16.4 DCDC |
| `ui_imgBattery` | 배터리 | 40×47 | -59, 67 | 16.4 |
| `ui_imgInvertor` | 인버터 DC/AC | 47×47 | 32, 4 | 16.9 INV |
| `ui_pnlInvPower` | INV → 출력 SCR | 40×14 | 77, 6 | 16.9 |
| `ui_imgScrBypass` | 바이패스 SCR | 38×30 | 115, -71 | 16.15=0 이면 ON |
| `ui_imgScrOutput` | 출력 SCR | 38×30 | 116, 6 | 16.15=1 이면 ON |
| `ui_pnlInvPower1` | 출력 SCR 뒤 수평 | 29×14 | 391, 6 LEFT | 16.15 |
| `ui_pnlInvPower2` | 바이패스 SCR 뒤 수평 | 42×14 | 391, -72 | 16.15 |
| `ui_pnlInvPower3` | 출력 수직 합류 | 14×77 | 419, -39 | 16.15 |
| `ui_pnlInvPower4` | 부하 수평 | 36×14 | 418, 6 | 16.15 양쪽 ON |
| `ui_lbaBatCapacity` | BAT % | | 109, 53 | addr 32 |
| `ui_lblLoadCapacity` | LOAD % | | 109, 76 | addr 33 |
| `ui_btnAlarm` | 알람 아이콘 | | | 알람 이력 + 부저 |

**현재 코드에 없는 전용 위젯:** INPUT NFB, OUTPUT NFB, BATTERY NFB, BYPASS NFB, MAINTENANCE NFB, INPUT M/C, INVERTER M/C.
엑셀 MW17 그래픽 접점은 펌웨어에 1:1 오브젝트가 없다.

3.2"는 같은 위젯 이름이 `lib/ui_disp32` 에 있다. 좌표는 다르다. 라인 연장 시 반드시 해당 `ui_MainScreen.c` 값을 읽는다.

---

## 7. 엑셀 335 HMI → 현재 펌웨어 (이름 번역)

사용자가 엑셀/구작화 이름으로 말하면 여기로 찾는다.

### 그래픽 접점 (엑셀 MW17 / 3x bit 1700–)

| 엑셀 | 의미 | 현재 펌웨어 |
|------|------|-------------|
| MB170 INPUT M/C | 입력 전자접촉기 | 미구현. 입력선 `ui_pnlMainPower1` 만 존재 |
| MB171 OUTPUT NFB | 출력 차단기 | 미구현. 출력선 `ui_pnlInvPower1/4` |
| MB172 INPUT NFB | 입력 차단기 | **미구현 전용 위젯.** PNG 또는 입력선 앞 구간 |
| MB173 BATTERY NFB | 축전지 차단기 | 알람만 `HWState.BAT_MCCB_Fault` (16.14) |
| MB174 INVERTER M/C | 인버터 접촉기 | `INVERTER_RUN_STOP_STATE` (16.9) 아이콘으로 대체 |
| MB175 BYPASS NFB | 바이패스 차단기 | 미구현. 바이패스선 `ui_pnlMainPower3` |
| MB176 MAINTENANCE NFB | 유지보수 차단기 | 미구현 |
| MB177 CONVERTER | 충전기 블록 | `CONVERTER_RUN_STOP_STATE` + `ui_imgConvertor` |
| MB178 INVERTER | 인버터 블록 | `INVERTER_RUN_STOP_STATE` + `ui_imgInvertor` |
| MB179 SCR | 스태틱 스위치 | `TRANSFER_RUN_STOP_STATE` + `ui_imgScrBypass/Output` |
| MB17D 입력분리 추가 NFB | 추가 입력 차단기 | 미구현 |

### 알람/이벤트 워드 (엑셀 MW12–16)

3상 335의 MW12–15 다수 비트(입력 NFB 열림, 상회전, 결상, 바이패스 UV 등)는
단상 `HWState`/`upsOperationFault` 에 없다. 없으면 mapping에 **미구현**으로 두고
새 비트를 만들기 전에 사용자에게 확인한다.

대략 대응:

| 엑셀 | 현재 |
|------|------|
| MW16 충전기/인버터 운전 이벤트 | Addr 15 `ModuleState` |
| MW14 정전(14B) | Addr 17.7 `utility_line_failure` |
| MW13 축전지 퓨즈(132) | Addr 16.10 `BAT_FUSE` |
| MW14 축전지 NFB 이상(140) | Addr 16.14 `BAT_MCCB_Fault` |
| MW12 옵셋체크(12C) | Addr 17.14 `OFFSET_CHECK_ERR` |
| MW14 입력 UV/OV/주파수 | Addr 17.3 / 17.4 / 17.5 |
| MW15 인버터 과부하 정지/출력전압/과부하 | Addr 17.13 / 17.11 / 17.12 |
| MW27–33 입력 RST 계측 | 단상 Addr 20–21, 29 로 축약 |
| MW67 운전 명령 | Addr 6 `upsRun` (ON/OFF/RESET으로 축약) |

### 엑셀 터치 내부 (LB/LW)

HMI 매크로용. ESP32 펌웨어에 대응 번지 없음.
절전시간·언어·부저는 NVS/`nvsSystemEEPRom` + Setting 탭.

---

## 8. 로그 UI

- 이벤트: `ui_eventTextArea` ← `upslogEvent`
- 알람: `ui_alarmTextArea` ← `upslogAlarm`
- 문자열 키: `lib/upsLog/src/upsLog.h` 배열 → `src/resource/ko-KR.yml`, `en-GB.yml`
- 비트를 무력화하면 해당 마스크 비트를 0으로 하고, 빈 문자열이면 로그에 안 나온다.

---

## 9. 소스 파일 체크리스트

비트/위젯을 바꾸면 이 순서로 검색한다.

1. `src/mainGrobal.h` — 비트 필드 주석
2. `src/modbusRtu.cpp` — `_CoilData` 동기화, FC04 폴링
3. `src/main.cpp` — `mainScrUpdata()`, 정격/용량 라벨
4. `src/ui_events.cpp` — `scrMeasureLoad()`, `scrSettingScreen()`, 버튼 이벤트
5. `lib/upsLog/src/upsLog.h` — 로그 키, 마스크
6. `lib/ui_disp43/src/screens/ui_MainScreen.c` — 좌표/생성
7. `lib/ui_disp32/src/screens/ui_MainScreen.c` — 3.2" 좌표
8. `lib/ui_disp43/src/ui.h` — `extern lv_obj_t *`
