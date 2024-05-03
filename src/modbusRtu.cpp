#include <Arduino.h>
#include <EEPROM.h>
#include "modbusRtu.h"
#include "mainGrobal.h"
#include <ModbusServerRTU.h>
#include <RtcDS1302.h>

#define COILSRUNTATUS_START  0
#define COILSUPSTATUS_START  16
#define COILSHWSATUUS_START  32 
#define COILSUPSOPERATIONFAULT_START  48 
ups_modbus_data_t upsModbusData;
ModbusServerRTU upsModbus232(2000);
uint8_t _CoilData[16*4]; // converter status 16개, inverter fault status 16개
void setRtcNewTime(RtcDateTime rtc);

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
  upsModbusData.upsOperationFault.Bit.Inverter_Frequency_fault = _CoilData[index++] ? 1 : 0;
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
  _CoilData[index++] = upsModbusData.upsOperationFault.Bit.Inverter_Frequency_fault;
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
void modbusSetup(){
  int address_485 = nvsSystemEEPRom.systemModusId;
  upsModbus232.registerWorker(address_485,READ_COIL,&FC01);
  upsModbus232.registerWorker(address_485,READ_HOLD_REGISTER,&FC03);
  upsModbus232.registerWorker(address_485,READ_INPUT_REGISTER,&FC04);
  upsModbus232.registerWorker(address_485,WRITE_COIL,&FC05);
  upsModbus232.registerWorker(address_485,WRITE_HOLD_REGISTER,&FC06);
  upsModbus232.begin(Serial1,nvsSystemEEPRom.BAUDRATE,address_485);
}

// void modbusTask(void *parameter){
//   modbusSetup();
//   for(;;){
//     vTaskDelay(100);
//   };
// }

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