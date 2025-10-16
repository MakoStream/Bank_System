#pragma once
#include "../Command.h"
#include "../User.h"
#include <iostream>

class PrintUserListCommand : public Command {
    Logger* logger;
public:
    void execute(handleInfo& handle) override {
        string input(handle.sessionData.cmd);
        vector<string> args = split(input);
        int page;
        if (args.size() < 2) { page = 1; }
        else {
            if (!isStringDigit(args[1])) {
                cout << "Page is incorect" << endl;
                return;
            };
            page = stoi(args[1]);
        };
        DB_list(handle.sessionData.msg, page);
        strncpy(handle.sessionData.cmd, "User list is printed!", sizeof(handle.sessionData.cmd) - 1);
        handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
        WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
        return;
    }

    std::string name() const override {
        return "user_list";
    }
};
