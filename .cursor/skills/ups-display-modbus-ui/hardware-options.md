# 하드웨어 옵션 — 차단기·전력선 UI 변경

장비에 부품이 없으면 화면에서도 빼고, 빈 구간은 앞 전력선을 연장하거나
대체 컴포넌트로 잇는다. 모드버스 맵은 사용자가 요청하기 전에는 유지한다.

레이아웃 파일 수정 순서는 `.cursor/skills/squareline-ui/SKILL.md` 를 따른다.
에이전트 기본: **생성 `ui_MainScreen.c` 를 고치지 않고** `ui_init()` 다음 `src/` 에서
숨김·폭·좌표를 바꾼다. 공통 작화 자체 변경은 사용자에게 SquareLine 재생성·통째 복사를 요청한다.

## 공통 절차

1. [mapping.md](mapping.md) 섹션 6–7에서 대상 이름 → 위젯을 찾는다.
2. 위젯 종류를 나눈다.
   - **패널 라인** (`ui_pnl*`): `lv_obj_set_width` / `lv_obj_set_height` / `lv_obj_set_x` / `lv_obj_set_y`
   - **이미지 블록** (`ui_img*`): `lv_obj_add_flag(..., LV_OBJ_FLAG_HIDDEN)` 또는 소스 교체
   - **배경 작화** (`ui_Image1` / `ui_img_flowmain_png`): NFB가 PNG에만 있으면
     라인 패널로 덮거나, PNG를 교체한다. 픽셀 편집이 없으면 흰색/라인 패널로 가린다.
3. `mainScrUpdata()` 가 숨긴 위젯 색을 계속 바꾸면 문제 없다. 다만 연장한 라인은
   **같은 구동 비트**를 유지해야 통전색이 끊기지 않는다.
4. 적용 위치는 `src/main.cpp` 의 `ui_init()` 직후(또는 전용 `applyHardwareOptions()`).
   좌표 값은 해당 디스플레이의 `ui_MainScreen.c` 를 **읽어서** 쓰되, 그 파일은 저장하지 않는다.
   4.3과 3.2 기본 좌표가 다르면 `#ifdef DISPLAY_43` 로 나눈다.

숨김:

```c
lv_obj_add_flag(ui_대상, LV_OBJ_FLAG_HIDDEN);
```

라인 연장 (수평): 폭을 늘리고, `LV_ALIGN_CENTER`이면 중심이 고정되므로
빠진 심볼 쪽으로 `x`를 이동해 빈칸을 메운다.

---

## 레시피: 입력 차단기(INPUT NFB) 제거

사용자 예: "UI에서 입력 차단기 스위치를 제거하고, 바로 앞에 있는 입력 라인의 길이를 연장해줘"

### 현재 상태

- 전용 위젯 `ui_imgInputNfb` **없음**.
- 입력 → 컨버터 선: `ui_pnlMainPower1` (4.3" 기준 w=73, x=-198, y=5).
- 컨버터: `ui_imgConvertor` (x=-137, y=4).
- NFB 그림이 보이면 `ui_img_flowmain_png` 왼쪽 입력단에 그려진 것이다.

### 작업

1. `ui_MainScreen.c`를 읽어 입력단 위젯을 찾는다. 별도 NFB 오브젝트가 있으면
   `ui_init()` 이후 `LV_OBJ_FLAG_HIDDEN`. 생성 파일은 수정하지 않는다.
2. 없으면 `ui_pnlMainPower1`을 컨버터 왼쪽까지 **런타임으로** 연장한다.
   - 4.3" 목표: 라인 오른쪽이 `ui_imgConvertor` 왼쪽(약 x=-137, 폭 41)에 닿게.
   - 폭을 키우고 x를 오른쪽으로 옮겨 갭을 메운다. 예: width 73→110 수준으로
     실제 갭을 본 뒤 조정. PNG NFB 위를 라인이 덮어야 한다.
3. `ui_pnlMainPower2` (수직 분기)가 NFB 위치에 묶여 있으면 x를 입력 쪽으로
   유지할지, 라인과 같이 옮길지 레이아웃을 보고 결정한다. 기본은 **분기점 유지**.
4. `mainScrUpdata()` 의 17.7 `utility_line_failure` 색 제어는 그대로 둔다.
5. 엑셀 MB172 / MW12 Input_NFB 알람 비트를 **화면에서만** 빼는 것이다.
   인버터가 그 비트를 계속 보내도 이 펌웨어는 원래 그래픽 접점이 없다.
6. 배터리 차단기(`BAT_MCCB_Fault`)와 혼동하지 말 것.
7. 이 레이아웃을 모든 제품의 기본 작화로 만들 때만 SquareLine에서 고치고 재생성한다.

### 완료 확인

- 입력 라인 색이 정전 시 세 패널(`MainPower1/2/3`) 모두 바뀐다.
- 컨버터 아이콘과 입력 라인 사이에 차단기 심볼이 보이지 않는다.
- 바이패스 수평선 `ui_pnlMainPower3` 길이는 건드리지 않는다. 사용자가
  바이패스 NFB까지 빼라고 한 경우에만 연장한다.

---

## 레시피: 다른 차단기

| 요청 | 인접 라인 | 비고 |
|------|-----------|------|
| 출력 NFB 제거 | `ui_pnlInvPower1` + `ui_pnlInvPower4` | SCR 뒤 ~ 부하 |
| 바이패스 NFB 제거 | `ui_pnlMainPower3` 폭 연장 | Bypass SCR `ui_imgScrBypass` 앞 |
| 축전지 NFB 제거 | `ui_pnlConvPower3` 높이 연장 | `ui_imgBattery` 위. 알람 16.14는 별도 |
| 유지보수 NFB 제거 | PNG/미구현 | 위젯 없으면 PNG만 처리 |
| 입력 M/C 제거 | `ui_pnlMainPower1` | INPUT NFB와 같은 구간일 수 있음 |

축전지 NFB를 **알람까지** 끄려면 `upsLog` `mask_HwStatusEvent` 의 bit14를 0으로 하고
`Battery_MCCB_Fault` 문자열을 비운다. 화면만 빼라는 요청이면 마스크는 유지.

---

## 레시피: 상태 비트 무력화

예: "정전 비트를 무시해라", "BAT MCCB 알람을 내지 마라"

범위 기본값 = **화면만**. 로그/통신은 명시될 때만.

| 범위 | 할 일 |
|------|--------|
| 화면 | `mainScrUpdata()` 조건을 항상 정상 분기로 고정하거나 비트 비교를 삭제 |
| 로그 | `lib/upsLog/src/upsLog.cpp` 생성자 `mask_*` 해당 비트 클리어 |
| 수신 무시 | `modbusRtu.cpp` 에서 해당 비트를 0으로 강제. 프로토콜 번지는 유지 |
| 문자열 | `upsLog.h` 배열 + `src/resource/*.yml` |

정전(17.7)을 화면에서 무시하면 `ui_pnlMainPower1/2/3` 는 항상 `ONLINE_COLOR`.
실제 정전이어도 입력선이 켜져 보인다. 사용자에게 그 효과를 한 줄로 알린다.

---

## 레시피: 전력선만 연장/단축

위젯을 지우지 않고 길이만 바꿀 때.

- 수평: `lv_obj_set_width`, CENTER면 `x`도 같이 이동.
- 수직: `lv_obj_set_height`, `y` 이동.
- 통전색은 기존 비트를 유지. 새 패널을 만들면 `mainScrUpdata()` 에
  같은 `lv_obj_set_style_bg_color` 를 추가한다.

새 패널이 필요하면 SquareLine에서 만들고 재생성한다. 에이전트가 생성
`ui.h` / `ui_MainScreen.c` 에 위젯을 새로 넣지 않는다. 기존 패널 연장만 2번(런타임)으로 한다.

---

## 레시피: 작화 PNG 교체

`ui_img_flowmain_png` / `ui_img_flowmain1_png` 를 바꾸면
`lib/ui_disp43/src/images/` (및 disp32) C 배열을 재생성해야 한다.
라인 패널 좌표는 PNG 위의 버스 위치에 맞춰 재측정한다.
PNG만 바꾸고 패널을 안 옮기면 통전선이 심볼과 어긋난다.
