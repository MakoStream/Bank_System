#pragma once
#include "mainProcess.h"
#include "Command.h"
#include <vector>
#include <memory>
#include "windows.h"
#include <string>

class CMD_Manager {
private:
    std::vector<std::unique_ptr<processCommand>> processCommands;
public:
    CMD_Manager(mainProcess& process);

    void execute(CMD_FS cmd_name, sessionConstruct& sessionData, HANDLE& hPipe, DWORD& bytesWritten);
};