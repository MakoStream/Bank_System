#pragma once
#include "../Command.h"
#include "../User.h"
#include <iostream>
#include "../mainProcess.h"
#include "../LogEye.h"


class userLoginCommand : public Command {

public:
    void execute(handleInfo& handle) override { // login <login> <password>
		int log_id = logEye.logTrace("login Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
        logEye.msgTrace(log_id, "input data", "*************************", true);

		string cmdStr(handle.sessionData.cmd);
		std::vector<std::string> args = split(cmdStr);

        handle.sessionData.hash[0] = 0;
        if (args.size() < 3) {
			throw_response(handle, "Not enough arguments for login command!");
			logEye.endTrace(log_id, FAILURE, "Not enough arguments for login command!");
            return;
        };
        
		logEye.commentTrace(log_id, "Attempting to login user: " + args[1]);
        char login[32]; char password[32];
        strcpy(login, args[1].c_str());
        strcpy(password, args[2].c_str());
        User user = getUser_byLogin(login);
        if (user.getId() == emptyUser.getId()) { 
			throw_response(handle, "User with this login does not exist!");
			logEye.endTrace(log_id, FAILURE, "User with this login does not exist!");
            return; 
        };
        

        if (user.checkPassword(password)) {
			logEye.commentTrace(log_id, "Password correct, logging in user: " + string(login));
            process.login(handle.sessionData.sessionId, login, password);
            Session& this_session = process.getSessionByID(handle.sessionData.sessionId);
            process.generateAuthKey(this_session, handle.sessionData);
            //cout << "1: " << this_session.auth_key << endl;
            strncpy(handle.sessionData.msg[0], user.getLogin(), sizeof(handle.sessionData.cmd) - 1);
			strncpy(handle.sessionData.msg[1], user.getName(), sizeof(handle.sessionData.cmd) - 1);
			strncpy(handle.sessionData.msg[2], user.getSurname(), sizeof(handle.sessionData.cmd) - 1);

			handle.sessionData.hash[0] = 1; // success login
            handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
            handle.sessionData.userId = user.getId();
            WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
			logEye.endTrace(log_id, SUCCESS, "User logged in successfully: " + string(login));
            return;
        };

		throw_response(handle, "Incorrect password!");
		logEye.endTrace(log_id, FAILURE, "Incorrect password for user: " + string(login));
        return;
    }

    std::string name() const override {
        return "login";
    }
};
