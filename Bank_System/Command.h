#pragma once
#include "mainProcess.h"
#include <string>
#include <vector>
#include <windows.h>


extern enum CMD_FS { NONE, GET_SID, LOGIN, LOGOUT };

extern struct sessionConstruct {
    int sessionId = 0;
    int hash[10];
    CMD_FS cmd_fs = NONE;
    char cmd[256];
};

// Базовий клас для будь-якої команди
class Command {
public:
    virtual ~Command() = default;
    virtual void execute(const std::vector<std::string>& args, Session& user) = 0; // чисто віртуальний метод
    virtual std::string name() const = 0; // ім'я команди
};

class processCommand {
public:
    virtual ~processCommand() = default;
    virtual void execute(CMD_FS cmd_name, sessionConstruct& sessionData, HANDLE hPipe, DWORD& bytesWritten) = 0;
    virtual CMD_FS name() const = 0;
};