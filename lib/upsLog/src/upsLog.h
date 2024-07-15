#ifndef _UPS_LOG_H
#define _UPS_LOG_H
#define ESP32DEF 
#ifndef WINDOWS
#include <Arduino.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <tuple>
#include <cstring>
#include "lv_i18n.h"
#ifndef WINDOWS
#include "../../../src/mainGrobal.h"
#else
#include "main.h"
#endif
//#define VSCODE
#define MAX_LOG_COUNT   1000


#define MAX_LOGMESSAGE_LENGTH 60 
typedef struct upslog_t{
    uint16_t logId;
    uint32_t logTime;
    uint16_t modulestatus ;
    uint16_t HWstatus ;
    uint16_t operationFault;
    uint8_t  message[MAX_LOGMESSAGE_LENGTH ];
    uint8_t  checkBit; 
} upslog_t;

enum directionType_t 
{
    PREVLOG= -1,
    CURRENTLOG= 0,
    NEXTLOG= 1,
};
    enum eventType_t 
    {
        EVENT_TYPE = 0,
        FAULT_TYPE = 1,
    };

class upsLog
{
private:
    char filename[30];
    eventType_t eventType;
    uint16_t old_moduleStatusEvent=0;
    uint16_t old_HwStatusEvent=0;
    uint16_t old_upsOperationFault=0;
    HWState_t oldHWState;                     // 16  trnms->1
    uint16_t logid=0;
    uint16_t mask_moduleStatusEvent =0b0111111111111111;
    uint16_t mask_HwStatusEvent     =0b0110110111100111;
    uint16_t mask_upsOperationFault =0b0111111111111111;

    int shrinkFile();

public:
    uint16_t logCount=0;
    uint16_t totalPage;
    int16_t currentMemoryPage=0;
    long getFileSize();
    enum upsStatus_t
    {
        MODULE_STATUS = 0,
        HW_STATUS = 1,
        OPERATIONAL_FAULT = 2,
    };
    std::vector <std::tuple<uint16_t , std::string> >vlogs;
    std::vector <std::tuple<uint16_t , std::string> >vWarninglogs;
    //int16_t  vlogMemPos=0 ;
    uint16_t runBuzzStatus=0;
    uint16_t eventHistory=0;
    upsLog();
    upsLog(eventType_t eventType);
    void init();
    upsLog(const char* filename,eventType_t eventType);
    int writeLog(upslog_t *log);
    int writeLogToVmem(upslog_t *log);
    int setEventCode(uint16_t moduleStatusEvent,uint16_t HwStatusEvent,uint16_t upsOperationFault);
    void parseMessage(std::string *string_t,uint16_t logId,uint16_t value, upsStatus_t ups_status,uint32_t logtime);
    const char* getLogString(const upslog_t *logArray);
    const char * readCurrentLog(directionType_t direction);
    const char * readCurrentLogFromVector(directionType_t direction);
 

   const char *Module_state_event[16] = {
        "R_C_Start_Run_Event",               /* 0 */
        "R_C_Re_Start_Event",                /* 1 */
        "R_C_Stop_by_Command_Event",         /* 2 */
        "R_C_Stop_by_Fault_Event",           /* 3 */
        "DC_DC_Start_Run_Event",             /* 4 */
        "DC_DC_Re_Start_Event",              /* 5 */
        "DC_DC_Stop_by_Command_Event",       /* 6 */
        "DC_DC_Stop_by_Fault_Event",         /* 7 */
        "Inverter_Start_Run_Event",          /* 8 */
        "Inverter_Re_Start_Run_Event",       /* 9 */
        "Inverter_Stop_by_Command_Event",    /* 10 */
        "Inverter_Stop_by_Fault_Event",      /* 11 */
        "Transfer_to_Inverter_Event",        /* 12 */
        "Transfer_to_Bypass_by_Com_Event",   /* 13 */
        "Transfer_to_Bypass_by_fault_Event", /* 14 */
        ""                                   /* 15 */
    };
    const char *ups_hw_state_alarm[16] = {
        "Input_OC",                    /* 0 */
        "Inverter_Over_Current_fault", /* 1 */
        "DC_Link_OV_fault",            /* 2 */
        "",                            /* 3 CONVERTER RUN/STOP STATE*/
        "",                            /* 4 DC/DC CONVERTER RUN/STOP STATE*/
        "Converter_GDU_fault",         /* 5 */
        "Inverter_GDU_fault",          /* 6 */
        "DC_DC_GDU_fault",             /* 7 */
        "Common_GDU_fault",            /* 8 */
        "",                            /* 9 INVERTER RUN/STOP STATE*/
        "Battery_Fuse_fault",          /* 10 */
        "Module_OT",                   /* 11 */
        "",                            /* 12 Buzz ON/OFF Control from COM.*/
        "EEPROM_ERR",                  /* 13*/
        "Battery_MCCB_Fault",          /* 14 */
        "",                            /* 15 TRANSFER RUN/STOP STATE*/
    };
    const char *ups_operation_fault_alarm[16] = {
        "converter_current_limit_fault", /* 0 */
        "Vdc_link_OV_limit_fault",       /* 1 */
        "Vdc_link_UV_limit_fault",       /* 2 */
        "input_under_voltage_fault",     /* 3 */
        "input_over_voltage_fault",      /* 4 */
        "Input_Frequency_fault",         /* 5 */
        "Inverter_Frequency_fault",      /* 6 */
        "utility_line_failure",          /* 7 */
        "battery_current_limit_fault",   /* 8 */
        "Battery_OV_limit_fault",        /* 9 */
        "battery_UV_limit_fault",        /* 10 */
        "inverter_output_voltage_fault", /* 11 */
        "Output_Overload",               /* 12 */
        "Inverter_Overload_Stop",        /* 13*/
        "OFFSET_Check_ERR",              /* 14 */
        "",                              /* 15 reserved*/
    };
};
#endif

// class upsLog
// {
// private:
//     char filename[30];
//     uint16_t eventType;
//     uint16_t numPerPage=MAX_NUM_PAGE;
//     upslog_t logArray[MAX_NUM_PAGE]     ;
//     uint16_t logCount=0;
//     uint16_t totalPage;
//     int16_t currentPage;
//     int16_t actualReadCount;
//     uint16_t old_moduleStatusEvent=0;
//     uint16_t old_HwStatusEvent=0;
//     uint16_t old_upsOperationFault=0;

//     long logfile_size ;
//     uint16_t mask_moduleStatusEvent =0b0111111111111111;
//     uint16_t mask_HwStatusEvent     =0b0110110111100111;
//     uint16_t mask_upsOperationFault =0b0111111111111111;;

//     long getFileSize();
//     int readfilePage(int page);
//     int writeLog(upslog_t *log);

// public:
 
//     // enum eventType_t 
//     // {
//     //     EVENT = 0,
//     //     FAULT = 1,
//     // };

//     enum upsStatus_t
//     {
//         MODULE_STATUS = 0,
//         HW_STATUS = 1,
//         OPERATIONAL_FAULT = 2,
//         // UPS_STATUS_ENG = 0,
//         // UPS_FALULT_ENG = 1,
//         // UPS_STATUS_HAN = 2,
//         // UPS_FALULT_HAN = 3
//     };
//     int16_t currVlogPos=0;
//     std::vector <upslog_t>vlogs;
//     upsLog();
//     upsLog(const char* filename,uint16_t eventType);
//     int setEventCode(uint16_t moduleStatusEvent,uint16_t HwStatusEvent,uint16_t upsOperationFault);
//     void parseMessage( std::string *string_t,uint16_t value, upsStatus_t ups_status,uint32_t logtime);
//     //void writeTest();
//     //int writeLog(uint16_t status, uint16_t fault,time_t logTIme);
//     int readAllLogtoVector();
//     int readLogFromEnd(upslog_t *log);
//     int ReadPage(uint16_t page);
//     int ReadCurrentPage();
//     int ReadNextPage();
//     int ReadPrevPage();
//     int ReadFirstPage();
//     int ReadLastPage();
//     const char* getLogString();

//    const char *Module_state_event[16] = {
//         _("R_C_Start_Run_Event"),               /* 0 */
//         _("R_C_Re_Start_Event"),                /* 1 */
//         _("R_C_Stop_by_Command_Event"),         /* 2 */
//         _("R_C_Stop_by_Fault_Event"),           /* 3 */
//         _("DC_DC_Start_Run_Event"),             /* 4 */
//         _("DC_DC_Re_Start_Event"),              /* 5 */
//         _("DC_DC_Stop_by_Command_Event"),       /* 6 */
//         _("DC_DC_Stop_by_Fault_Event"),         /* 7 */
//         _("Inverter_Start_Run_Event"),          /* 8 */
//         _("Inverter_Re_Start_Run_Event"),       /* 9 */
//         _("Inverter_Stop_by_Command_Event"),    /* 10 */
//         _("Inverter_Stop_by_Fault_Event"),      /* 11 */
//         _("Transfer_to_Inverter_Event"),        /* 12 */
//         _("Transfer_to_Bypass_by_Com_Event"),   /* 13 */
//         _("Transfer_to_Bypass_by_fault_Event"), /* 14 */
//         _("")                                   /* 15 */
//     };
//     const char *ups_hw_state_alarm[16] = {
//         _("Input_OC"),                    /* 0 */
//         _("Inverter_Over_Current_fault"), /* 1 */
//         _("DC_Link_OV_fault"),            /* 2 */
//         _(""),                            /* 3 CONVERTER RUN/STOP STATE*/
//         _(""),                            /* 4 DC/DC CONVERTER RUN/STOP STATE*/
//         _("Converter_GDU_fault"),         /* 5 */
//         _("Inverter_GDU_fault"),          /* 6 */
//         _("DC_DC_GDU_fault"),             /* 7 */
//         _("Common_GDU_fault"),            /* 8 */
//         _(""),                            /* 9 INVERTER RUN/STOP STATE*/
//         _("Battery_Fuse_fault"),          /* 10 */
//         _("Module_OT"),                   /* 11 */
//         _(""),                            /* 12 Buzz ON/OFF Control from COM.*/
//         _("EEPROM_ERR"),                  /* 13*/
//         _("Battery_MCCB_Fault"),          /* 14 */
//         _(""),                            /* 15 TRANSFER RUN/STOP STATE*/
//     };
//     const char *ups_operation_fault_alarm[16] = {
//         _("converter_current_limit_fault"), /* 0 */
//         _("Vdc_link_OV_limit_fault"),       /* 1 */
//         _("Vdc_link_UV_limit_fault"),       /* 2 */
//         _("input_under_voltage_fault"),     /* 3 */
//         _("input_over_voltage_fault"),      /* 4 */
//         _("Input_Frequency_fault"),         /* 5 */
//         _("Inverter_Frequency_fault"),      /* 6 */
//         _("utility_line_failure"),          /* 7 */
//         _("battery_current_limit_fault"),   /* 8 */
//         _("Battery_OV_limit_fault"),        /* 9 */
//         _("battery_UV_limit_fault"),        /* 10 */
//         _("inverter_output_voltage_fault"), /* 11 */
//         _("Output_Overload"),               /* 12 */
//         _("Inverter_Overload_Stop"),        /* 13*/
//         _("OFFSET_Check_ERR"),              /* 14 */
//         _(""),                              /* 15 reserved*/
//     };
// };
    // enum eventType_t 
    // {
    //     EVENT = 0,
    //     FAULT = 1,
    // };
    // int readPrevLog(upslog_t *upsLog);
    // int readNextLog(upslog_t *upsLog);
    //int getLogFromFile(uint16_t index, upslog_t *retLog );
    //int readLogToVector(int16_t memoryPage);

    //const char * readStartLog();
    //const char * readLastLog();
    //const char * readPrevLog();
    //const char * readNextLog();

    //int readFirstLog(upslog_t *upsLog);
    //int readCurrentLog(upslog_t *upsLog);

    //void getLogStringToVlogs(const upslog_t *logArray);