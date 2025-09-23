#include "CommandsManager.h"
#include "commands/CreateDBCommand.h"
#include "commands/RegisterUserCommand.h"
#include "commands/PrintUserListCommand.h"
#include <iostream>

CommandsManager::CommandsManager() {
    commands.push_back(std::make_unique<CreateDBCommand>());
    commands.push_back(std::make_unique<RegisterUserCommand>());
    commands.push_back(std::make_unique<PrintUserListCommand>());
}

void CommandsManager::execute(const std::string& cmdName, const std::vector<std::string>& args) {
    for (auto& cmd : commands) {
        if (cmd->name() == cmdName) {
            cmd->execute(args);
            return;
        }
    }
    std::cout << "Невідома команда: " << cmdName << "\n";
}