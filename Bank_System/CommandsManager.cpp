#include "CommandsManager.h"

#include "commands/CreateDBCommand.h"
#include "commands/RegisterUserCommand.h"
#include "commands/PrintUserListCommand.h"
#include "commands/userLoginCommand.h"
#include "commands/getSessionIdCommand.h"

#include <iostream>





CommandsManager::CommandsManager() {
    commands.push_back(std::make_unique<CreateDBCommand>());
    commands.push_back(std::make_unique<RegisterUserCommand>());
    commands.push_back(std::make_unique<PrintUserListCommand>());
    commands.push_back(std::make_unique<userLoginCommand>());
	commands.push_back(std::make_unique<getSessionIdCommand>());
}


void CommandsManager::execute(handleInfo& handle) {
	string cmdStr(handle.sessionData.cmd);
    std::vector<std::string> args = split(cmdStr);
	string cmdName = args[0];
    for (auto& cmd : commands) {
        if (cmd->name() == cmdName) {
            cmd->execute(handle);
            strncpy(handle.sessionData.cmd, "", sizeof(handle.sessionData.cmd) - 1);
            handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
            return;
        }
    }
    std::cout << "Невідома команда: " << cmdName << "\n";
};

