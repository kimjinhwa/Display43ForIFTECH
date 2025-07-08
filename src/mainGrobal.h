#ifndef _MAIN_GROVAL_H
#define _MAIN_GROVAL_H
#include <Arduino.h>
//#define DONOTUSECOMM
#define LOG_PER_PAGE	5

typedef enum {
  NO_JOB = 0,
  SYSTEM_REBOOT = 1,
  FILE_FORMAT = 2,
  JOB_DONE = 99
}jobCommant_t;
typedef struct{
  uint16_t UpsON  : 1;
  uint16_t UpsOFF : 1;
  uint16_t ALARM_RESET: 1;
} upsRunCommandBit_t;
typedef union {
  upsRunCommandBit_t upsRunCommandBit;
  uint16_t upsRun;
}upsRun_t;

typedef struct
{
  uint16_t Charger_RUN : 1;                   // 0
  uint16_t Charger_ReRUN : 1;                 // 1
  uint16_t Charger_STOP : 1;                  // 2
  uint16_t Charger_FaultSTOP : 1;             // 3
  uint16_t Charger_DCDC_RUN : 1;              // 4
  uint16_t Charger_DCDC_ReRUN : 1;            // 5
  uint16_t Charger_DCDC_STOP : 1;             // 6
  uint16_t Charger_DCDC_FaultSTOP : 1;        // 7
  uint16_t Inverter_RUN : 1;                  // 8
  uint16_t Inverter_ReRUN : 1;                // 9
  uint16_t Inverter_STOP : 1;                 // 10
  uint16_t Inverter_FaultSTOP : 1;            // 11
  uint16_t To_Inverter_ModeChange : 1;        // 12
  uint16_t To_Bypass_ModeChange : 1;          // 13
  uint16_t To_Bypass_ModeChange_ForFault : 1; // 14
  uint16_t IInverter_over_load_over_time : 1; // 15
} ModuleStateBit_t;

typedef union{
 ModuleStateBit_t Bit;
 uint16_t status;          // 비트 필드 외의 일반 변수
} ModuleState_t;
//  uint16_t eventWriteMask;  //ModuleStateBit의 값이 설정되고 
//                            //Event Mask와 & 연산을 하여 True이면  
//                            //Event 를 기록한다.

typedef struct
{
  uint16_t Input_OC : 1;                       // 0 // 16.0	Input_OC(H/W Latch)	1: fault, 0: Normal
  uint16_t Inverter_OC : 1;                    // 1 // 16.1	Inverter OC(H/W Latch)	1: fault, 0: Normal
  uint16_t Vdc_OV : 1;                         // 2 // 16.2	Vdc_OV(H/W Latch)	1: fault, 0: Normal
  uint16_t CONVERTER_RUN_STOP_STATE : 1;       // 3 // 16.3	CONVERTER RUN/STOP STATE	1: CONV ON, 0: CONV OFF
  uint16_t DC_DC_CONVERTER_RUN_STOP_STATE : 1; // 4 // 16.4	DC/DC CONVERTER RUN/STOP STATE	1: DCDC ON, 0: DCDC OFF
  uint16_t Conv_GDU : 1;                       // 5 // 16.5	Conv_GDU	1: fault, 0: Normal
  uint16_t Inv_GDU : 1;                        // 6 // 16.6	Inv_GDU	1: fault, 0: Normal
  uint16_t GDU_DCDC : 1;                       // 7 // 16.7	GDU_DCDC	1: fault, 0: Normal
  uint16_t Com_GDU : 1;                        // 8 // 16.8	 Com_GDU	1: fault, 0: Normal
  uint16_t INVERTER_RUN_STOP_STATE : 1;        // 9 // 16.9	INVERTER RUN/STOP STATE	1: INV ON, 0: INV OFF
  uint16_t BAT_FUSE : 1;                       // 10 // 16.10	BAT_FUSE	1: fault, 0: Normal
  uint16_t Module_OT : 1;                      // 11 // 16.11	Module OT	1: fault, 0: Normal
  uint16_t Buzz_ON_OFF : 1;                    // 12 // 16.12	Buzz ON/OFF Control from COM.	toggle : 부져정지
  uint16_t EEPROM_ERR : 1;                     // 13 // 16.13	EEPROM ERR	1: EEPROM Fault
  uint16_t BAT_MCCB_Fault : 1;                 // 14 // 16.14	BAT MCCB Fault	1: fault, 0: Normal
  uint16_t TRANSFER_RUN_STOP_STATE : 1;        // 15 // 16.15	TRANSFER RUN/STOP STATE	1: INV MODE, 0: BYP MODE
} HWStateBit_t;
typedef union{
 HWStateBit_t Bit;
 uint16_t status;    // 비트 필드 외의 일반 변수
} HWState_t;

typedef struct
{
  uint16_t converter_current_limit_fault : 1; // 17.0	converter current limit fault(컨버터 전류제한이상)	1: fault, 0: Normal	충전기 정지
  uint16_t Vdc_link_OV_limit_fault : 1;       // 17.1	Vdc_link OV limit fault(직류링크 과전압 제한이상)	1: fault, 0: Normal	ALL 정지
  uint16_t UV_limit_fault : 1;                // 17.2	Vdc_link UV limit fault(직류링크 저전압제한 이상)	1: fault, 0: Normal	인버터 정지, 절환
  uint16_t input_under_voltage_fault : 1;     // 17.3	input under voltage fault(입력 저전압 이상)	1: fault, 0: Normal	충전기 정지
  uint16_t input_over_voltage_fault : 1;      // 17.4	input over voltage fault(입력 고 전압 이상)	1: fault, 0: Normal	충전기 정지
  uint16_t Input_Frequency_fault : 1;         // 17.5	Input Frequency fault(입력 주파수 이상)	1: fault, 0: Normal	충전기 정지
  uint16_t Inverter_Frequency_fault : 1;      // 17.6	Inverter Frequency fault(인버터 주파수 이상)	1: fault, 0: Normal
  uint16_t utility_line_failure : 1;          // 17.7	utility line failure(정전)	1: fault, 0: Normal	충전기 정지
  uint16_t battery_current_limit : 1;         // 17.8	battery current limit(배터리 전류제한 이상)	1: fault, 0: Normal	DC/DC 정지
  uint16_t Battery_OV_limit_fault : 1;        // 17.9	Battery OV limit fault(배터리 과전압제한 이상)	1: fault, 0: Normal	DC/DC 정지
  uint16_t battery_UV_limit_fault : 1;        // 17.10	battery UV limit fault(배터리 저전압제한 이상)	1: fault, 0: Normal	DC/DC 정지(boost)
  uint16_t inverter_output_voltage_fault : 1; // 17.11	inverter output voltage fault(인버터 출력전압이상)	1: fault, 0: Normal	인버터 정지, 절환
  uint16_t output_OC_overtime_fault : 1;      // 17.12	output OC overtime fault(출력 과부하)	1: fault, 0: Normal
  uint16_t inverter_OL_STOP : 1;              // 17.13	inverter OL STOP(인버터 과부하 정지)	1: fault, 0: Normal	인버터 정지, 절환
  uint16_t OFFSET_CHECK_ERR : 1;              // 17.14	OFFSET CHECK ERR	1: fault, 0: Normal
  uint16_t reserved : 1;                      // 17.15
} UpsOperationFaultBit_t;

typedef union{
 UpsOperationFaultBit_t Bit;
 uint16_t status;    // 비트 필드 외의 일반 변수
} UpsOperationFault_t;

typedef struct
{
  uint16_t Input_OC : 1;
  uint16_t Inverter_OC : 1;
  uint16_t Vdc_link_OV : 1;
  uint16_t CONVERTER_OFF : 1;
  uint16_t DC_DC_OFF : 1;
  uint16_t Converter_GDU : 1;
  uint16_t Inverter_GDU : 1;
  uint16_t DC_DC_GDU : 1;
  uint16_t DCommon_GDU : 1;
  uint16_t DEarth_fault : 1;
  uint16_t DFuse_Open : 1;
  uint16_t DModule_OT : 1;
  uint16_t Dreserved1 : 1;
  uint16_t DEEPROM_Error : 1;
  uint16_t Dreserved2 : 1;
  uint16_t Dreserved3 : 1;
} converterStatusBit_t;
typedef union{
 converterStatusBit_t converterStatus;
 uint16_t status;    // 비트 필드 외의 일반 변수
} converterStatus_t;


typedef struct {
  uint16_t modbusMode : 1;
  uint16_t useWiFi : 1;
  uint16_t useBlueTooth: 1;
  uint16_t useWebServer: 1;
}systemOperatingModeBit_t;
typedef union{
  systemOperatingModeBit_t sysOpModeBit;
  uint16_t operatingMode;
}operatingMode_t;

typedef struct
{
  uint16_t Nominal_Capacity;             // 0  10의 배수
  uint16_t Nominal_InputVoltage;         // 1 [###V]
  uint16_t Nominal_OutputVoltage;        // 2 [###V]
  uint16_t Nominal_BatVoltage;           // 3 [###V]
  uint16_t reserved_1;                   // 4
  uint16_t reserved_2;                   // 5
  upsRun_t upsRun;                       // 6
  uint16_t reserved_3;                   // 7
  uint16_t Bat_Current_Ref;              // 8
  uint16_t Bat_Voltage_Ref;              // 9
  uint16_t Output_Voltage_Ref;           // 10
  uint16_t HF_MODE;                      // 11 0:Normal 1: High Efficiency
  converterStatus_t reserved_4;          // 12  Not Use
  uint16_t reserved_5;                   // 13  Not Use
  uint16_t reserved_6;                   // 14  Not Use
  ModuleState_t ModuleState;             // 15
  HWState_t HWState;                     // 16  trnms->1
  UpsOperationFault_t upsOperationFault; // 17  trnms->2
  uint16_t reserved_7;                   // 18  trnms->3
  uint16_t reserved_8;                   // 19
  uint16_t Input_volt_rms;               // 20
  uint16_t Input_current_rms;            // 21
  uint16_t vdc_link_volt_rms;            // 22
  uint16_t bat_volt_rms;                 // 23
  int16_t bat_current_rms;              // 24
  uint16_t inverter_volt_rms;            // 25
  uint16_t inverter_current_rms;         // 26
  uint16_t output_volt_rms;              // 27
  uint16_t output_current_rms;           // 28
  uint16_t conv_Frequency;               // 29
  uint16_t inv_Frequency;                // 30
  uint16_t bypass_Frequency;             // 31
  uint16_t battery_capacity;             // 32
  uint16_t load_percentage;              // 33
  uint16_t inv_internal_Temperature;     // 34
  uint16_t reserved_9;                   // 35
  uint16_t reserved_10;                  // 36
  uint16_t reserved_11;                  // 37
  uint16_t input_volt_gain;              // 38
  uint16_t input_current_gain;           // 39
  uint16_t vdc_link_volt_gain;           // 40
  uint16_t vbat_volt_gain;               // 41
  uint16_t bat_current_gain;             // 42
  uint16_t inverter_volt_gain;           // 43
  uint16_t inverter_current_gain;        // 44
  uint16_t GND_1;                        // 45
  uint16_t output_current_gain;          // 46
  uint16_t reserved_12;                  // 47
  uint16_t reserved_13;                  // 48
  uint16_t reserved_14;                  // 49
  uint16_t input_volt_offset;            // 50
  uint16_t input_current_offset;         // 51
  uint16_t vdc_link_volt_offset;         // 52
  uint16_t bat_volt_offset;              // 53
  uint16_t bat_current_offset;           // 54
  uint16_t inverter_volt_offset;         // 55
  uint16_t inverter_current_offset;      // 56
  uint16_t GND_2;                        // 57
  uint16_t output_current_offset;        // 58
  uint16_t reserved_15;                  // 59 charging Method Bit(0)
  uint16_t systemYear;                   // 60
  uint16_t systemMonth;                  // 61
  uint16_t systemDay;                    // 62
  uint16_t systemHour;                   // 63
  uint16_t systemMinute;                 // 64
  uint16_t systemSecond;                 // 65
  uint16_t systemModusId;                // 66
  int16_t systemLanguage;               // 67
  uint16_t systemBaudRate;               // 68
  operatingMode_t systemOperatingMode;   // 69  0 : Agent, 1: Server
  uint16_t systemModbusQInterval;        // 70  0 : Agent, 1: Server
  uint16_t systemLedOffTime;             // 71
  uint16_t systemLedBright;              // 72 maximum : 100;
  uint16_t ipAddress1;                   // 73 maximum : 100;
  uint16_t ipAddress2;                   // 74 maximum : 100;
  uint16_t ipAddress3;                   // 75 maximum : 100;
  uint16_t ipAddress4;                   // 76 maximum : 100;
  uint16_t subnetMask1;                  // 77 maximum : 100;
  uint16_t subnetMask2;                  // 78 maximum : 100;
  uint16_t subnetMask3;                  // 79 maximum : 100;
  uint16_t subnetMask4;                  // 80 maximum : 100;
  uint16_t gateWay1;                     // 81 maximum : 100;
  uint16_t gateWay2;                     // 82 maximum : 100;
  uint16_t gateWay3;                     // 83 maximum : 100;
  uint16_t gateWay4;                     // 84 maximum : 100;
} ups_modbus_data_t;

extern ups_modbus_data_t upsModbusData;

typedef struct
{
    int16_t systemLanguage;
    uint16_t lcdBright;
    uint16_t systemLedOffTime;
    uint16_t systemModusId;
    uint32_t IPADDRESS;
    uint32_t GATEWAY;
    uint32_t SUBNETMASK;
    uint32_t WEBSOCKETSERVER;
    uint16_t WEBSERVERPORT;
    uint16_t BAUDRATE;
    uint16_t systemModbusQInterval;
    operatingMode_t operatingMode; // 0 : Agent, 1: Server
    uint16_t alarmSetStatus;
    bool isUpdate;
    char deviceName[20];
    char ssid[20];
    char password[20];
} nvsSystemSet_t;

extern nvsSystemSet_t nvsSystemEEPRom;
#endif