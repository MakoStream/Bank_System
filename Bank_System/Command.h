#pragma once
#include "mainProcess.h"
#include <string>
#include <vector>
#include <windows.h>
#include "basic_functions.h"


extern struct sessionConstruct {
    int sessionId = 0;
    int hash[10];
    char cmd[256];
    char msg[5][256];
};

extern struct handleInfo {
    HANDLE& hPipe;
    sessionConstruct& sessionData;
    DWORD& bytesRead;
    DWORD& bytesWritten;
};

// Базовий клас для будь-якої команди
class Command {
public:
    virtual ~Command() = default;
    virtual void execute(handleInfo& handle) = 0; // чисто віртуальний метод
    virtual std::string name() const = 0; // ім'я команди
};