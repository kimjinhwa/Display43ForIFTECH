#ifndef _FILE_SYSTEM_H
#define  _FILE_SYSTEM_H
#include <Arduino.h>
//#include <mainGrobal.h>

class LittleFileSystem
{
    public:
    LittleFileSystem();
    void littleFsInitFast(int bformat);
    void littleFsInit(int bformat);
    void setOutputStream(Print* stream);
    int rm(String fileName);
    void listDir(const char *path, char *match);
    int format();
    void cat(String finename);
    int fnmatch(const char *pattern, const char *string, int flags);
    const char* rangematch(const char *pattern, char test, int flags); 
    void df();

    private:
    Print* outputStream;
};
#endif