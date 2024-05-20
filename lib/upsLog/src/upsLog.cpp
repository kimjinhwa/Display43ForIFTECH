#include "upsLog.h"
#include <stdio.h>
#include "upsLog.h"
#include "math.h"
#include "string.h"
#include <vector>
#include <cstring>

//#include <RtcDS1302.h>
#include "RtcUtility.h"
#include <RtcDateTime.h>

upsLog::upsLog()
{
    strcpy(this->filename, "/spiffs/eventLog.hex");
    eventType = 0;
    logCount = 0;
    logid=0;
    numPerPage = 7;
    totalPage = 0;
    oldHWState.status =0x00;
}
/* filename은 어떤 것을 줘도 관계없다.  */
/* eventType: event는 0, alarm은 1을 준다.*/
upsLog::upsLog(const char* filename,uint16_t eventType)
{
    strcpy(this->filename, filename);
    this->eventType = eventType;
    if (this->eventType == 0) // EVENT
    {
        mask_moduleStatusEvent = 0b0111111111111111;
        mask_HwStatusEvent = 0x00;
        mask_upsOperationFault = 0x00;
    }
    else // ALARM
    {
        mask_moduleStatusEvent = 0x00;
        mask_HwStatusEvent = 0b0110110111100111;
        mask_upsOperationFault = 0b0111111111111111;
        ;
    }
    filePos = 0;
    logCount = 0;
    logid = 0;
    numPerPage = 7;
    totalPage = 0;
    oldHWState.status =0x00;
};
int upsLog::setEventCode(uint16_t moduleStatusEvent,uint16_t HwStatusEvent,uint16_t upsOperationFault)
{
    upslog_t log;

    //부저의 상태 변화는 마스킹하기 전에 체크한다.
    // 사용할 값만 마스킹한다
    HWState_t HWState;                     // 16  trnms->1
    HWState.status = HwStatusEvent;
    if (oldHWState.Bit.Buzz_ON_OFF != HWState.Bit.Buzz_ON_OFF)
    {
        // 값이 변경되었다.
        if (HWState.Bit.Buzz_ON_OFF) // 요청값이 1인경우에만 상태를 변경한다.
            runBuzzStatus = runBuzzStatus ? 0 : 1;
        // 1->0으로 변하던 0->1로 변하던 상태는 변경한다
        oldHWState.Bit.Buzz_ON_OFF = HWState.Bit.Buzz_ON_OFF;
    }
    //---------------------------------------
    moduleStatusEvent  &= mask_moduleStatusEvent;
    HwStatusEvent &= mask_HwStatusEvent;
    upsOperationFault &= mask_upsOperationFault;

    if (eventType == 0)
    { // Event검사일경우에는
        // 기존값과 변화가 없으면 리턴한다
        if (moduleStatusEvent == old_moduleStatusEvent){
            return  0;
        }
        old_moduleStatusEvent = moduleStatusEvent;
        log.logTime=0;
        log.modulestatus=moduleStatusEvent;
        log.operationFault=0;
        log.HWstatus = 0;
        // 이제 EVENT를 기록 한다.
        // 변화가 있기는 하나 값이 0->1로 되어야 하므로 
        if(log.modulestatus)
            writeLog(&log);
    }
    else if(eventType == 1)// Alarm Event
    {
        if (HwStatusEvent == old_HwStatusEvent && upsOperationFault == old_upsOperationFault)
        {  //값의 변화가 없으면 리턴한다
            return 0;
        }
        if (HwStatusEvent == 0 && upsOperationFault == 0 )
        {
            //값이 같지는 않으나 알람 이벤트가 0 값이면 
            //알람이 해소 된다
            alarmStatus = 0;
            runBuzzStatus =0;
        }

        ESP_LOGW("UI", "HwStatusEvent %d %d ", HwStatusEvent ,old_HwStatusEvent);
        ESP_LOGW("UI", "upsOperationFault %d %d ", upsOperationFault,old_upsOperationFault);
        old_HwStatusEvent = HwStatusEvent;
        old_upsOperationFault = upsOperationFault;
        log.modulestatus = 0;
        log.HWstatus = HwStatusEvent;
        log.operationFault = upsOperationFault;
        if(log.HWstatus || log.operationFault)  //둘중의 하나라도 값이 0이 아닐경우 로그를 기록한다.
        {
            // 이제 EVENT를 기록 한다.
            alarmStatus = 1;
            runBuzzStatus =1;
            writeLog(&log);
        }
    }
    return 1;
}
void upsLog::parseMessage(std::string *string_t,uint16_t value, upsStatus_t ups_status,uint32_t logtime)
{
    uint16_t index = 0x01;
    timeval tmv;
    tmv.tv_sec = logtime;
    RtcDateTime nowTime = RtcDateTime(tmv.tv_sec);
    const char **sArray[3] = {Module_state_event, ups_hw_state_alarm, ups_operation_fault_alarm};
    const char **status = sArray[ups_status];
        // MODULE_STATUS = 0,
        // HW_STATUS = 1,
        // OPERATIONAL_FAULT = 2,
    if(ups_status == MODULE_STATUS ) value = value & mask_moduleStatusEvent;
    if(ups_status == HW_STATUS ) value = value & mask_HwStatusEvent;
    if(ups_status == OPERATIONAL_FAULT ) value = value & mask_upsOperationFault;
    for (uint16_t i = 0; i < 16; i++)
    {
        if (value & (index << i))
        {
            string_t->append( String(nowTime.Year()-2000).c_str() );
            string_t->append( "/" );
            string_t->append( String(nowTime.Month()).c_str() );
            string_t->append( "/" );
            string_t->append( String(nowTime.Day()).c_str() );
            string_t->append( " " );
            string_t->append( String(nowTime.Hour()).c_str() );
            string_t->append( ":" );
            string_t->append( String(nowTime.Minute()).c_str() );
            string_t->append( ":" );
            string_t->append( String(nowTime.Second()).c_str() );
            string_t->append( " " );

            string_t->append(_(status[i]));
            string_t->append("\n");
            //string_t += "\r\n";
        }
    }
    //descString.resize(string_t->length());
    //std::strcpy(descString.data(),string_t->c_str());
}
/* 이 함수는 파일에서 현재 페이지를 읽고  
데이타를 logArray에 저장을 한다. 저장된 갯수는 actualReadCount
에 기록한다. 
event기록은 이벤트 메세지 구조체 하나에서 화면에 표시할 데이타를 만들고 
alarm은 2개의 구조체에서 데이타를 읽는 다.
alarm은 하나의 로그에 2개의 데이타가 기록되기 때문에 같이 parsing을 수행한다.
만들어지는 문자의 최대 배열은 numPerPage 에 의해 결정되면 이것은 한 화면에 디스플레이 할수 있는 
라인수이다.
*/
const char *upsLog::getLogString(const upslog_t *logArray)
{
    static std::string strRet;
    strRet.clear();
    if (eventType == 0)
    {
        parseMessage(&strRet, logArray->modulestatus, MODULE_STATUS, logArray->logTime); // 0 : moduleStatus
    }
    else
    {
        parseMessage(&strRet, logArray->HWstatus, HW_STATUS, logArray->logTime);               // 1 : moduleStatus
        parseMessage(&strRet, logArray->operationFault, OPERATIONAL_FAULT, logArray->logTime); // 1 : moduleStatus
    }
    return strRet.c_str(); // descString.data();
}

long upsLog::getFileSize()
{
    int bRet = -1;
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("\nFile Open Error");
        return bRet;
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    if (file_size > 0)
    {
        logCount = file_size / sizeof(upslog_t);
        totalPage = ceil((double)logCount / numPerPage);
    }
    else
    {
        logCount = 0;
        totalPage = 1;
        filePos= 0;
    }
    upslog_t log; 
    if(logCount)
    fseek(fp, (logCount-1)*sizeof(upslog_t), SEEK_SET);
    if(fread((upslog_t *)&log, 1, sizeof(upslog_t), fp)){
        logid =  log.logId;
    }
    fclose(fp);
    if(filePos==0 ) filePos = logCount -1;
    return file_size;
}
int upsLog::shrinkFile()
{
    FILE *fp_src;
    FILE *fp_desc;
    uint16_t start_pos= MAX_LOG_COUNT/2;
    fp_src = fopen(filename, "rb");
    fp_desc= fopen("spiffs/temp.hex", "wb+");//파일이 있어도 덥어쓰기 모드이다.
    upslog_t log;
    if (fp_src == NULL || fp_desc == NULL )
    {
        printf("\nFile Open Error");
        return -1;
    }
    fseek(fp_src, start_pos * sizeof(upslog_t), SEEK_SET);
    for(int i=start_pos;i<logCount;i++){
        if(fread((upslog_t *)&log, 1, sizeof(upslog_t), fp_src))
        fwrite((upslog_t *)&log, 1, sizeof(upslog_t), fp_desc);
    }
    fclose(fp_src);
    fclose(fp_desc); // 이제 반으로 줄인 파일이 존재 한다.
    fp_desc= fopen(filename, "wb+");
    fp_src = fopen("spiffs/temp.hex", "rb");
    if (fp_src == NULL || fp_desc == NULL )
    {
        printf("\nFile Open Error");
        return -1;
    }
    //temp.hex에 있는 데이타를 filename에 옮긴다
    while(!feof(fp_src)){
        if(fread((upslog_t *)&log, 1, sizeof(upslog_t), fp_src)){
        fwrite((upslog_t *)&log, 1, sizeof(upslog_t), fp_desc);

        }
    }
    fclose(fp_src);
    fclose(fp_desc); 
    return 1;
}

int upsLog::writeLog(upslog_t *log)
{
    long file_size = getFileSize();
    FILE *fp;
    int wSize;

    long file_position;

    timeval tmv;
    gettimeofday(&tmv, NULL);
    //RtcDateTime nowTime = RtcDateTime(tmv.tv_sec);
    log->logTime = tmv.tv_sec;

    if (logCount >= MAX_LOG_COUNT)
        shrinkFile();

    fp = fopen(filename, "ab+");
    if (fp == NULL)
    {
        printf("\nFile Open Error");
        return -1;
    }
    logCount++;
    log->logId = ++logid;
    fseek(fp, 0, logCount * sizeof(upslog_t));
    wSize = fwrite((upslog_t *)log, 1, sizeof(upslog_t), fp);
    fclose(fp);

    return wSize;

};
int upsLog::getLogFromFile(uint16_t index, upslog_t *retLog )
{
    int bRet = 0;
    if(index< 0 || index  >= logCount )
      return 0;
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        printf("\nFile Open Error");
        return 0;
    }
    fseek(fp, (index)*sizeof(upslog_t), SEEK_SET);
    bRet =fread((upslog_t *)retLog, 1, sizeof(upslog_t), fp);
    fclose(fp);
    return bRet;
}
int upsLog::readLastLog(upslog_t *upsLog)
{
    getFileSize();
    filePos = logCount - 1;
    return getLogFromFile(filePos, upsLog);
};
// int upsLog::readFirstLog(upslog_t *upsLog)
// {
//     getFileSize();
//     filePos = 0;
//     return getLogFromFile(filePos, upsLog);
// };
int upsLog::readCurrentLog(upslog_t *upsLog)
{
    getFileSize();
    return getLogFromFile(filePos, upsLog);
};
const char * upsLog::readCurrentLog(upslog_t *upsLog,int count)
{
    getFileSize();
    static std::string retStr;
    retStr="";
    ESP_LOGW("log","%s->%d(%d)\n\n",filename,filePos,logCount);
    //filePos는 최대 읽을수 있는 수를 설정한다.
    if(filePos < 0)filePos = 0;
    if(filePos >= logCount )filePos = logCount-1;
    int tempFilePos=filePos;
    for(int i=0;i<count;i++,tempFilePos--){
        if(tempFilePos< 0){
            tempFilePos=0;
            break;
        }
        if ( getLogFromFile(tempFilePos, upsLog) == 0) break;

        ESP_LOGW("log","%s->pos:%d id:%d(count:%d)-->tempFP %d",filename,filePos, upsLog->logId,logCount,tempFilePos);
        retStr.append( getLogString(upsLog));
    }
    return retStr.c_str();
};
int upsLog::readPrevLog(upslog_t *upsLog)
{
    filePos = filePos - 1;
    return getLogFromFile(filePos, upsLog);
};
int upsLog::readNextLog(upslog_t *upsLog)
{
    filePos = filePos + 1;
    return getLogFromFile(filePos, upsLog);
};