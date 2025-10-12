#pragma once
#include "../Command.h"
#include "../User.h"
#include <iostream>
#include "../mainProcess.h"

class userLoginCommand : public Command {

public:
    void execute(handleInfo& handle) override { // login <login> <password>
		string cmdStr(handle.sessionData.cmd);
		std::vector<std::string> args = split(cmdStr);

        if (args.size() < 3) {
            cout << "loggin: Args issue" << endl;
            return;
        };
        
        char login[32]; char password[32];
        strcpy(login, args[1].c_str());
        strcpy(password, args[2].c_str());
        User user = getUser_byLogin(login);
        if (user.getId() == emptyUser.getId()) { 
            strncpy(handle.sessionData.cmd, "This user does't exist!", sizeof(handle.sessionData.cmd) - 1);
            handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
            WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
            return; 
        };
        
        if (user.checkPassword(password)) {
            process.login(handle.sessionData.sessionId, login, password);
            strncpy(handle.sessionData.cmd, "Loggined!", sizeof(handle.sessionData.cmd) - 1);
            handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
            WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
            return;
        };

        strncpy(handle.sessionData.cmd, "Invalid password!", sizeof(handle.sessionData.cmd) - 1);
        handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
        WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
        
        return;
    }

    std::string name() const override {
        return "login";
    }
};
