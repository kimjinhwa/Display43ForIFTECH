/*
   Copyright (c) 2019 Stefan Kremser
   This software is licensed under the MIT License. See the license file for details.
   Source: github.com/spacehuhn/SimpleCLI
 */

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

#include "SimpleCLI.h"
#include <RtcDS1302.h>
#include "EEPROM.h"

#include "../../../src/myBlueTooth.h"

LittleFileSystem lsFile;
SimpleCLI simpleCli;
static char TAG[] ="CLI" ;
extern "C" {
#include "c/cmd.h"       // cmd
#include "c/parser.h"    // parse_lines
#include "c/cmd_error.h" // cmd_error_destroy
}
int makeRelayControllData(uint8_t *buf,uint8_t modbusId,uint8_t funcCode, uint16_t address, uint16_t len);
int readResponseData(uint8_t modbusId,uint8_t funcCode, uint8_t *buf,uint8_t len,uint16_t timeout);
void setRtcNewTime(RtcDateTime rtc);


void ls_configCallback(cmd *cmdPtr){
  Command cmd(cmdPtr);
  Argument arg = cmd.getArgument(0);
  String argVal = arg.getValue();
  lsFile.listDir("/spiffs/", NULL);
}
void rm_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  Argument arg = cmd.getArgument(0);
  String argVal = arg.getValue();
  simpleCli.outputStream->printf("\r\n");

  if (argVal.length() == 0)
    return;

  lsFile.rm(argVal);
  // if (!argVal.startsWith("*"))
  // {
  //   argVal = String("/spiffs/") + argVal;
  // }
};
void df_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  lsFile.df();
}
void reboot_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  simpleCli.outputStream->println( "\r\nNow System Rebooting...\r\n");
  esp_restart();
}
void format_configCallback(cmd *cmdPtr)
{
  simpleCli.outputStream->printf("\r\nWould you system formating(Y/n)...\r\n");
  int c = 0;
  while (1)
  {
    if (simpleCli.inputStream->available())
      c = Serial.read();

    if (c == 'Y')
    {
      lsFile.littleFsInit(1);
      simpleCli.outputStream->printf("\r\nSystem format completed\r\n");
      return;
    }
    if (c == 'n')
      return;
  }
}
void cat_configCallback(cmd *cmdPtr)
{
  Command cmd(cmdPtr);
  Argument arg = cmd.getArgument(0);
  String argVal = arg.getValue();
  simpleCli.outputStream->printf("\r\n");

  if (argVal.length() == 0)
    return;
  argVal = String("/spiffs/") + argVal;

  lsFile.cat(argVal);
}

void errorCallback(cmd_error *errorPtr)
{
  CommandError e(errorPtr);

  simpleCli.outputStream->printf( (String("ERROR: ") + e.toString()).c_str());

  if (e.hasCommand())
  {
    simpleCli.outputStream->printf(  (String("Did you mean? ") + e.getCommand().toString()).c_str());
  }
  else
  {
     simpleCli.outputStream->printf(  simpleCli.toString().c_str());
  }
}
void help_Callback(cmd *cmdptr){

}
void SimpleCLI::setOutputStream(Print *outputStream ){
    this->outputStream = outputStream;
}

void SimpleCLI::setInputStream(Stream *inputStream ){
        this->inputStream = inputStream ;
}
SimpleCLI::SimpleCLI(int commandQueueSize, int errorQueueSize,Print *outputStream ) : commandQueueSize(commandQueueSize), errorQueueSize(errorQueueSize) {
  this->inputStream = &Serial;
  Command cmd_config = addCommand("ls",ls_configCallback);
  cmd_config.setDescription(" File list \r\n ");
  cmd_config =  addSingleArgCmd("cat", cat_configCallback);
  cmd_config = addSingleArgCmd("rm", rm_configCallback);
  cmd_config = addSingleArgCmd("format", format_configCallback);
  cmd_config = addCommand("df", df_configCallback);
  cmd_config = addSingleArgCmd("reboot", reboot_configCallback);
  simpleCli.setOnError(errorCallback);
  cmd_config= addCommand("help",help_Callback);
}

SimpleCLI::~SimpleCLI() {
    cmd_destroy_rec(cmdList);
    cmd_destroy_rec(cmdQueue);
    cmd_error_destroy_rec(errorQueue);
}

void SimpleCLI::pause() {
    pauseParsing = true;
}

void SimpleCLI::unpause() {
    pauseParsing = false;

    // Go through queued errors
    while (onError && errored()) {
        onError(getError().getPtr());
    }

    // Go through queued commands
    if(available()) {
        cmd* prev = NULL;
        cmd* current = cmdQueue;
        cmd* next = NULL;

        while(current) {
            next = current->next;

            // Has callback, then run it and remove from queue
            if(current->callback) {
                current->callback(current);
                if(prev) prev->next = next;
                cmd_destroy(current);
            } else {
                prev = current;
            }

            current = next;
        }
    }
}

void SimpleCLI::parse(const String& input) {
    parse(input.c_str(), input.length());
}

void SimpleCLI::parse(const char* input) {
    if (input) parse(input, strlen(input));
}

void SimpleCLI::parse(const char* str, size_t len) {
    // Split str into a list of lines
    line_list* l = parse_lines(str, len);

    // Go through all lines and try to find a matching command
    line_node* n = l->first;

    while (n) {
        cmd* h       = cmdList;
        bool success = false;
        bool errored = false;

        while (h && !success && !errored) {
            cmd_error* e = cmd_parse(h, n);

            // When parsing was successful
            if (e->mode == CMD_PARSE_SUCCESS) {
                if (h->callback && !pauseParsing) h->callback(h);
                else cmdQueue = cmd_push(cmdQueue, cmd_copy(h), commandQueueSize);

                success = true;
            }

            // When command name matches but something else went wrong, exit with error
            else if (e->mode > CMD_NOT_FOUND) {
                if (onError && !pauseParsing) {
                    onError(e);
                } else {
                    errorQueue = cmd_error_push(errorQueue, cmd_error_copy(e), errorQueueSize);
                }

                errored = true;
            }

            // When command name does not match

            /*else (e->mode <= CMD_NOT_FOUND) {

               }*/

            cmd_error_destroy(e);

            cmd_reset_cli(h);

            h = h->next;
        }

        // No error but no success either => Command could not be found
        if (!errored && !success) {
            cmd_error* e = cmd_error_create_not_found(NULL, n->words->first);

            if (onError && !pauseParsing) {
                onError(e);
            } else {
                errorQueue = cmd_error_push(errorQueue, cmd_error_copy(e), errorQueueSize);
            }

            cmd_error_destroy(e);

            errored = true;
        }

        n = n->next;
    }

    line_list_destroy(l);
}

bool SimpleCLI::available() const {
    return cmdQueue;
}

bool SimpleCLI::errored() const {
    return errorQueue;
}

bool SimpleCLI::paused() const {
    return pauseParsing;
}

int SimpleCLI::countCmdQueue() const {
    cmd* h = cmdQueue;
    int  i = 0;

    while (h) {
        h = h->next;
        ++i;
    }

    return i;
}

int SimpleCLI::countErrorQueue() const {
    cmd_error* h = errorQueue;
    int i        = 0;

    while (h) {
        h = h->next;
        ++i;
    }

    return i;
}

Command SimpleCLI::getCmd() {
    if (!cmdQueue) return Command();

    // "Pop" cmd pointer from queue
    cmd* ptr = cmdQueue;

    cmdQueue = cmdQueue->next;

    // Create wrapper class and copy cmd
    Command c(ptr, COMMAND_TEMPORARY);

    // Destroy old cmd from queue
    cmd_destroy(ptr);

    return c;
}

Command SimpleCLI::getCmd(String name) {
    return getCmd(name.c_str());
}

Command SimpleCLI::getCmd(const char* name) {
    if (name) {
        cmd* h = cmdList;

        while (h) {
            if (cmd_name_equals(h, name, strlen(name), h->case_sensetive) == CMD_NAME_EQUALS) return Command(h);
            h = h->next;
        }
    }
    return Command();
}

Command SimpleCLI::getCommand() {
    return getCmd();
}

Command SimpleCLI::getCommand(String name) {
    return getCmd(name);
}

Command SimpleCLI::getCommand(const char* name) {
    return getCmd(name);
}

CommandError SimpleCLI::getError() {
    if (!errorQueue) return CommandError();

    // "Pop" cmd_error pointer from queue
    cmd_error* ptr = errorQueue;
    errorQueue = errorQueue->next;

    // Create wrapper class and copy cmd_error
    CommandError e(ptr, COMMAND_ERROR_TEMPORARY);

    // Destroy old cmd_error from queue
    cmd_error_destroy(ptr);

    return e;
}

void SimpleCLI::addCmd(Command& c) {
    if (!cmdList) {
        cmdList = c.cmdPointer;
    } else {
        cmd* h = cmdList;

        while (h->next) h = h->next;
        h->next = c.cmdPointer;
    }

    c.setCaseSensetive(caseSensetive);
    c.persistent = true;
}

Command SimpleCLI::addCmd(const char* name, void (* callback)(cmd* c)) {
    Command c(cmd_create_default(name));

    c.setCallback(callback);
    addCmd(c);

    return c;
}

Command SimpleCLI::addBoundlessCmd(const char* name, void (* callback)(cmd* c)) {
    Command c(cmd_create_boundless(name));

    c.setCallback(callback);
    addCmd(c);

    return c;
}

Command SimpleCLI::addSingleArgCmd(const char* name, void (* callback)(cmd* c)) {
    Command c(cmd_create_single(name));

    c.setCallback(callback);
    addCmd(c);

    return c;
}
Command SimpleCLI::addCommand(const char* name, void (* callback)(cmd* c)) {
    return addCmd(name, callback);
}

Command SimpleCLI::addBoundlessCommand(const char* name, void (* callback)(cmd* c)) {
    return addBoundlessCmd(name, callback);
}

Command SimpleCLI::addSingleArgumentCommand(const char* name, void (* callback)(cmd* c)) {
    return addSingleArgCmd(name, callback);
}

String SimpleCLI::toString(bool descriptions) const {
    String s;

    toString(s, descriptions);
    return s;
}

void SimpleCLI::toString(String& s, bool descriptions) const {
    cmd* h = cmdList;

    while (h) {
        Command(h).toString(s, descriptions);
        if (descriptions) s += "\r\n";
        s += "\r\n";
        h  = h->next;
    }
}

void SimpleCLI::setCaseSensetive(bool caseSensetive) {
    this->caseSensetive = caseSensetive;

    cmd* h = cmdList;

    while (h) {
        h->case_sensetive = caseSensetive;
        h                 = h->next;
    }
}

void SimpleCLI::setCaseSensitive(bool caseSensitive) {
    setCaseSensetive(caseSensitive);
}

void SimpleCLI::setOnError(void (* onError)(cmd_error* e)) {
    this->onError = onError;
}

void SimpleCLI::setErrorCallback(void (* onError)(cmd_error* e)) {
    setOnError(onError);
}