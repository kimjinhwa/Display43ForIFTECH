---
name: squareline-ui
description: >-
  SquareLine Studio UI source policy for this display firmware. Use when editing
  screens, widgets, ui_MainScreen.c, ui_disp43, ui_disp32, LVGL layout, hiding
  NFB/차단기, extending power lines, copying SquareLine export, or when the user
  mentions SquareLine, ui_init, or generated UI files.
---

# SquareLine UI 수정 원칙

SquareLine을 사용해서 고친 소스는 가급적 손대지 않는다.
왜냐하면, 라이브러리에 포함된 ui소스는 SquareLine에서 재 생성 할 때마다 변경되고, 난 이것을 통째로 복사를 하거든.
단지 ui_events.cpp 라는지 이 프로그램에 종속되는것은 삐고 그대로 복사하는것이지.

그래서 사실 ui_MainScreen.c 를 바꿔야 한다면, SquareLine에 적용을 하고, 다시 생성해서 적용을 하는게 맞겠지. 하지만 급하게 처리하다보면, ui_MainScreen.c를 바로 수정해는 상황이 발생하는것이야.
따라서 이 부분은 원칙은 SquareLine을 사용하고, 필요시 ui_MainScreen.c 를 직접 수정하고, 나중에 이것을 SquareLine에 반영하야햐 하며, SquareLine에 반영해서 풀 수 없는 문제에 한해서, ui_MainScreen.c를 수정하거나, main에서 로드 한 후 속성값을 변경하는 방식이면 좋을것 같아.

## 우선순위 (에이전트는 이 순서로만 고친다)

1. **SquareLine → 재생성 → 라이브러리 통째 복사**  
   모든 제품이 공유하는 작화(위젯 추가/삭제, 기본 좌표, 이미지, 화면 구조).
2. **`ui_init()` 이후 `src/` 에서 속성 변경**  
   SquareLine이 표현하지 못하는 것, 또는 **제품 옵션**(이 장비만 차단기 없음).
   재생성·통째 복사에 안 덮인다. 기본 구현 위치: `src/main.cpp` (`ui_init()` 다음),
   `src/ui_events.cpp`, `src/myui.c`.
3. **생성 파일 직접 수정 (긴급)**  
   `lib/ui_disp43/src/screens/ui_MainScreen.c` 등. 반드시 나중에 SquareLine에
   반영할 빚이다. 채팅과 코드 주석에 위젯·속성·의도한 SquareLine 작업을 남긴다.

에이전트가 SquareLine Studio를 실행할 수 없으면 1번을 사용자에게 맡긴다.
그 경우 **기본은 2번**. 사용자가 “생성 파일 직접 고쳐라”고 하거나 2번으로
안 되면 3번.

## 복사할 때 / 손대지 말 것

통째 복사 대상 (생성물):

- `lib/ui_disp43/` 또는 `lib/ui_disp32/` 의 `screens/`, `images/`, `fonts/`,
  `ui.c`, `ui.h`, `ui_helpers.*`
- SquareLine이 만든 스텁 `lib/ui_dispXX/src/ui_events.cpp` (보통 빈 함수)

복사에서 빼는 프로그램 종속 파일:

- `src/ui_events.cpp` — 실제 이벤트·계측·설정 로직. **여기가 구현체.**
- `src/main.cpp`, `src/myui.c`, `src/modbusRtu.cpp` 등 `src/` 전체

`lib/.../ui_events.h` 의 선언은 SquareLine이 이벤트를 추가하면 갱신된다.
구현은 계속 `src/ui_events.cpp` 에 둔다. 라이브러리 스텁에 로직을 넣지 않는다.

`ui_init()` 호출은 `src/main.cpp` (`Guide.md`: 라벨 등 컴포넌트는 `ui_init()` 후 적용).

## SquareLine로 할 일 vs 런타임으로 할 일

| SquareLine (1) | `ui_init()` 이후 src (2) |
|----------------|--------------------------|
| 새 위젯·화면·이미지 | 숨김, 폭/좌표 연장, 재색 |
| 모든 파생 제품이 같은 레이아웃 | 이 장비만 다른 옵션 (입력 NFB 없음) |
| 폰트·기본 텍스트 | 모드버스 값으로 라벨/색 갱신 (`mainScrUpdata`) |
| 이벤트 핸들러 **연결** | 핸들러 **본문** (`src/ui_events.cpp`) |

제품 옵션(충전기에 입력 차단기 없음)은 SquareLine 공통 작화를 유지하고
2번으로 숨김+라인 연장하는 편이, UPS/충전기가 같은 export를 공유할 때 안전하다.

## 긴급으로 생성 파일을 고친 뒤

체크리스트를 남기고 끝내지 않는다.

```
SquareLine 반영 빚:
- 파일: lib/ui_disp43/src/screens/ui_MainScreen.c
- 위젯: ui_pnlMainPower1
- 변경: width, x
- SquareLine에서: 동일 위젯 크기/위치 맞추기
```

다음 SquareLine 통째 복사 전에 이 빚을 1번으로 갚거나, 2번으로 옮겨
생성 파일 수정을 되돌린다.

## 금지

- 생성 `screens/*.c`, `ui.c`, 이미지 C 배열을 기본 수정 경로로 쓰지 말 것.
- `lib/ui_dispXX/src/ui_events.cpp` 스텁에 프로그램 로직을 넣지 말 것.
- 생성 파일을 고치고 SquareLine 반영 빚을 남기지 않은 채 끝내지 말 것.
