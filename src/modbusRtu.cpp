#include <Arduino.h>
#include <EEPROM.h>
#include "modbusRtu.h"
#include "mainGrobal.h"
#include "main.h"
#include "fileSystem.h"
#include "upsLog.h"
#include "lv_i18n.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#ifdef MODBUSSERVER
#include <ModbusServerRTU.h>
#else
#include "ModbusClientRTU.h"
#endif
#include <RtcDS1302.h>

#define COILSRUNTATUS_START  0
#define COILSUPSTATUS_START  16
#define COILSHWSATUUS_START  32 
#define COILSUPSOPERATIONFAULT_START  48 
ups_modbus_data_t upsModbusData = {
  .Nominal_Capacity=30,             // 0  10의 배수
  .Nominal_InputVoltage=220,         // 1 [###V]
  .Nominal_OutputVoltage=220,        // 2 [###V]
  .Nominal_BatVoltage=12,           // 3 [###V]
  .upsRun=0,                       // 6
  .Bat_Current_Ref=2,              // 8  [2-20]
  .Bat_Voltage_Ref=120,              // 9 [80-280]
  .Output_Voltage_Ref=220,           // 10 [110-240]
  .HF_MODE=0,                      // 11 0:Normal 1: High Efficiency
  .Input_volt_rms=0,               // 20
  .Input_current_rms=0,            // 21
  .vdc_link_volt_rms=0,            // 22
  .bat_volt_rms=0,                 // 23
  .bat_current_rms=0,              // 24
  .inverter_volt_rms=0,            // 25
  .inverter_current_rms=0,         // 26
  .output_volt_rms=0,              // 27
  .output_current_rms=0,           // 28
  .conv_Frequency=0,               // 29
  .inv_Frequency=0,                // 30
  .output_Frequency=0,             // 31
  .battery_capacity=0,             // 32
  .load_percentage=0,              // 33
  .inv_internal_Temperature=0,     // 34
  .reserved_9=0,                   // 35
  .reserved_10=0,                  // 36
  .reserved_11=0,                  // 37
  .input_volt_gain=250,              // 38
  .input_current_gain=250,           // 39
  .vdc_link_volt_gain=250,           // 40
  .vbat_volt_gain=250,               // 41
  .bat_current_gain=250,             // 42
  .inverter_volt_gain=250,           // 43
  .inverter_current_gain=250,        // 44
  .GND_1=0,                        // 45
  .output_current_gain=250,          // 46
  .input_volt_offset=0,            // 50
  .input_current_offset=0,         // 51
  .vdc_link_volt_offset=0,         // 52
  .bat_volt_offset=0,              // 53
  .bat_current_offset=0,           // 54
  .inverter_volt_offset=0,         // 55
  .inverter_current_offset=0,      // 56
  .output_current_offset=0,        // 58
  .systemModusId=1,                // 66
  .systemBaudRate=9600,               // 68
  .systemModbusQInterval=1000,        // 70  0 : Agent, 1: Server
  .systemLedOffTime=60,             // 71
  .systemLedBright=255,              // 72 maximum : 100,
  .ipAddress1=192,                   // 73 maximum : 100,
  .ipAddress2=168,                   // 74 maximum : 100,
  .ipAddress3=0,                   // 75 maximum : 100,
  .ipAddress4=54,                   // 76 maximum : 100,
  .subnetMask1=255,                  // 77 maximum : 100,
  .subnetMask2=255,                  // 78 maximum : 100,
  .subnetMask3=255,                  // 79 maximum : 100,
  .subnetMask4=0,                  // 80 maximum : 100,
  .gateWay1=192,                     // 81 maximum : 100,
  .gateWay2=168,                     // 82 maximum : 100,
  .gateWay3=0,                     // 83 maximum : 100,
  .gateWay4=1                     // 84 maximum : 100,

};
uint8_t _CoilData[16*4]; // converter status 16개, inverter fault status 16개
void setRtcNewTime(RtcDateTime rtc);
void scrMeasureLoad();
void scrSettingScreen();

void syncUpsCoilStatusToUpsdata()
{
  uint16_t index = COILSRUNTATUS_START;
  upsModbusData.upsRun.upsRunCommandBit.UpsON = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsRun.upsRunCommandBit.UpsOFF = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsRun.upsRunCommandBit.ALARM_RESET = _CoilData[index++] ? 1 : 0;

  index = COILSUPSTATUS_START;
  upsModbusData.ModuleState.Bit.Charger_RUN = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Charger_ReRUN = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Charger_STOP = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Charger_FaultSTOP = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Charger_DCDC_RUN = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Charger_DCDC_ReRUN = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Charger_DCDC_STOP = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Charger_DCDC_FaultSTOP = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Inverter_RUN = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Inverter_ReRUN = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Inverter_STOP = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.Inverter_FaultSTOP = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.To_Inverter_ModeChange = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.To_Bypass_ModeChange = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.To_Bypass_ModeChange_ForFault = _CoilData[index++] ? 1 : 0;
  upsModbusData.ModuleState.Bit.IInverter_over_load_over_time = _CoilData[index++] ? 1 : 0;

  index = COILSHWSATUUS_START;
  upsModbusData.HWState.Bit.Input_OC = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.Inverter_OC = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.Vdc_OV = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.CONVERTER_RUN_STOP_STATE = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.DC_DC_CONVERTER_RUN_STOP_STATE = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.Conv_GDU = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.Inv_GDU = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.GDU_DCDC = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.Com_GDU = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.INVERTER_RUN_STOP_STATE = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.BAT_FUSE = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.Module_OT = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.Buzz_ON_OFF = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.EEPROM_ERR = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.BAT_MCCB_Fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.HWState.Bit.TRANSFER_RUN_STOP_STATE = _CoilData[index++] ? 1 : 0;

  index = COILSUPSOPERATIONFAULT_START;
  upsModbusData.upsOperationFault.Bit.converter_current_limit_fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.Vdc_link_OV_limit_fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.UV_limit_fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.input_under_voltage_fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.input_over_voltage_fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.Input_Frequency_fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.Battery_UV_Primary_Fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.utility_line_failure = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.battery_current_limit = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.Battery_OV_limit_fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.battery_UV_limit_fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.inverter_output_voltage_fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.output_OC_overtime_fault = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.inverter_OL_STOP = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.OFFSET_CHECK_ERR = _CoilData[index++] ? 1 : 0;
  upsModbusData.upsOperationFault.Bit.reserved = _CoilData[index++] ? 1 : 0;
}
void UpsdataTosyncUpsCoilStatus()
{
  uint16_t index;
  index = COILSRUNTATUS_START;
  _CoilData[index++] = upsModbusData.upsRun.upsRunCommandBit.UpsON ? 1 : 0;
  _CoilData[index++] = upsModbusData.upsRun.upsRunCommandBit.UpsOFF ? 1 : 0;
  _CoilData[index++] = upsModbusData.upsRun.upsRunCommandBit.ALARM_RESET ? 1 : 0;

  index = COILSUPSTATUS_START;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Charger_RUN ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Charger_ReRUN ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Charger_STOP ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Charger_FaultSTOP ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Charger_DCDC_RUN ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Charger_DCDC_ReRUN ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Charger_DCDC_STOP ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Charger_DCDC_FaultSTOP ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Inverter_RUN ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Inverter_ReRUN ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Inverter_STOP ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.Inverter_FaultSTOP ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.To_Inverter_ModeChange ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.To_Bypass_ModeChange ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.To_Bypass_ModeChange_ForFault ? 1 : 0;
  _CoilData[index++] = upsModbusData.ModuleState.Bit.IInverter_over_load_over_time ? 1 : 0;

  index = COILSHWSATUUS_START;
  _CoilData[index++] = upsModbusData.HWState.Bit.Input_OC;
  _CoilData[index++] = upsModbusData.HWState.Bit.Inverter_OC;
  _CoilData[index++] = upsModbusData.HWState.Bit.Vdc_OV;
  _CoilData[index++] = upsModbusData.HWState.Bit.CONVERTER_RUN_STOP_STATE;
  _CoilData[index++] = upsModbusData.HWState.Bit.DC_DC_CONVERTER_RUN_STOP_STATE;
  _CoilData[index++] = upsModbusData.HWState.Bit.Conv_GDU;
  _CoilData[index++] = upsModbusData.HWState.Bit.Inv_GDU;
  _CoilData[index++] = upsModbusData.HWState.Bit.GDU_DCDC;
  _CoilData[index++] = upsModbusData.HWState.Bit.Com_GDU;
  _CoilData[index++] = upsModbusData.HWState.Bit.INVERTER_RUN_STOP_STATE;
  _CoilData[index++] = upsModbusData.HWState.Bit.BAT_FUSE;
  _CoilData[index++] = upsModbusData.HWState.Bit.Module_OT;
  _CoilData[index++] = upsModbusData.HWState.Bit.Buzz_ON_OFF;
  _CoilData[index++] = upsModbusData.HWState.Bit.EEPROM_ERR;
  _CoilData[index++] = upsModbusData.HWState.Bit.BAT_MCCB_Fault;
  _CoilData[index++] = upsModbusData.HWState.Bit.TRANSFER_RUN_STOP_STATE;

  index = COILSUPSOPERATIONFAULT_START;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.converter_current_limit_fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.Vdc_link_OV_limit_fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.UV_limit_fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.input_under_voltage_fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.input_over_voltage_fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.Input_Frequency_fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.Battery_UV_Primary_Fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.utility_line_failure;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.battery_current_limit;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.Battery_OV_limit_fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.battery_UV_limit_fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.inverter_output_voltage_fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.output_OC_overtime_fault;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.inverter_OL_STOP;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.OFFSET_CHECK_ERR;
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.reserved;
}
#ifdef MODBUSSERVER
ModbusServerRTU upsModbus232(2000);
void modbusSetup(){
  int address_485 = nvsSystemEEPRom.systemModusId;
  upsModbus232.registerWorker(address_485,READ_COIL,&FC01);
  upsModbus232.registerWorker(address_485,READ_HOLD_REGISTER,&FC03);
  upsModbus232.registerWorker(address_485,READ_INPUT_REGISTER,&FC04);
  upsModbus232.registerWorker(address_485,WRITE_COIL,&FC05);
  upsModbus232.registerWorker(address_485,WRITE_HOLD_REGISTER,&FC06);
  upsModbus232.begin(Serial2,nvsSystemEEPRom.BAUDRATE,address_485);
}


void setSendbuffer(uint16_t *sendValue){
  EEPROM.readBytes(1, (byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
  IPAddress ipaddr ;
  struct timeval tmv;
  gettimeofday(&tmv, NULL);
  RtcDateTime now;
  now = RtcDateTime(tmv.tv_sec);
  uint16_t *modbusData = (uint16_t *)&upsModbusData;
  if(sizeof(ups_modbus_data_t)>=255) return;
  //모든 데이타를 sendValue에 넣어준다.
  upsModbusData.systemYear = now.Year();
  upsModbusData.systemMonth=now.Month();
  upsModbusData.systemDay=now.Day();
  upsModbusData.systemHour=now.Hour();
  upsModbusData.systemMinute=now.Minute();
  upsModbusData.systemSecond=now.Second();
  upsModbusData.systemModusId = nvsSystemEEPRom.systemModusId;
  upsModbusData.systemLanguage= nvsSystemEEPRom.systemLanguage;
  upsModbusData.systemBaudRate= nvsSystemEEPRom.BAUDRATE;
  upsModbusData.systemOperatingMode= nvsSystemEEPRom.operatingMode;
  upsModbusData.systemModbusQInterval= nvsSystemEEPRom.systemModbusQInterval;
  upsModbusData.systemLedOffTime= nvsSystemEEPRom.systemLedOffTime;
  upsModbusData.systemLedBright= nvsSystemEEPRom.lcdBright;
  ipaddr = nvsSystemEEPRom.IPADDRESS ; 
  upsModbusData.ipAddress1= (uint8_t)ipaddr[0];
  upsModbusData.ipAddress2= (uint8_t)ipaddr[1];
  upsModbusData.ipAddress3= (uint8_t)ipaddr[2];
  upsModbusData.ipAddress4= (uint8_t)ipaddr[3];
  ipaddr = nvsSystemEEPRom.SUBNETMASK; 
  upsModbusData.subnetMask1= (uint8_t)ipaddr[0];
  upsModbusData.subnetMask2= (uint8_t)ipaddr[1];
  upsModbusData.subnetMask3= (uint8_t)ipaddr[2];
  upsModbusData.subnetMask4= (uint8_t)ipaddr[3];
  ipaddr = nvsSystemEEPRom.GATEWAY; 
  upsModbusData.gateWay1= (uint8_t)ipaddr[0];
  upsModbusData.gateWay2= (uint8_t)ipaddr[1];
  upsModbusData.gateWay3= (uint8_t)ipaddr[2];
  upsModbusData.gateWay4= (uint8_t)ipaddr[3];

  for(int i=0;i<sizeof(ups_modbus_data_t);i++){
    sendValue[i] = modbusData[i]; 
  }
}
uint16_t _div8RndUp(uint16_t value) {
  return (value + 7) >> 3;
}
ModbusMessage FC01(ModbusMessage request) {
  uint16_t address;       // requested register address
  uint16_t writeAddress;  // requested register address
  uint16_t words;         // requested number of registers
  ModbusMessage response; // response message to be sent back
  uint16_t value;
  uint16_t sendValue[255];
  memset(sendValue, 0x00, sizeof(sendValue));
  UpsdataTosyncUpsCoilStatus();
  // get request values
  request.get(2, address);
  request.get(4, words);
  writeAddress = address & 0xFF;
  // words 요청하는 길이 이므로 는 1보다 커야 한다.
  // 2Byte이상의 길이는 삭제하고 요청하는 길이를 더해서 255가 넘으면 오류이다.
  //words == 0 ||
  if ( ((address & 0x00FF) + words) > 65)
  {
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
    return response;
  }
  // 응답데이타에 요청자 아이디를 더해 주고, function코드, 그리고 전송될 데이타의 길이를 적어준다.
  uint8_t responseCount =  (uint8_t)_div8RndUp(words);
  response.add(request.getServerID(), request.getFunctionCode(),responseCount );
  uint8_t _buf[4];
  for(uint16_t i=0;i<words;i++){  // _coil에 있는 값을 _buf에 넣어준다.
    bitWrite(_buf[ i>> 3],i & 7 ,_CoilData[ address + i]);
  };
  for(uint16_t i=0;i<responseCount ;i++)
    response.add((uint8_t )_buf[i]);
  return response;
}
ModbusMessage FC03(ModbusMessage request) {
  uint16_t address;       // requested register address
  uint16_t writeAddress;  // requested register address
  uint16_t words;         // requested number of registers
  ModbusMessage response; // response message to be sent back
  uint16_t value;
  uint16_t sendValue[255];
  memset(sendValue, 0x00, sizeof(sendValue));
  setSendbuffer(sendValue);
  // get request values
  request.get(2, address);
  request.get(4, words);
  writeAddress = address & 0xFF;
  // words 요청하는 길이 이므로 는 1보다 커야 한다.
  // 2Byte이상의 길이는 삭제하고 요청하는 길이를 더해서 255가 넘으면 오류이다.
  //words == 0 || 
  if (((address & 0x00FF) + words) > 255)
  {
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
    return response;
  }
  // 응답데이타에 요청자 아이디를 더해 주고, function코드, 그리고 전송될 데이타의 길이를 적어준다.
  response.add(request.getServerID(), request.getFunctionCode(), (uint8_t)(words * 2));

  // Serial.printf("\nFunction code 04 %d[%d] %d ",address,writeAddress,words);
  for (int i = address; i < words + address; i++)
  {
    value = sendValue[i];
    response.add(value);
    //Serial.printf(" %d",value);
  }
  return response;
};
ModbusMessage FC04(ModbusMessage request)
{
  uint16_t address;       // requested register address
  uint16_t writeAddress;  // requested register address
  uint16_t words;         // requested number of registers
  ModbusMessage response; // response message to be sent back
  uint16_t value;
  uint16_t sendValue[255];
  memset(sendValue, 0x00, sizeof(sendValue));
  setSendbuffer(sendValue);
  // get request values
  request.get(2, address);
  request.get(4, words);
  writeAddress = address & 0xFF;
  // words 요청하는 길이 이므로 는 1보다 커야 한다.
  // 2Byte이상의 길이는 삭제하고 요청하는 길이를 더해서 255 넘으면 오류이다.
  //words == 0 || 
  if (((address & 0x00FF) + words) > 255)
  {
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
    return response;
  }
  // 응답데이타에 요청자 아이디를 더해 주고, function코드, 그리고 전송될 데이타의 길이를 적어준다.
  response.add(request.getServerID(), request.getFunctionCode(), (uint8_t)(words * 2));

  // Serial.printf("\nFunction code 04 %d[%d] %d ",address,writeAddress,words);
  for (int i = address; i < words + address; i++)
  {
    value = sendValue[i];
    response.add(value);
    //Serial.printf(" %d",value);
  }
  return response;
};
ModbusMessage FC05(ModbusMessage request) {
  uint16_t address;       // requested register address
  ModbusMessage response; // response message to be sent back
  uint16_t value;
  uint16_t sendValue[255];
  memset(sendValue, 0x00, sizeof(sendValue));
  setSendbuffer(sendValue);
  // get request values
  request.get(2, address);
  request.get(4, value);
  uint16_t writeAddress = (0x00FF & address);
  if (writeAddress <1 || writeAddress > 65)
  {
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
    return response;
  }
  else if (value != 0 && value != 0xFF00){
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_VALUE     );
    return response;
  }
  _CoilData[address] = value>>8;
  response.add(request.getServerID(), request.getFunctionCode(),(uint16_t)address,(uint16_t)value); // 2 : 데이타 길이
  syncUpsCoilStatusToUpsdata();
  return response;
}
ModbusMessage FC06(ModbusMessage request)
{
  uint16_t address;       // requested register address
  ModbusMessage response; // response message to be sent back
  uint16_t value;
  uint16_t sendValue[255];
  memset(sendValue, 0x00, sizeof(sendValue));
  setSendbuffer(sendValue);
  // get request values
  request.get(2, address);
  request.get(4, value);
  uint16_t writeAddress = (0x00FF & address);

  struct timeval tmv;
  gettimeofday(&tmv, NULL);
  RtcDateTime now;
  now = RtcDateTime(tmv.tv_sec);

  EEPROM.readBytes(1, (byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));

  if (writeAddress > 255)
  {
    response.setError(request.getServerID(), request.getFunctionCode(), ILLEGAL_DATA_ADDRESS);
    return response;
  }
  response.add(request.getServerID(), request.getFunctionCode(),address, value); // 2 : 데이타 길이
  //response.add(value);

  // ESP_LOGI("MODBUS", "\nFunction code 06 address(%d) writeAddress(%d) value(%d) ", address, writeAddress, value);
  // ESP_LOGI("MODBUS", "Write and read %d ", systemDefaultValue.voltageCompensation[writeAddress]);
  if (writeAddress < 60)  // UPS Data 
  {
    uint16_t *modbusData = (uint16_t *)&upsModbusData;
    modbusData[writeAddress] = value;
  }
  if (writeAddress >= 60 && writeAddress < 66) // system data 
  {
    // 시간을 설정한다.
    switch (writeAddress)
    {
    case 60:// Year
      now = RtcDateTime(value, now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
      break;
    case 61://Month
      now = RtcDateTime(now.Year(), value, now.Day(), now.Hour(), now.Minute(), now.Second());
      break;
    case 62: //day
      now = RtcDateTime(now.Year(), now.Month(), value, now.Hour(), now.Minute(), now.Second());
      break;
    case 63://Hour
      now = RtcDateTime(now.Year(), now.Month(), now.Day(), value, now.Minute(), now.Second());
      break;
    case 64://Minute
      now = RtcDateTime(now.Year(), now.Month(), now.Day(), now.Hour(), value, now.Second());
      break;
    case 65: //Second
      now = RtcDateTime(now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), value);
      break;

    default:
      break;
    }
    tmv.tv_sec = now.TotalSeconds();
    tmv.tv_usec = 0;
    settimeofday(&tmv, NULL);
    setRtcNewTime(now);
  }
  if (writeAddress >= 66 && writeAddress < 84)  //Reserved
  {
    IPAddress ipaddr ;
    ipaddr = nvsSystemEEPRom.IPADDRESS ; 
    switch (value)
    {
    case 66:
      nvsSystemEEPRom.systemModusId = value;
      break;
    case 67:
      nvsSystemEEPRom.systemLanguage= value;
      break;
    case 68:
      nvsSystemEEPRom.BAUDRATE= value;
      break;
    case 69:
      nvsSystemEEPRom.operatingMode.operatingMode= value;
      break;
    case 70:
      nvsSystemEEPRom.systemModbusQInterval= value;
      break;
    case 71:
      nvsSystemEEPRom.systemLedOffTime= value;
      break;
    case 72:
      nvsSystemEEPRom.systemLedOffTime= value;
      break;
    case 73: //ipAddress1
      ipaddr[0] = value;
      nvsSystemEEPRom.IPADDRESS = ipaddr;
      break;
    case 74: //ipAddress2
      ipaddr[1] = value;
      nvsSystemEEPRom.IPADDRESS = ipaddr;
      break;
    case 75: //ipAddress3
      ipaddr[2] = value;
      nvsSystemEEPRom.IPADDRESS = ipaddr;
      break;
    case 76: //ipAddress4
      ipaddr[3] = value;
      nvsSystemEEPRom.IPADDRESS = ipaddr;
      break;
    case 77: //gateWay1
      ipaddr[0] = value;
      nvsSystemEEPRom.SUBNETMASK = ipaddr;
      break;
    case 78: //gateWay2
      ipaddr[1] = value;
      nvsSystemEEPRom.SUBNETMASK = ipaddr;
      break;
    case 79: //gateWay3
      ipaddr[2] = value;
      nvsSystemEEPRom.SUBNETMASK = ipaddr;
      break;
    case 80: //gateWay4
      ipaddr[3] = value;
      nvsSystemEEPRom.SUBNETMASK = ipaddr;
      break;
    case 81: //gateWay1
      ipaddr[0] = value;
      nvsSystemEEPRom.GATEWAY = ipaddr;
      break;
    case 82: //gateWay2
      ipaddr[1] = value;
      nvsSystemEEPRom.GATEWAY = ipaddr;
      break;
    case 83: //gateWay3
      ipaddr[2] = value;
      nvsSystemEEPRom.GATEWAY = ipaddr;
      break;
    case 84: //gateWay4
      ipaddr[3] = value;
      nvsSystemEEPRom.GATEWAY = ipaddr;
      break;

    default:
      break;
    }
    EEPROM.writeBytes(1, (const byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
    EEPROM.commit();
    EEPROM.readBytes(1, (byte *)&nvsSystemEEPRom, sizeof(nvsSystemSet_t));
  }
  if (writeAddress >= 120 && writeAddress < 126)
  { 
  }
  if (writeAddress >= 126 && writeAddress < 132)
  {

    // switch (writeAddress)
    // {
    // case 126:
    //   systemDefaultValue.modbusId = value;
    //   break;
    // case 127:
    //   systemDefaultValue.installed_cells= value;
    //   break;
    // case 128:
    //   systemDefaultValue.AlarmTemperature = value;
    //   break;
    // case 129:
    //   systemDefaultValue.alarmHighCellVoltage = value;
    //   break;
    // case 130:
    //   systemDefaultValue.alarmLowCellVoltage = value;
    //   break;
    // case 131:
    //   systemDefaultValue.AlarmAmpere = value;
    //   break;
    // default:
    //   break;
    // };
    // ESP_LOGI("MODUBS", "Write EEPROM");
    // EEPROM.writeBytes(1, (const byte *)&systemDefaultValue, sizeof(nvsSystemSet_t));
    // EEPROM.commit();
    // EEPROM.readBytes(1, (byte *)&systemDefaultValue, sizeof(nvsSystemSet_t));
  }
  return response;
};
#else
ModbusClientRTU MB(-1,100);
static uint32_t request_response;
static bool data_ready = false;
char requestToken[12]={'E','E','E','A','E','E','E','A','E','E','E','A'};
int tokenLoopCount=0;
int modbusErrorCounter=0;
void handleData(ModbusMessage response, uint32_t token)
{
  // First value is on pos 3, after server ID, function code and length byte
  uint16_t offs = 3;
  // The device has values all as IEEE754 float32 in two consecutive registers
  // Read the requested in a loop
  uint16_t *values;
  values = (uint16_t *)&upsModbusData;
  uint8_t func = response.getFunctionCode();
  if (func == READ_INPUT_REGISTER)
  {
    offs = 3;
    //ESP_LOGE("MODBUS","token %c", token);
    if (token == 'E')
    { // 이벤트를 요청한것이며 15,16,17번지이다
      for (uint8_t i = 0; i < 3; ++i)
      {
        offs = response.get(offs, values[15 + i]);
      }
      // for (uint8_t i = 0; i < 3; ++i)
      //   ESP_LOGE("MODBUS","%2d", values[15+i]);
    }
    if (token == 'A')
    { 
      for (uint8_t i = 0; i < 59; ++i)
      {
        offs = response.get(offs, values[i]);
      }
      scrMeasureLoad();
      scrSettingScreen();
      // ESP_LOGE("MODBUS","Rev data");
      // for (uint8_t i = 0; i < 59; ++i)
      //   printf("[%d]=%2d ",i, values[i]);
    }
    // Signal "data is complete"
    request_response = token;
    data_ready = true;
  }
  else if (func == WRITE_MULT_REGISTERS)
  {
    int address ; 
    const uint8_t *rev =  response.data();
    int len = response.size();
    address = rev[2]<<8 | rev[3];
    //for(int i=0;i<len;i++) ESP_LOGE("MODBUS"," %02x",rev[i]);
    offs = 4;
    offs = response.get(offs, len);
    ESP_LOGE("MODBUS","multi func %d token %d address %d count %d",func, token,address, values[token]);
    data_ready = true;
    request_response = token;
  }
  else if (func == WRITE_HOLD_REGISTER )
  {
    int address ; 
    const uint8_t *rev =  response.data();
    int len = response.size();
    address = rev[2]<<8 | rev[3];
    for(int i=0;i<len;i++)
      ESP_LOGE("MODBUS"," %02x",rev[i]);
    offs = 4;
    offs = response.get(offs, values[token]);
    ESP_LOGE("MODBUS","func %d token %d address %d value %d",func, token,address, values[token]);
    data_ready = true;
    request_response = token;
  }
}
uint32_t getDataReady ()
{
  return data_ready;
}
uint32_t getReceiveToken()
{
  return request_response ;
}
void handleError(Error error, uint32_t token) 
{
  // ModbusError wraps the error code and provides a readable error message for it
  modbusErrorCounter++;
  if(modbusErrorCounter>10){
  }
  ModbusError me(error);
  ESP_LOGE("MODBUS","Error response: %02X - %s\n", (int)me, (const char *)me);
}
uint32_t waitDataReceive(int wCount)
{

	uint16_t waitingCount = 0;
	while (!data_ready)  // 1초간 Waiting한다.
	{
		vTaskDelay(10);
    waitingCount++;
		//ESP_LOGI("MODUBS", "Wating...%d", waitingCount++);
    if(waitingCount>wCount) break;
	}
	ESP_LOGI("MODUBS", "WatingCount %d Received %d..",waitingCount, request_response );
  tokenLoopCount = 3;
  return request_response ;
}
/* 데이타를 보낼때 Time out을 결정한다. 
*  @timeout milisecond
*/

int WriteHoldRegistorNoSync(int index,int value,uint32_t Token){
  tokenLoopCount = -1;  // 더이상 Looping을 하지 않게 한다
  ESP_LOGE("MODBUS","ready WriteHoldRegistor %ld ",millis());
  ESP_LOGE("MODBUS","send WriteHoldRegistor index %ld ,value %d ", index,value);
  data_ready = false;

  //Error err = MB.addRequest(Token, 1, WRITE_HOLD_REGISTER, index, value);
  // if (err!=SUCCESS) {
  //   ModbusError e(err);
  //   ESP_LOGE("MODBUS","Error creating request: %02X - %s\n", (int)e, (const char *)e);
  // }
  //ModbusMessage rc = MB.syncRequest(Token, 1, WRITE_HOLD_REGISTER, index, value);
  uint16_t address=1;
  uint8_t byteCount=2;
  uint16_t arrayWord[1];
  arrayWord[0]= value;
  uint16_t *values;
  values = (uint16_t *)&upsModbusData;
  values[index] = value;
  Error err = MB.addRequest(Token,(uint8_t) 1, (uint8_t)WRITE_MULT_REGISTERS,(uint16_t) index,(uint16_t)address,(uint8_t) byteCount,(uint16_t *) &arrayWord);
  if (err!=SUCCESS) {
    ModbusError e(err);
    ESP_LOGE("MODBUS","Error creating request: %02X - %s\n", (int)e, (const char *)e);
  }
  tokenLoopCount = 0;  // 다시 데이타를 받기 시작한다 
  return Token;
}
void enqueueModbusCommand(int index, int value, uint32_t token) {
    ModbusCommand cmd = { index, value, token };
    //ESP_LOGW("MODBUS","enqueueModbusCommand index %d ,value %d ,token %d", index,value,token);
    xQueueSend(modbusCmdQueue, &cmd, portMAX_DELAY);
}

int WriteHoldRegistor(int index,int value,uint32_t Token){
  // 태스크 핸들 유효성 검사
  tokenLoopCount = -1;  // 더이상 Looping을 하지 않게 한다
  // ESP_LOGW("MODBUS","ready WriteHoldRegistor %ld ",millis());
  // ESP_LOGW("MODBUS","send WriteHoldRegistor index %ld ,value %d ", index,value);
  data_ready = false;

  uint16_t address=1;
  uint8_t byteCount=2;
  uint16_t arrayWord[1];
  arrayWord[0]= value;
  //                               (uint8_t serverID, uint8_t functionCode, uint16_t p1, uint16_t p2, uint8_t count, uint16_t *arrayOfWords);
  uint16_t *values;
  values = (uint16_t *)&upsModbusData;
  values[index] = value;
  ModbusMessage rc = MB.syncRequest(Token,(uint8_t) 1, (uint8_t)WRITE_MULT_REGISTERS,(uint16_t) index,(uint16_t)address,(uint8_t) byteCount,(uint16_t *) &arrayWord);
  if(rc.getError() == 0){
    handleData(rc,Token);
  }
  else{
    ESP_LOGW("MODUBS","MODBUS ERROR %d",rc.getError());
    Token=0;
  }
  tokenLoopCount = 0;  // 다시 데이타를 받기 시작한다 
  return Token;
}
// int WriteHoldRegistor(int index,int value,uint32_t Token){
//   uint16_t sendCount=1;
//   tokenLoopCount = -1;
//   vTaskDelay(300);
//   data_ready = false;
//   Error err = MB.addRequest(Token, 1, WRITE_MULT_REGISTERS, sendCount, index, value);
//   if (err!=SUCCESS) {
//     ModbusError e(err);
//     ESP_LOGE("MODBUS","Error creating request: %02X - %s\n", (int)e, (const char *)e);
//   }
//   //waitDataReceive();
//   return 0;
// }

void showMessageLabel(const char *message);
int modbusEventSendLoop(int token)
{
  if (tokenLoopCount == -1)
    return -1; // -1은 데이타 요청을 중지한다.
               // 나중에 데이타를 요청할 경우를 대비 하기 위하여
  //tokenLoopCount = token;
  data_ready = false;
  
  if (tokenLoopCount >= 12)
    tokenLoopCount = 0;
  FunctionCode func;
  int16_t startAddress;
  int16_t dataCount;
  
  ModbusCommand modbusCommand;
  if(xQueueReceive(modbusCmdQueue,&modbusCommand,0) == pdPASS){
    ESP_LOGW("MODBUS","modbusCommand received");

    //UPS가동시간은 길게 걸린다.
    MB.setTimeout(10000);
    int token = WriteHoldRegistor(modbusCommand.index,modbusCommand.value,modbusCommand.token);
    ESP_LOGW("MODBUS","modbusCommand received token %d",token);
		if (token == 0)
			showMessageLabel(_("Comm_Error"));
    tokenLoopCount=0;
    MB.setTimeout(1000);
		// else
		// 	ESP_LOGI("MODUBS", "Received token %d..", token);
  }

  switch (requestToken[tokenLoopCount])
  {
  case 'E':
    /* code */
    func = READ_INPUT_REGISTER;
    startAddress = 15;
    dataCount =3;
    break;
  case 'A':
    func = READ_INPUT_REGISTER;
    startAddress = 0;
    dataCount =59;
    break;
  
  default:
    break;
  }

  ModbusMessage rc = MB.syncRequest (requestToken[tokenLoopCount], 1, func, startAddress, dataCount );
  if(rc.getError() == 0){
    handleData(rc,requestToken[tokenLoopCount]);
    modbusErrorCounter=0;
    tokenLoopCount++;  // 전송이 제대로 됬다면 다음 루틴으로 간다
    return requestToken[tokenLoopCount -1];
    //return requestToken[tokenLoopCount];
  }
  else{
    modbusErrorCounter++;
    ESP_LOGW("MODBUS", "Comm Error count %d",modbusErrorCounter);
    return 0;
  }

  //return data_ready;
}
void modbusStop(){
  tokenLoopCount=-1;
}
int modbusEventGetLoop()
{
  if (data_ready)
  {
    data_ready = false;
    //ESP_LOGE("MODBUS", "Data Read OK");
    return 1;
  }
  return 0;
}

void modbusSetup()
{
  MB.onDataHandler(&handleData);
  MB.onErrorHandler(&handleError);
  MB.setTimeout(1000);
  MB.begin(Serial2,nvsSystemEEPRom.BAUDRATE,1);
}

static int every1000ms = 1000;
static unsigned long previous1000mills = 0;
static unsigned long now;


unsigned long elaspTime=0;
uint16_t targetTime=0;
#include <Arduino_GFX_Library.h>
extern Arduino_RPi_DPI_RGBPanel *gfx;
extern LittleFileSystem lsFile;
#endif
    //MB.clearQueue();
    //MB.end();
    //vTaskDelay(100);