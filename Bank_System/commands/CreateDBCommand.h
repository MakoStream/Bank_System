#pragma once
#include "../mainProcess.h"
#include "../Logger.h"
#include "../User.h"
#include "../Command.h"
#include <iostream>

class CreateDBCommand : public Command {
public:
    void execute(handleInfo& handle) override {
        string input(handle.sessionData.cmd);
        vector<string> args = split(input);
        if (args[1] == "users") {
            DB_create();
            //std::cout << "База даних створена!\n" 

            strncpy(handle.sessionData.cmd, "User database created!", sizeof(handle.sessionData.cmd) - 1);
            handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
            WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
            return;
        }
        else if (args[1]=="accounts") {
			DB_create_accounts();
			//std::cout << "База даних створена!\n" 

			strncpy(handle.sessionData.cmd, "Account database created!", sizeof(handle.sessionData.cmd) - 1);
			handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
			WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
			return;
		}
		else {
			std::cout << "Unknown database type: " << args[1] << "\n";

			strncpy(handle.sessionData.cmd, "Unknown database type!", sizeof(handle.sessionData.cmd) - 1);
			handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
			WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
			return;
        };
        
    }

    std::string name() const override {
        return "create_db";
    }
};