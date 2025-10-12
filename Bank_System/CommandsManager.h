#pragma once
#include "Command.h"
#include <vector>
#include <memory>
#include <string>
#include "basic_functions.h"

class CommandsManager {
private:
    std::vector<std::unique_ptr<Command>> commands;
public:
    CommandsManager();

    void execute(handleInfo& handle);
};