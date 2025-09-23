#pragma once
#include "../mainProcess.h"
#include "../Logger.h"
#include "../DB_op.h"
#include "../Command.h"
#include <iostream>

class CreateDBCommand : public Command {
    Logger* logger;  // вказівник на глобальний логгер
public:
    void execute(const std::vector<std::string>& args, Session& user) override {
        DB_create();
        //std::cout << "База даних створена!\n" 
    }

    std::string name() const override {
        return "create_db";
    }
};