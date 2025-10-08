#pragma once
#include "../Command.h"
#include "../User.h"
#include <iostream>

class PrintUserListCommand : public Command {
    Logger* logger;
public:
    void execute(const std::vector<std::string>& args, handleInfo& handle) override {
        DB_list();
        strncpy(handle.sessionData.cmd, "User list is printed!", sizeof(handle.sessionData.cmd) - 1);
        handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
        WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
        return;
    }

    std::string name() const override {
        return "user_list";
    }
};
