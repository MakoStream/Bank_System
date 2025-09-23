#pragma once
#include "../Command.h"
#include "../DB_op.h"
#include <iostream>

class userLoginCommand : public Command {
    Logger* logger;
public:
    void execute(const std::vector<std::string>& args, Session& user) override {
        DB_list();
        logger->write("DB created");
        return;
    }

    std::string name() const override {
        return "login";
    }
};
