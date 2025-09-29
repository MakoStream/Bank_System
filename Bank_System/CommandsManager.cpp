#include "CommandsManager.h"
#include "commands/CreateDBCommand.h"
#include "commands/RegisterUserCommand.h"
#include "commands/PrintUserListCommand.h"
#include "commands/userLoginCommand.h"
#include <iostream>

CommandsManager::CommandsManager() {
    commands.push_back(std::make_unique<CreateDBCommand>());
    commands.push_back(std::make_unique<RegisterUserCommand>());
    commands.push_back(std::make_unique<PrintUserListCommand>());
    commands.push_back(std::make_unique<userLoginCommand>());
}

void CommandsManager::execute(std::string& cmdName, std::vector<std::string>& args, handleInfo& handle) {
    std::cout << "debug 1" << endl;
    for (auto& cmd : commands) {
        if (cmd->name() == cmdName) {
            cmd->execute(args, handle);
            std::cout << "debug 2" << endl;
            return;
        }
    }
    std::cout << "Невідома команда: " << cmdName << "\n";
}