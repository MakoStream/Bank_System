#include "CommandsManager.h"

#include "commands/CreateDBCommand.h"
#include "commands/RegisterUserCommand.h"
#include "commands/PrintUserListCommand.h"
#include "commands/userLoginCommand.h"
#include "commands/userLogoutCommand.h"
#include "commands/getSessionIdCommand.h"
#include "commands/PrintAccountListCommand.h"
#include "commands/NewAccountCommand.h"
#include "commands/AccountVerifyCommand.h"
#include "commands/AccountBanCommand.h"
#include "commands/UserInfoCommand.h"
#include "commands/DebugOnCommand.h"
#include "commands/UnitTestCommand.h"


#include <iostream>


CommandsManager::CommandsManager() {
    commands.push_back(std::make_unique<CreateDBCommand>());
    commands.push_back(std::make_unique<RegisterUserCommand>());
    commands.push_back(std::make_unique<PrintUserListCommand>());
    commands.push_back(std::make_unique<userLoginCommand>());
	commands.push_back(std::make_unique<userLogoutCommand>());
	commands.push_back(std::make_unique<getSessionIdCommand>());
	commands.push_back(std::make_unique<PrintAccountListCommand>());
	commands.push_back(std::make_unique<NewAccountCommand>());
	commands.push_back(std::make_unique<AccountVerifyCommand>());
	commands.push_back(std::make_unique<AccountBanCommand>());
    commands.push_back(std::make_unique<AccountUnbanCommand>());
    commands.push_back(std::make_unique<UserInfoCommand>());
    commands.push_back(std::make_unique<DebugOnCommand>());
    commands.push_back(std::make_unique<UnitTestCommand>());
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

