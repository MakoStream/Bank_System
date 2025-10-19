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
			throw_response(handle, "Not enough arguments for login command!");
            return;
        };
        
        char login[32]; char password[32];
        strcpy(login, args[1].c_str());
        strcpy(password, args[2].c_str());
        User user = getUser_byLogin(login);
        if (user.getId() == emptyUser.getId()) { 
			throw_response(handle, "User with this login does not exist!");
            return; 
        };
        
        if (user.checkPassword(password)) {
            
            process.login(handle.sessionData.sessionId, login, password);
            Session& this_session = process.getSessionByID(handle.sessionData.sessionId);
            process.generateAuthKey(this_session, handle.sessionData);
            //cout << "1: " << this_session.auth_key << endl;
            strncpy(handle.sessionData.msg[0], user.getLogin(), sizeof(handle.sessionData.cmd) - 1);
			strncpy(handle.sessionData.msg[1], user.getName(), sizeof(handle.sessionData.cmd) - 1);
			strncpy(handle.sessionData.msg[2], user.getSurname(), sizeof(handle.sessionData.cmd) - 1);

			handle.sessionData.hash[0] = 1; // success login
            handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
            WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
            return;
        };

		throw_response(handle, "Incorrect password!");
        
        return;
    }

    std::string name() const override {
        return "login";
    }
};
