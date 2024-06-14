#include "upsLog.h"
#include <stdio.h>
#include "upsLog.h"
#include "math.h"
#include "string.h"
#include <vector>
#include <cstring>
#include <stdexcept>

#include <RtcDS1302.h>
#ifndef WINDOWS 
// #include "RtcUtility.h"
// #include <RtcDateTime.h>
#endif
upsLog::upsLog()
{
    strcpy(this->filename, "/spiffs/eventLog.hex");
    eventType = FAULT_TYPE;
    logCount = 0;
    logid=0;
    totalPage = 0;
    oldHWState.status =0x00;
    currentMemoryPage =0;
}
upsLog::upsLog(eventType_t eventType)
{
    this->eventType = eventType;
    logCount = 0;
    logid=0;
    totalPage = 0;
    oldHWState.status =0x00;
    currentMemoryPage =0;
}
/* filename은 어떤 것을 줘도 관계없다.  */
/* eventType: event는 0, alarm은 1을 준다.*/
upsLog::upsLog(const char* filename,eventType_t eventType)
{
    strcpy(this->filename, filename);
    this->eventType = eventType;
    if (this->eventType == EVENT_TYPE) // EVENT
    {
        mask_moduleStatusEvent  = 0b0111111111111111;
        mask_HwStatusEvent      = 0b0110110111100111;
        mask_upsOperationFault  = 0b0111111111111111;
    }
    else // ALARM
    {
        mask_moduleStatusEvent  = 0x00;
        mask_HwStatusEvent      = 0b0110110111100111;
        mask_upsOperationFault  = 0b0111111111111111;
        ;
    }
    logCount = 0;
    logid = 0;
    totalPage = 0;
    oldHWState.status =0x00;
    // if(eventType==FAULT_TYPE)
    //     currentMemoryPage =0;
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

    if (eventType == EVENT_TYPE)
    { // Event검사일경우에는
        // 기존값과 변화가 없으면 리턴한다
        if (moduleStatusEvent == old_moduleStatusEvent 
            && HwStatusEvent == old_HwStatusEvent 
            && upsOperationFault == old_upsOperationFault){
            return  0;
        }
        //변화가 생겼다.
        old_moduleStatusEvent = moduleStatusEvent;
        old_HwStatusEvent = HwStatusEvent ;
        old_upsOperationFault = upsOperationFault ;

        log.logTime=0;
        log.modulestatus= mask_moduleStatusEvent & moduleStatusEvent;
        log.HWstatus = mask_HwStatusEvent &  HwStatusEvent ;
        log.operationFault = mask_upsOperationFault & upsOperationFault;
        // 이제 EVENT를 기록 한다.
        // 변화가 있기는 하나 값이 0->1로 되어야 하므로 
        if(log.modulestatus || log.HWstatus || log.operationFault )
            writeLog(&log);
    }
    else if(eventType == FAULT_TYPE)// Alarm Event
    {
        if (HwStatusEvent == old_HwStatusEvent && upsOperationFault == old_upsOperationFault)
        {  //값의 변화가 없으면 리턴한다
            return 0;
        }
        if (HwStatusEvent == 0 && upsOperationFault == 0 )
        {
            //값이 같지는 않으나 알람 이벤트가 0 값이면 
            //알람이 해소 된다
            vWarninglogs.clear();
            alarmStatus = 0;
            runBuzzStatus =0;
        }

        ESP_LOGW("UI", "HwStatusEvent %d %d ", HwStatusEvent ,old_HwStatusEvent);
        ESP_LOGW("UI", "upsOperationFault %d %d ", upsOperationFault,old_upsOperationFault);
        old_HwStatusEvent = HwStatusEvent;
        old_upsOperationFault = upsOperationFault;
        log.modulestatus = 0;
        log.HWstatus = mask_HwStatusEvent & HwStatusEvent;
        log.operationFault = mask_upsOperationFault & upsOperationFault;
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

/* Argument ToVlogs :  문자 배열에 저장할 것인지 아니면 vector에 
*  저장할 것인지를 결정한다.
*/
void upsLog::parseMessage(std::string *string_t,uint16_t logId,uint16_t value, upsStatus_t ups_status,uint32_t logtime)
{
    uint16_t index = 0x01;
    timeval tmv;
    char temp[3];
    tmv.tv_sec = logtime;
    printf("\ntmv.tv_sec = logtime %d\n",logtime);
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
            #ifdef WINDOWS
            string_t->append( std::to_string(nowTime.Year()+2000).c_str() );
            #else
            string_t->append( std::to_string(nowTime.Year()-2000).c_str() );
            #endif
            string_t->append( "/" );
            sprintf(temp,"%02d",nowTime.Month());
            string_t->append( std::string(temp).c_str() );
            string_t->append( "/" );
            sprintf(temp,"%02d",nowTime.Day());
            string_t->append( std::string(temp).c_str() );
            string_t->append( " " );
            sprintf(temp,"%02d",nowTime.Hour());
            string_t->append( std::string(temp).c_str() );
            string_t->append( ":" );
            sprintf(temp,"%02d",nowTime.Minute());
            string_t->append( std::string(temp).c_str() );
            string_t->append( ":" );
            sprintf(temp,"%02d",nowTime.Second());
            string_t->append( std::string(temp).c_str() );
            string_t->append( " " );

            #ifdef WINDOWS
            string_t->append(status[i]);
            #else
            string_t->append(_(status[i]));
            #endif
            if(eventType == EVENT_TYPE)
                vlogs.push_back(std::make_tuple(logId,*string_t));
            if(eventType == FAULT_TYPE){
                vWarninglogs.push_back(std::make_tuple(logId,*string_t));
            }
            string_t->clear();
            //string_t->append("\n");
        }
    }
    //descString.resize(string_t->length());
    //std::strcpy(descString.data(),string_t->c_str());
}
/* 이 함수는 파일에서 현재 페이지를 읽고  
데이타를 logArray에 저장을 한다. 저장된 갯수는 
에 기록한다. 
event기록은 이벤트 메세지 구조체 하나에서 화면에 표시할 데이타를 만들고 
alarm은 2개의 구조체에서 데이타를 읽는 다.
alarm은 하나의 로그에 2개의 데이타가 기록되기 때문에 같이 parsing을 수행한다.
만들어지는 
*/
const char *upsLog::getLogString(const upslog_t *logArray)
{
    static std::string strRet;
    strRet.clear();
    if (eventType == EVENT_TYPE)
    {
        parseMessage(&strRet, logArray->logId, logArray->modulestatus, MODULE_STATUS, logArray->logTime); // 0 : moduleStatus
        parseMessage(&strRet,logArray->logId, logArray->HWstatus, HW_STATUS, logArray->logTime);               // 1 : moduleStatus
        parseMessage(&strRet,logArray->logId, logArray->operationFault, OPERATIONAL_FAULT, logArray->logTime); // 1 : moduleStatus
    }
    else
    {
        printf("\nstep1\n");
        parseMessage(&strRet,logArray->logId, logArray->HWstatus, HW_STATUS, logArray->logTime);               // 1 : moduleStatus
        printf("\nstep2\n");
        parseMessage(&strRet,logArray->logId, logArray->operationFault, OPERATIONAL_FAULT, logArray->logTime); // 1 : moduleStatus
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
        ESP_LOGW("UI","\ngetFileSize File Open Error");
        return bRet;
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    //printf("\nlong file_size %d\n",file_size ) ;
    if (file_size > 0)
    {
        logCount = file_size / sizeof(upslog_t);
        totalPage = ceil((double)logCount / LOG_PER_PAGE);
        //printf("\ntotalPage  %d\n",totalPage) ;
        currentMemoryPage=logCount / LOG_PER_PAGE;
    }
    else
    {
        logCount = 0;
        totalPage = 1;
    }
    upslog_t log; 
    //마지막 로그를 하나 읽는다.
    if(logCount) fseek(fp, (logCount-1)*sizeof(upslog_t), SEEK_SET);
    if(fread((upslog_t *)&log, 1, sizeof(upslog_t), fp)){
        logid =  log.logId;
        //printf("\ngetFileSize %s ",log.message);
    }
    fclose(fp);
    ESP_LOGW("CURRENTLOG","\ncurrentMemoryPage %d logCount %d",currentMemoryPage,logCount);
    return file_size;
}
int upsLog::shrinkFile()
{
    FILE *fp_src;
    FILE *fp_desc;
    uint16_t start_pos= MAX_LOG_COUNT/2;
    fp_src = fopen(filename, "rb");
    fp_desc= fopen("spiffs/temp.hex", "wb+");//파일이 있어도 덥어쓰기 모드이다.
    upslog_t logs; 
    if (fp_src == NULL || fp_desc == NULL )
    {
        ESP_LOGW("UI","\nshrinkFile File Open Error");
        return -1;
    }
    fseek(fp_src, start_pos * sizeof(upslog_t), SEEK_SET);
    for(int i=start_pos;i<logCount;i++){
        if(fread((upslog_t *)&logs, 1, sizeof(upslog_t), fp_src))
        fwrite((upslog_t *)&logs, 1, sizeof(upslog_t), fp_desc);
    }
    fclose(fp_src);
    fclose(fp_desc); // 이제 반으로 줄인 파일이 존재 한다.
    fp_desc= fopen(filename, "wb+");
    fp_src = fopen("spiffs/temp.hex", "rb");
    if (fp_src == NULL || fp_desc == NULL )
    {
        ESP_LOGW("UI","\nshrinkFile File Open Error");
        return -1;
    }
    //temp.hex에 있는 데이타를 filename에 옮긴다
    while(!feof(fp_src)){
        if(fread((upslog_t *)&logs, 1, sizeof(upslog_t), fp_src)){
        fwrite((upslog_t *)&logs, 1, sizeof(upslog_t), fp_desc);

        }
    }
    fclose(fp_src);
    fclose(fp_desc); 
    return 1;
}

int upsLog::writeLog(upslog_t *log)
{
    //wirteLog을 하면 currentMemoryPage를 마지막 값으로 돌린다.
    if(eventType == FAULT_TYPE) return writeLogToVmem(log);
    long file_size = getFileSize();
    FILE *fp;
    int wSize;
    int16_t  filePos;

    long file_position;

    timeval tmv;
    gettimeofday(&tmv, NULL);
    //RtcDateTime nowTime = RtcDateTime(tmv.tv_sec);
    log->logTime = tmv.tv_sec;
    if (logCount > MAX_LOG_COUNT)
        shrinkFile();
    fp = fopen(filename, "ab+");
    if (fp == NULL)
    {
        ESP_LOGW("UI","\nwriteLog File Open Error");
        return -1;
    }
    logCount++;
    log->logId = ++logid;
    static std::string retStr;

    vlogs.clear();
    //vLogs 값을 할당한다.
    retStr.append( getLogString(log));
    int i=0;
    fseek(fp, 0, logCount * sizeof(upslog_t));
    ESP_LOGW("LOG","\nFile Write Log Data"); 
    for (const auto& entry : vlogs) {
        ESP_LOGW("LOG","\nLog ID(%d): %d , Log Message: %s" ,i++, std::get<0>(entry)  ,std::get<1>(entry).c_str() );
        strncpy((char *)&log->message, std::get<1>(entry).c_str(),50);
        //printf("-->%s",log->message);
        wSize = fwrite((upslog_t *)log, 1, sizeof(upslog_t), fp);
    }
    fclose(fp);
    //printf("log.message=\n%s",retStr.c_str());

    return wSize;

};
int upsLog::writeLogToVmem(upslog_t *log)
{

    timeval tmv;
    //std::tuple<uint16_t , std::string> logEntry; 
    gettimeofday(&tmv, NULL);
    
    log->logTime = tmv.tv_sec;

    //logCount = vWarninglogs.size();
    //tempLog.logId =0;
    vWarninglogs.clear();
    //if(logCount >0) logEntry = vlogs.at(logCount-1);
    //     messageStr = std::get<1>(logEntry);
    // log->logId = std::get<0>(logEntry)+1;
    // static std::string retStr;
    getLogString(log);
    int i=0;
    for (const auto& entry : vWarninglogs) {
        printf("\nvvWarninglogs(%d): %d , Log Message: %s\n" ,i++, std::get<0>(entry)  ,std::get<1>(entry).c_str() );
        strncpy((char *)&log->message, std::get<1>(entry).c_str(),50);
    }
    //printf("log.message=\n%s",retStr.c_str());
    return vWarninglogs.size();

};
const char * upsLog::readCurrentLogFromVector(directionType_t direction)
{
    upslog_t upsLog;
    int16_t  logMemPos;
    int bRet ;
    int16_t remain;
    logCount = vWarninglogs.size();
    remain = logCount%LOG_PER_PAGE; 
    totalPage = ceil((double)logCount / LOG_PER_PAGE);
    if(direction == CURRENTLOG){
        // currentMemoryPage--;
        // currentMemoryPage = currentMemoryPage<0?0:currentMemoryPage;
    }
    else if(direction == PREVLOG){
        currentMemoryPage--;
        currentMemoryPage = currentMemoryPage<0?0:currentMemoryPage;
    }
    else if(direction == NEXTLOG){
        currentMemoryPage++;
        currentMemoryPage = currentMemoryPage>=totalPage ? totalPage-1:currentMemoryPage;
        printf("\ncurrentMemoryPage----> %d ",currentMemoryPage);
    }

    logMemPos = (currentMemoryPage)*LOG_PER_PAGE ;
    logMemPos = logMemPos > logCount? logCount:logMemPos;

    printf("\nlogcount %d ",logCount);
    printf("\nremain %d ",remain);
    printf("\nlogMemPos %d ",logMemPos);
    printf("\ntotalPage %d ",totalPage);
    printf("\ncurrentMemoryPage %d ",currentMemoryPage);

    //logMemPos는 읽을 곳을 가르키고 있다.
    std::vector <std::tuple<uint16_t , std::string> >vectorlogs;
    std::tuple<uint16_t , std::string> logEntry; 
    static std::string retStr;
    vectorlogs.clear();
    for (int i = 0; i < LOG_PER_PAGE; logMemPos++, i++)
    {
        try
        {
            logEntry = vWarninglogs.at(logMemPos);
            retStr.clear();
            retStr += std::get<1>(logEntry);
            retStr.append("\n");
            vectorlogs.push_back(std::make_tuple(upsLog.logId, retStr));
                // printf("\n%d:fread %d logMemPos %d data %s",i, bRet, logMemPos, upsLog.message);
        }
        catch(const std::out_of_range& e){
            break;
        }
        // else break;
    }
    logMemPos = logMemPos<0 ? 0:logMemPos;
    printf("\nvLogVector size %d ",vectorlogs.size());
    retStr.clear();
    for (const auto &entry : vectorlogs)
    {
        //std::string app = std::get<1>(entry);
        retStr += std::get<1>(entry);//app;
    }
    return retStr.c_str() ;
}
const char * upsLog::readCurrentLog(directionType_t direction)
{
    ESP_LOGW("UI","\nEvent Type is %d direction is %d",eventType,direction);
    if(eventType == FAULT_TYPE) return readCurrentLogFromVector(direction);
    upslog_t upsLog;
    int16_t  filePos;
    int bRet = -1;
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        ESP_LOGW("UI","\nFile Open Error");
        return "";
    }
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    int16_t remain;
    if (file_size > 0)
        logCount = file_size / sizeof(upslog_t);

    remain = logCount%LOG_PER_PAGE; 
    totalPage = ceil((double)logCount / LOG_PER_PAGE);

    if(direction == CURRENTLOG){
        ESP_LOGW("CURRENTLOG","\ncurrentMemoryPage %d logCount %d",currentMemoryPage,logCount);
        // currentMemoryPage--;
        // currentMemoryPage = currentMemoryPage<0?0:currentMemoryPage;
    }
    else if(direction == PREVLOG){
        currentMemoryPage++;
        if(remain==0)
            currentMemoryPage = currentMemoryPage>totalPage ? totalPage:currentMemoryPage;
        else 
            currentMemoryPage = currentMemoryPage>totalPage-1 ? totalPage-1:currentMemoryPage;
        //currentMemoryPage=logCount / LOG_PER_PAGE;
    }
    else if(direction == NEXTLOG){
        currentMemoryPage--;
        currentMemoryPage = currentMemoryPage<0?0:currentMemoryPage;
    }

    if(remain>0)
    {
        if(currentMemoryPage>0)
            filePos = remain + (currentMemoryPage)*LOG_PER_PAGE;
        else 
            filePos = remain ;
    }
    else
        filePos = (currentMemoryPage)*LOG_PER_PAGE;
    filePos = filePos > logCount? logCount:filePos;

    printf("\nlogcount %d ",logCount);
    printf("\nremain %d ",remain);
    printf("\nfilePos %d ",filePos);
    printf("\ntotalPage %d ",totalPage);
    printf("\ncurrentMemoryPage %d ",currentMemoryPage);

    //filePos는 읽을 곳을 가르키고 있다.
    std::vector <std::tuple<uint16_t , std::string> >vectorlogs;
    static std::string retStr;
    vectorlogs.clear();
    for (int i = 0; i < LOG_PER_PAGE; filePos--, i++)
    {
        //filePos는 legth가 1일때 0을 갖고 있어야 읽을 수 있다.
        if (fseek(fp, (filePos-1) * sizeof(upslog_t), SEEK_SET) == 0)
        {
            bRet = fread((upslog_t *)&upsLog, 1, sizeof(upslog_t), fp);
            retStr.clear();
            retStr.append((char*)upsLog.message);
            retStr.append("\n");
            vectorlogs.push_back(std::make_tuple(upsLog.logId, retStr));
            //printf("\n%d:fread %d filePos %d data %s",i, bRet, filePos, upsLog.message);
        }
        else break;
    }
    fclose(fp);
    filePos = filePos<0 ? 0:filePos;
    printf("\nvLogVector size %d ",vectorlogs.size());
    retStr.clear();
    for (const auto &entry : vectorlogs)
    {
        //std::string app = std::get<1>(entry);
        retStr += std::get<1>(entry);//app;
    }
    return retStr.c_str() ;
};
// int upsLog::readPrevLog(upslog_t *upsLog)
// {
//     // filePos = filePos - 1;
//     // return getLogFromFile(filePos, upsLog);
//     return 0;
// };
// int upsLog::readNextLog(upslog_t *upsLog)
// {
//     // filePos = filePos + 1;
//     // return getLogFromFile(filePos, upsLog);
//     return 0;
// };

// const char *upsLog::readNextLog()
// {
//     // std::string messageStr;
//     // uint16_t endPos;
//     // uint16_t logid;
//     // std::tuple<uint16_t , std::string> logEntry; 
//     // if( vlogs.empty()) readLogToVector(999);
//     // if( vlogs.empty()) return "";
//     // printf("\nvlogs.size(): %d",vlogs.size());
//     // if(vlogs.size() < vlogMemPos+LOG_PER_PAGE) 
//     // {
//     //     currentMemoryPage++;
//     //     readLogToVector(currentMemoryPage);
//     // }
    
//     // //vLogs에 마지막 데이타를 채워 넣는다.
//     // //vlogMemPos = vlogs.size()-LOG_PER_PAGE>=0 ? vlogs.size()-LOG_PER_PAGE:0;
//     // endPos = vlogs.size()- vlogMemPos>= LOG_PER_PAGE? vlogMemPos+:LOG_PER_PAGEvlogs.size() ;
//     // //printf("vlogs.size()=%d vlogMemPos %d endPos %d",vlogs.size(),vlogMemPos,endPos);
//     // for(;vlogMemPos< endPos;vlogMemPos++){
//     //     logEntry = vlogs.at(vlogMemPos); 
//     //     logid = std::get<0>(logEntry);
//     //     messageStr = std::get<1>(logEntry);
//     //     ESP_LOGW("log", "[%d] id=%d, message=%s",vlogMemPos,logid,messageStr.c_str());
//     // }
//     // return messageStr.c_str();
// }
// const char *upsLog::readStartLog()
// {
//     //vlogs.push_back(std::make_tuple(logId,*string_t));
//     std::string messageStr;
//     uint16_t endPos;
//     uint16_t logid;
//     std::tuple<uint16_t , std::string> logEntry; 

//     //vLogs에 데이타를 채워 넣는다.
//     readLogToVector(0);
//     if( vlogs.empty()) return "";
//     vlogMemPos = 0;
//     endPos = vlogs.size()- vlogMemPos >= vlogMemPos+LOG_PER_PAGE? LOG_PER_PAGE:vlogs.size() ;
//     for(;vlogMemPos< endPos;vlogMemPos++){
//         logEntry = vlogs.at(vlogMemPos); 
//         logid = std::get<0>(logEntry);
//         messageStr = std::get<1>(logEntry);
//         ESP_LOGW("log", "[%d] id=%d, message=%s",vlogMemPos,logid,messageStr.c_str());
//     }
//     return messageStr.c_str();
// }
/* 
* readLogToVector(999)
*/
// const char *upsLog::readLastLog()
// {
//     // std::string messageStr;
//     // uint16_t endPos;
//     // uint16_t logid;
//     // std::tuple<uint16_t , std::string> logEntry; 

//     // //vLogs에 마지막 데이타를 채워 넣는다.
//     // readLogToVector(999);
//     // if( vlogs.empty()) return "";
//     // //vlogMemPos = vlogs.size()-LOG_PER_PAGE>=0 ? vlogs.size()-LOG_PER_PAGE:0;
//     // vlogMemPos  = 0;
//     // endPos = vlogs.size()- vlogMemPos>= LOG_PER_PAGE ? vlogMemPos+LOG_PER_PAGE:vlogs.size() ;
//     // //printf("vlogs.size()=%d vlogMemPos %d endPos %d",vlogs.size(),vlogMemPos,endPos);
//     // for(;vlogMemPos< endPos;vlogMemPos++){
//     //     logEntry = vlogs.at(vlogMemPos); 
//     //     logid = std::get<0>(logEntry);
//     //     messageStr = std::get<1>(logEntry);
//     //     ESP_LOGW("log", "[%d] id=%d, message=%s",vlogMemPos,logid,messageStr.c_str());
//     // }
//     // return messageStr.c_str();
//     return "";
// }
// const char *upsLog::readPrevLog()
// {
//     std::string retStr;
//     return retStr.c_str();
// }

    // getFileSize();
    // retStr="";
    // ESP_LOGW("log","%s->%d(%d)\n\n",filename,filePos,logCount);
    // //filePos는 최대 읽을수 있는 수를 설정한다.
    // if(filePos < 0)filePos = 0;
    // if(filePos >= logCount )filePos = logCount-1;
    // int tempFilePos=filePos;
    // for(int i=0;i<count;i++,tempFilePos--){
    //     if(tempFilePos< 0){
    //         tempFilePos=0;
    //         break;
    //     }
    //     if ( getLogFromFile(tempFilePos, upsLog) == 0) break;

    //     ESP_LOGW("log","%s->pos:%d id:%d(count:%d)-->tempFP %d",filename,filePos, upsLog->logId,logCount,tempFilePos);
    //     retStr.append( getLogString(upsLog));
    // }
    // return retStr.c_str();
// int upsLog::getLogFromFile(uint16_t index, upslog_t *retLog )
// {
//     int bRet = 0;
//     if(index< 0 || index  >= logCount )
//       return 0;
//     FILE *fp;
//     fp = fopen(filename, "rb");
//     if (fp == NULL)
//     {
//         return 0;
//     }
//     fseek(fp, (index)*sizeof(upslog_t), SEEK_SET);
//     bRet =fread((upslog_t *)retLog, 1, sizeof(upslog_t), fp);
//     fclose(fp);
//     return bRet;
// }
/*
* vLogs에 파일에 있는 데이타를 Windowing하여 채워 넣는다.
* memereyPage는 0~시작하며, 999는 마지막 페이지를 로드한다.
*/
// int upsLog::readLogToVector(int16_t memoryPage)
// {
//     getFileSize(); // logCount를 얻는다.
//     upslog_t upsLog;
//     int logPage = logCount/(LOG_PER_PAGE*MAX_TOTAL_PAGE);
//     printf("\nmemoryPage=%d",memoryPage);
//     if(memoryPage==999 || memoryPage==0)
//     {
//         vlogs.clear();
//     }
//     else
//     {
//         //mPos까지 데이타를 지우고 뒤에 데이타를 붙인다. 
//         vlogs.erase(vlogs.begin(),vlogs.begin()+vlogMemPos);
//     }
//     memoryPage = memoryPage>logPage ? logPage:memoryPage;
//     if(memoryPage<0)memoryPage=0;
//     int endMemoryLogPos = (memoryPage+1)*LOG_PER_PAGE*MAX_TOTAL_PAGE;// 5*20=100
//     endMemoryLogPos = endMemoryLogPos > logCount ? logCount:endMemoryLogPos;
//     int startMemoryLogPos;// = memoryPage*LOG_PER_PAGE*MAX_TOTAL_PAGE;// 5*20=100
//     startMemoryLogPos = endMemoryLogPos-100>0?endMemoryLogPos-100:0;
//     //startMemoryLogPos = startMemoryLogPos > logCount ? logCount : startMemoryLogPos; 
//     //endMemoryLogPos = endMemoryLogPos > logCount ? logCount:endMemoryLogPos;
//     int tempFilePos = endMemoryLogPos-1 ;
//     std::string messageStr;
//     printf("reqPage %d start: %d , end: %d" , memoryPage,startMemoryLogPos,endMemoryLogPos);
//     for (int i = startMemoryLogPos ; i < endMemoryLogPos  ; i++, tempFilePos--)
//     {
//         if (getLogFromFile(tempFilePos, &upsLog) == 0) break;
        
//         getLogStringToVlogs(&upsLog);
//         //vlogs.pop_back();
//         //ESP_LOGW("log", "message=%s",messageStr.c_str());
//     }
//     int i=0;
//     for (const auto& entry : vlogs) {
//             printf("Log ID(%d): %d , Log Message: %s\n" ,i++, std::get<0>(entry)  ,std::get<1>(entry).c_str() );
//     }
//     // uint16_t logid;
//     // std::tuple<uint16_t , std::string> logEntry; 
//     // for(int i=0;i< vlogs.size();i++){
//     //     logEntry = vlogs.at(i); 
//     //     logid = std::get<0>(logEntry);
//     //     messageStr = std::get<1>(logEntry);
//     //     ESP_LOGW("log", "[%d] id=%d, message=%s",i,logid,messageStr.c_str());
//     // }

//     filePos = logCount - 1;
//     currentMemoryPage = memoryPage;
//     return memoryPage;
// };
//  for (const auto& entry : vlogs) {
//         std::cout << "Log ID: " << std::get<0>(entry) << ", Log Message: " << std::get<1>(entry) << std::endl;
//     }
// int upsLog::readFirstLog(upslog_t *upsLog)
// {
//     getFileSize();
// };
// int upsLog::readCurrentLog(upslog_t *upsLog)
// {
//     getFileSize();
//     return getLogFromFile(filePos, upsLog);
// };
// void upsLog::getLogStringToVlogs(const upslog_t *logArray)
// {
//     static std::string strRet;
//     strRet.clear();
//     if (eventType == 0)
//     {
//         parseMessage(&strRet,logArray->logId, logArray->modulestatus, MODULE_STATUS, logArray->logTime); // 0 : moduleStatus
//     }
//     else
//     {
//         parseMessage(&strRet, logArray->logId,logArray->HWstatus, HW_STATUS, logArray->logTime);               // 1 : moduleStatus
//         parseMessage(&strRet,logArray->logId, logArray->operationFault, OPERATIONAL_FAULT, logArray->logTime); // 1 : moduleStatus
//     }
//     //return strRet.c_str(); // descString.data();
// }
        //filePos = filePos == -2 ? logCount:filePos;
        //currentMemoryPage--;
        // if(filePos<0){
        //     filePos = LOG_PER_PAGE;
        //     filePos = filePos >= logCount? logCount:filePos;
        // }
        // else{
        //     filePos = 2*LOG_PER_PAGE;
        //     filePos = filePos >= logCount? logCount:filePos;
        // }
