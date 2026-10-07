#define VERSION "4.0.40"
/*
지금 화면에서 전력라인이 보인다. 
전력이 흐르지 않을때를 표현하지 위해 그림파일에 전력선을 미리 그려 놓았었는데, 
그림파일에서 이 부분을 제거 하겠다. 
C:\DevWork\4.IFTechWork\1.UPS1P1P\Display3.2  이쪽에서는 
보더로 사용을 했다. 
그래서 여기서도 그런 방법을 사용하고 싶다. 
즉 pnlMainPower1의 경우 보더에 흰색을 보여 주고 
통전이 되면, 내부에 색을 칠한다. 
pnlMainPower2는 역시 색이 칠해지는데, 이때 보더의 아랫쪽 보더는 보이지 않아야 한다. 그래야 전기가 흐르는 모습이 보일것이다. 
다시 pnlMainPower3 라인이 그려지고, 이때는 이 라인늬 왼쪽 끝 보더가 보이지 않아야 할것이다. 
pnlInvPower2.pnlInvPower3.pnlInvPower4 이와 같은 식이고, 
pnlConvPower1, pnlConvPower2, pnlConvPower3도 이런식으로 되어야 겠다.
pnlConvPower1은 오른쪽 보더를 숨기고, pnlConvPower2는 왼쪽 보더, 
pnlConvPower3은 위쪽 보더일것이다. 
맞는지 확인하고 구현해줘.
*/
//efine VERSION "4.0.35"
/*
1. 15번지의 14,11,7,3은 이벤트에만 사용했는데, 이것들을 경보화면에도 넣어주자.
2. 로그 버튼을 클릭해서 경보창 혹은 이벤트창을 보여 주게 되어 있는데, 
    이벤트의 경우는 나갔다 들어 갈때는 가장 마지막 페이지를 보여 줄 수 있게 하자. 
    사용자는 이벤트를 볼때 마지막을 위주로 보고 싶어한다.
3. 패스워드 최대 입력갯수는 4개 이상입력되지 않게 하자.
4. 이제부터 RTC에 제어권을 넘깁니다. 메세지를 뿌리고 나서, 이후의 시리얼 로그는 더 이상 출력하지 않아야 하지? 
5. "MAIN" 과 "OFFSET" 번역 문제. 
6. 비밀번호 키보드 입력시 한번에 2글자가 찍히는 문제. 
*/
//#define VERSION "4.0.30"
/*
 event로그를 앱에서 확인하고, 전송할수 있게 한다.
*/
//#define VERSION "4.0.24"
/*
 파티션 복사 파일을 정리한다. 
*/
//#define VERSION "4.0.19"
/*
 앱을 이용한 시간 설정, 최신 펌웨어 업데이트 기능 추신등을 확인한다.
*/
//#define VERSION "4.0.4"
/* 
 여기서 하드웨어를 변경하여 변경한다. 
 시리얼 통신 TX를 사용하여 RTC를 사용한다.
*/
//#define VERSION "3.0.22"
/*
여기에도 반영해줘.

출력 CT 에러(17.15)를 펌웨어·로그·UI에 넣겠습니다. 먼저 스킬과 기존 상태 비트 처리 방식을 확인합니다.

17.15를 reserved에서 출력 CT 에러로 바꾸고, 알람 마스크·문자열·번역을 4.3과 3.2에 같이 넣습니다.
*/
//#define VERSION "3.0.21"
/* 
키보드를 충전기와 같이 인터페이스를 변경했다.
*/
//#define VERSION "3.0.21"
/* WiFi OTA: IIS Esp32UploadFirmware + IFTECH_DISP43.json 파이프라인 */
//#define VERSION "3.0.0"
/* 새로운 보드에서 RTC를 완료 하였다. First Release */
//#define VERSION "2.4.9"
/* 새로운 보드에서 RTC를 완료 하였다. */
//efine VERSION "2.4.9"  // RTC 시간을 설정 ..드디어 잡음.
//#define VERSION "2.4.8"  // RTC 시간을 설정 ..드디어 잡음.
//RTC설정에서 delay(500) 및 300을 줬을때 시간을 제대로 유지했다. 
//이것이 중요한 줄은 처음 알았아.
//#define VERSION "2.4.7"  // Modbus에서 RTC 시간을 설정하는 부분을 주석처리한다.
//#define VERSION "2.4.6"  // 시간설정에 오류가 있었다. 
// outmode를 설정해 놓았어야 했는데 , 터치에서 이것이 터치 초기화에서 input로 바뀐다. 
// main에서 이것을 찾아주고, 티치에서 인터럽트 부분을 삭제한다. 
//#define VERSION "2.4.5"  // Touch Calibration 초기화 기능 추가 
//#define VERSION "2.4.4"  // PREV 버튼 한글 변경 
//#define VERSION "2.4.3"  // PREV 버튼 한글 변경 
//#define VERSION "2.4.2"  // 년.월.일 정보 입력에 제한을 준다. 
//#define VERSION "2.4.1"  // 터치 캘리브레이션 기능 추가 .....
//#define VERSION "2.4.0"  // 터치 캘리브레이션 기능 추가 
//#define VERSION "2.3.7"  // 입력 과전류 -> 입력 과전류 이상, N상 게이트 -> N상 게이트 이상 
//#define VERSION "2.3.6"  // lcd 밝기 최소값을 80으로 변경 
//#define VERSION "2.3.5"  // bypass frequency -> output frequency 로 변경 
//#define VERSION "2.3.4"  // 입력값 즉시 반영하게 함  
//#define VERSION "2.3.3"  // ui_SettingScreen 에서 통신을 중단하고 화면 데이타를 유지하도록 한다. 
//#define VERSION "2.3.2"  // change language -> always restart
//#define VERSION "2.3.1"  // xpt2046파일을 변경했으므로 이것을 remote git에 추가한다. 
//#define VERSION "2.3.0"  // Display 패턴을 끊지 않고 RTC를 해결함.  
//#define VERSION "2.2.1"  // 통신 반응을 높이기 위해 modbusErrorCounter 를 수정한다.
//#define VERSION "2.2.0"  // Board V2.0 추가 ,Rtc.SetDateTime(compiled)
//#define VERSION "2.1.3"  // 로그문제를 추가 해결함 
//#define VERSION "2.1.2"  // 이제 중복로그를 해결함. 
//#define VERSION "2.1.1"  // 중복 로그를 지우기 전 동작 버전 Backup 
//#define VERSION "2.1.0"  // 큐를 사용하여 모드버스 통신을 하게 한다. 
//#define VERSION "2.0.1"  // 잡다한 출력 로그를 없앤다. 
//#define VERSION "2.0.0"  // BlueTooth에 update 명령을 추가한다. 
//#define VERSION "1.1.9"  // BlueTooth에 update 명령을 추가한다. 
//#define VERSION "1.1.4"  // 화면 흔들림을 clk를 조절해서 잡았다. 6000000
//#define VERSION "VER_1.1.4"  // file pointer를 사용하여 로그 파일을 쓰는 부분을 수정한다. 
//#define VERSION "VER_1.1.4"  // 쓰레드를 분리하여 CPU0에서 모드버스 통신을 하게 한다. 
//#define VERSION "VER_1.1.4"  // 시간 설정 부분을 수정한다. 
//#define VERSION "VER_1.1.4"  // 입력제한을 풀어준다 
//#define VERSION "VER_1.1.4"  // 고효율모드->배터리 1차저전압 으로 변경 
//#define VERSION "VER_1.1.4"  // WIFI ssid를 변경할수 있게 명령을 추가한다. 
//#define VERSION "VER_1.1.4"  // 로그 순서를 역순으로 변경한다. 
//#define VERSION "VER_1.1.3"  // HFMODE를 변경하여 입력할 수 있게 한다.
//efine VERSION "VER_1.1.2"  // RTC 시간 설정 부분을 수정함.
