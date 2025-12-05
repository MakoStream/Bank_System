/**
@file CommandsManager.cpp
@brief Implementation of CommandsManager class methods.
@details Defines the constructor to register all commands and the execute method
to process incoming commands from clients.Includes both regular and debug / test commands.
@note Required headers : CommandsManager.h, headers of all command classes, <iostream>.
*/

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
#include "commands/AccountInfoCommand.h"
#include "commands/SetAccountComand.h"
#include "commands/TransactionRequestCommand.h"
#include "commands/getUserAccountsCommand.h"
#include "commands/GetUserTransactionsCommand.h"


#include "commands/DEBUG/DEBUG_setAccountCurrencyCommand.h"
#include "commands/DEBUG/DEBUG_removeDBCommand.h"
#include "commands/DEBUG/DEBUG_printSessionsCommand.h"

#include <iostream>

/**
@brief Registers all available command objects.
@details The constructor adds instances of all concrete Command classes
to the internal vector, including both standard and debug/test commands.
@note Used internally when creating a CommandsManager object.
*/
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
    commands.push_back(std::make_unique<AccountInfoCommand>());
    commands.push_back(std::make_unique<SetAccountCommand>());
    commands.push_back(std::make_unique<TransactionRequestPANCommand>());
    commands.push_back(std::make_unique<TransactionRequestIBANCommand>());
    commands.push_back(std::make_unique<TransactionRequestListCommand>());
    commands.push_back(std::make_unique<TransactionInfoCommand>());
    commands.push_back(std::make_unique<getUserAccountsCommand>());
    commands.push_back(std::make_unique<AccountInfoIdCommand>());
    commands.push_back(std::make_unique<GetUserTransactionsCommand>());


    // Debug commands
    commands.push_back(std::make_unique<DEBUG_setAccountCurrencyCommand>());
    commands.push_back(std::make_unique<DEBUG_removeDBCommand>());
	commands.push_back(std::make_unique<DEBUG_printSessionCommand>());
};

/**
@brief Executes a command based on the string received from a client.
@details Parses the command string from sessionData, finds the matching Command object,
executes it, and clears the cmd field. If the command does not exist, sends a response
to the client indicating an unknown command.
@param handle Reference to handleInfo containing session data and pipe handle.
@note Calls split() from basic_functions.h to parse the command string.
@note Calls throw_response() to notify client of unknown commands.
*/
void CommandsManager::execute(handleInfo& handle) {
	string cmdStr(handle.sessionData.cmd);
    std::vector<std::string> args = split(cmdStr);
	string cmdName = args[0];
    for (auto& cmd : commands) {
        if (cmd->name() == cmdName) {
			cout << "Виконання команди: " << cmdName << "\n";
            cmd->execute(handle);
            strncpy(handle.sessionData.cmd, "", sizeof(handle.sessionData.cmd) - 1);
            handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
            return;
        }
    }
    std::cout << "Невідома команда: " << cmdName << "\n";
    throw_response(handle, "Невідома команда");
};

