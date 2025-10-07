#pragma once
#include "../mainProcess.h"
#include "../Logger.h"
#include "../DB_operations.h"
#include "../Command.h"
#include <iostream>

class CreateDBCommand : public Command {
    Logger* logger;  // вказівник на глобальний логгер
public:
    void execute(const std::vector<std::string>& args, handleInfo& handle) override {
        DB_create();
        //std::cout << "База даних створена!\n" 

		strncpy(handle.sessionData.cmd, "Database created!", sizeof(handle.sessionData.cmd) - 1);
		handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
		WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
		return;
    }

    std::string name() const override {
        return "create_db";
    }
};