#pragma once
#include "../Command.h"
#include "../User.h"
#include <iostream>
#include "../mainProcess.h"
#include "../LogEye.h"

/** @class userLoginCommand
 *  @brief Command class for user login.
 *  @details Handles:
 *   - Prompting for user credentials (username/password)
 *   - Verifying credentials against User database
 *   - Creating a session upon successful login
 *  @note Requires: <string>, <iostream>, User, Session, Logger
 *  @note Syntax: login <login> <password>
 *  @used For:
 *   - Initiating user sessions
 */
class userLoginCommand : public Command {

public:
    /**
 * @class userLoginCommand
 * @brief Handles user login into the system.
 *
 * @details
 * The userLoginCommand performs the following steps:
 * - Logs session ID and input data via logEye.
 * - Parses input command to extract login and password.
 * - Checks if the provided user exists in the system using getUser_byLogin().
 * - Verifies the password using User::checkPassword().
 * - If successful:
 *   - Creates a session using process.login().
 *   - Generates an authentication key for the session.
 *   - Returns user information (login, name, surname) via handle.sessionData.msg.
 *   - Updates handle.sessionData.hash[0] to 1 indicating success.
 * - If login fails:
 *   - Sends appropriate error messages via throw_response().
 *   - Logs the failure using logEye.
 *
 * @param handle Reference to handleInfo containing:
 *   - sessionData (session ID, input command, hash/message arrays)
 *   - Pipe information for communication
 * @note Syntax: login <login> <password>
 * @note Side Effects:
 *   - Logs all actions using logEye
 *   - Updates session state and generates authentication keys
 *   - Writes response to sessionData over the pipe
 * @retval void Execution result indicated by handle.sessionData.hash[0]
 *         (1 for success, 0 for failure)
 */
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
        User user = User::getUser_byLogin(login);
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
			handle.sessionData.hash[1] = user.getId();
            handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
            handle.sessionData.userId = user.getId();
            WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);

			logEye.endTrace(log_id, SUCCESS, "User logged in successfully: " + string(login));

            process.printSessions();

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
