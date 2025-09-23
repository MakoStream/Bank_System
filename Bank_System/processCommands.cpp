//#include "Command.h"
#include "Command.h"
#include "windows.h"
#include "processCommands/getSId.h"
#include "processCommands.h"
//#include "commands/RegisterUserCommand.h"
//#include "commands/PrintUserListCommand.h"
#include <iostream>
#include <vector>

CMD_Manager::CMD_Manager(mainProcess& process) {
    processCommands.push_back(std::make_unique<getSId>(process));
    //commands.push_back(std::make_unique<RegisterUserCommand>());
    //commands.push_back(std::make_unique<PrintUserListCommand>());
}

void CMD_Manager::execute(CMD_FS cmd_name, sessionConstruct& sessionData, HANDLE hPipe, DWORD& bytesWritten) {
    for (auto& cmd : processCommands) {
        if (cmd->name() == cmd_name) {
            cmd->execute(cmd_name, sessionData, hPipe, bytesWritten);
            return;
        }
    }
    std::cout << "Невідома команда: " << "" << "\n";
}