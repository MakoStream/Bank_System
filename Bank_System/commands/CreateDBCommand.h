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
			throw_response(handle, "User database created!");
            return;
        }
        else if (args[1]=="accounts") {
			DB_create_accounts();
			//std::cout << "База даних створена!\n" 
			throw_response(handle, "Accounts database created!");
			return;
		}
		else {
			throw_response(handle, "Unknown database type!");
			return;
        };
        
    }

    std::string name() const override {
        return "create_db";
    }
};