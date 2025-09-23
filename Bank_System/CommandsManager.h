#pragma once
#include "Command.h"
#include <vector>
#include <memory>
#include <string>

class CommandsManager {
private:
    std::vector<std::unique_ptr<Command>> commands;
public:
    CommandsManager();

    void execute(const std::string& cmdName, const std::vector<std::string>& args, Session& user);
};