
    // fseek(fp, page*numPerPage*sizeof(upslog_t), SEEK_SET);
    // upslog_t logArray[logCount];
    // upslog_t *log;
    // log = &logArray[0]

// int upsLog::readfilePage(int page){
//     long file_size =getFileSize();
//     if(page<0)page=0;
//     if(page>= totalPage)page=totalPage-1;
//     currentPage=page;

//     upslog_t *log;
//     log = &logArray[0];
//     FILE *fp;
//     fp = fopen(filename, "rb");
//     if(fp==NULL){
//         ESP_LOGE("file","File Open Error %s",filename);
//         return -1;
//     } 
//     size_t read_size;
   
//     fseek(fp, page*numPerPage*sizeof(upslog_t), SEEK_SET);
//     //printf("\nFile size %ld %d ", file_size, file_size / sizeof(upslog_t));
//     actualReadCount=0;
//     for(int i =0 ;i< (numPerPage>logCount? logCount:numPerPage) ;i++)
//     {
//         read_size = fread((upslog_t *)(log+i), 1, sizeof(upslog_t), fp); // if (read_size >= 1){ //     printf("\nid:%d  read_size %d  %d %d %d", log->logId,read_size, log->logTime, log->status, log->fault); // }
//         if(read_size<1)break;
//         actualReadCount++;
//     }
//     fclose(fp);

//     // for(int i =0 ;i< actualReadCount ;i++)
//     //      printf("\nid:%d  read_size %d time: %d moduleStatus %d hwStatus %d fault %d", 
//     //         logArray[i].logId,
//     //         read_size, 
//     //         logArray[i].logTime, 
//     //         logArray[i].modulestatus, 
//     //         logArray[i].HWstatus, 
//     //         logArray[i].operationFault);
//     return 0;

// }
// int upsLog::readfilePage(int page){

//     if(vlogs.size()<1)return 0;
//     totalPage = ceil((double)vlogs.size()/numPerPage);
//     if(page<0)page=0;
//     if(page>= totalPage)page=totalPage-1;
//     currentPage=page;

//     upslog_t *log;
//     log = &logArray[0];
//     actualReadCount=0;
//     for(int i =currVlogPos  ;i< currVlogPos+numPerPage    ;i++)
//     {
//         actualReadCount++;
//         logArray[i] = vlogs[i] ;
//     }
//     return 0;
// }
// int upsLog::ReadPage(uint16_t page)
// {
//     return readfilePage(page);
// }
// int upsLog::ReadCurrentPage(){
//     uint16_t page =  currentPage;
//     return readfilePage(page);
// };
// int upsLog::ReadNextPage(){
//     currVlogPos += numPerPage;
//     return readfilePage(++currentPage);
// };
// int upsLog::ReadPrevPage(){
//     currVlogPos -= numPerPage;
//     return ReadPage(--currentPage);
// };
// int upsLog::ReadFirstPage(){
//     currentPage=0;
//     currVlogPos = 0;
//     return ReadPage(currentPage);
// };
// int upsLog::ReadLastPage(){
//     currentPage = totalPage-1;
//     return ReadPage(currentPage);
// };

// int upsLog::readLogFromEnd(upslog_t *log)
// {
//     long file_size =getFileSize();
    
//     FILE *fp;
//     fp = fopen(filename, "rb");
//     if(fp==NULL){
//         ESP_LOGE("file","File Open Error %s",filename);
//         return -1;
//     } 
//     size_t read_size;
//     fseek(fp, 0, SEEK_END);
//     // 마지막에 저장된 NULL로 인해 하나가 크게 잡힌다.
//     // long file_size = ftell(fp);
//     // logCount = file_size / sizeof(upslog_t);
//     // printf("\nFile size %ld %d ", file_size, logCount);
//     fseek(fp, (long int) ((-1) * sizeof(upslog_t)), SEEK_CUR); // 하나를 뒤로 뺀다.
//     while (file_size > 0)
//     {
//         read_size = fread((upslog_t *)log, sizeof(upslog_t), 1, fp);
//         file_size -= read_size * sizeof(upslog_t);
//         fseek(fp, (long int)((-2) * sizeof(upslog_t)), SEEK_CUR); // 이제 2개를 뒤로 빼야 앞의 것을 읽을 수 있다
//         //printf("\nid:%d  read_size %d  %d %d %d", log->logId,read_size, log->logTime, log->modulestatus, log->operationFault);
//     }
//     fclose(fp);
//     tm *nowTime;
//     // nowTime = localtime(&log.logTime);
//     return 0;
// }
//         //for(int i=0;i<logCount;i++)
        //fseek(fp, (-1) * sizeof(upslog_t), SEEK_END);
        // while (file_size > 0)
        // {
        //     read_size = fread((upslog_t *)&log, sizeof(upslog_t), 1, fp);

        //     //if (read_size >= 1)
        //     {
        //         read_size = sizeof(upslog_t);
        //         printf("\nread_size %d  %d %d %d", read_size, log.logTime, log.modulestatus, log.operationfault);
        //         fseek(fp, (-1) * sizeof(upslog_t), SEEK_CUR);
        //         file_size -= read_size;
        //     }
        //     // printf("\nfile_size  %d ", file_size);
        // }

    // tm *nowTime;
    //_getsystime(nowTime);
    // time_t time_t_now = time(NULL);
    //  _localtime32_s(nowTime,(long *)&time_t_now);
    // log.logTime = time_t_now;
    // nowTime = _localtime32((long *)&log.logTime);
    //std::string retStr="";
        // retStr += std::to_string( logArray[i].logId) ;
        // retStr += " ";
    //static std::vector<char> descString;
    //descString.resize(retStr.length());
    //descString.resize(retStr.length());
    //static std::vector<char> descString(retStr.length());
    //std::strcpy(descString.data(),retStr.c_str());
// int upsLog::writeLog(uint16_t status, uint16_t fault,time_t logTIme)
// {
//     long file_size =getFileSize();
//     FILE *fp;
//     upslog_t log;
//     log.modulestatus = status;
//     log.operationFault= fault;
//     log.logTime = logTIme;
//     fp = fopen(filename, "rb+");
//     if(fp==NULL){
//         printf("\nFile Open Error"); 
//         return -1;
//     } 
//     logCount++;
//     log.logId = logCount;
//     int wSize = fwrite((upslog_t *)&log, sizeof(upslog_t), 1, fp);
//     fclose(fp);
//     return wSize ;
// };

//static std::vector<char> descString;

