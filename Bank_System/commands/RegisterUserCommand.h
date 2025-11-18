#pragma once
#include "../Command.h"
#include "../User.h"
#include <iostream>
#include "../mainProcess.h"
#include "../LogEye.h"

/**
 * @class RegisterUserCommand
 * @brief Registers a new user in the system.
 * @details Validates input parameters, creates user, stores in DB,
 * logs all actions via logEye.
 * @note Requires: handleInfo, logEye, User
 * @note Syntax: reg_user <login> <password> <name> <surname> <phone>
 */
class RegisterUserCommand : public Command {
public:
    void execute(handleInfo& handle) override { // reg_user <login> <password> <name> <surname> <phone> //example: reg_user user1 pass1 John Doe 1234567890
		int log_id = logEye.logTrace("reg_user Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);
        
        string input (handle.sessionData.cmd);
		vector<string> args = split(input);

		handle.sessionData.hash[0] = 0; // default fail
		cout << endl;

		logEye.commentTrace(log_id, "Check input requirement");
        if (args.size() < 6) {
			throw_response(handle, "not enough arguments for reg_user");
			logEye.endTrace(log_id, FAILURE, "not enough arguments for reg_user");
			return;
        }

        if (args[1].size() > 32 or args[2].size() > 32 or args[3].size() > 64) {
			throw_response(handle, "one of the arguments is too long");
			logEye.endTrace(log_id, FAILURE, "one of the arguments is too long");
            return;
        };

        int phone;
        if (isPhone(args[5])) {
            phone = stoi(args[5]);
        }
        else {
			throw_response(handle, "phone number is invalid");
			logEye.endTrace(log_id, FAILURE, "phone number is invalid");
            return;
        };


        const char* newUserLogin = args[1].c_str();
        if (isUserExist_byLogin(newUserLogin)) {
			throw_response(handle, "user with this login already exists");
			logEye.endTrace(log_id, FAILURE, "user with this login already exists");
            return;
        };
        int newUserId = 0;


        User lastUser = loadLastUser();
        if (lastUser.getId() >= 0) {
            newUserId = lastUser.getId() + 1;
        };

		logEye.commentTrace(log_id, "Creating new user with ID: " + to_string(newUserId));
        User newUser(
            newUserId,
			args[1].c_str(), // login
			args[2].c_str(), // password
			args[3].c_str(), // name
			args[4].c_str(), // surname
            phone,
			emptyPassport,
			0,
			USER_NONVERIFED,
            USER
        );
        DB_newUser(newUser);
		logEye.commentTrace(log_id, "New user created successfully: " + string(newUserLogin));
		//cout << "User " << newUserLogin << " registered!" << endl;

		handle.sessionData.hash[0] = 1; // success
		throw_response(handle, "user registered successfully");
		logEye.endTrace(log_id, SUCCESS, "user registered successfully: " + string(newUserLogin));
		return;

    }

    std::string name() const override {
        return "reg_user";
    };
};
