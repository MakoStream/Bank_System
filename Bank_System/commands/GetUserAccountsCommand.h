#pragma once

#include "../Command.h"
#include "../User.h"
#include <iostream>
#include "../mainProcess.h"
#include "../LogEye.h"
#include <vector>
#include "../account.h"

using namespace std;

/**
 * @class getUserAccountsCommand
 * @brief Retrieves all accounts for a specific user.
 * @details Fetches accounts based on user ID, returns account IDs,
 * logs all actions via logEye.
 * @note Requires: handleInfo, logEye, Account
 * @note Syntax: getUserAccounts
 */
class getUserAccountsCommand : public Command {

public:
	/**
 * @brief Executes the command to retrieve all accounts for a user.
 *
 * @details
 * Fetches all accounts associated with the current user's session.
 * The method:
 * - Logs the start of execution, session ID, user ID, and input command
 * - Retrieves user ID from the session
 * - Calls getUserAccounts() to fetch all accounts for the user
 * - Prepares a response string containing all account IDs
 * - Sets handle.sessionData.hash[0] to 1 on success (0 on failure)
 * - Sends a success response to the client
 * - Completes logging with SUCCESS status
 *
 * @param handle Reference to handleInfo containing session data,
 *               including user session, command input, and hash/message arrays.
 *
 * @note
 * Side effects:
 * - Modifies handle.sessionData.hash[0]
 * - Copies account IDs into handle.sessionData.msg[1]
 * - Produces trace logs via logEye
 * - Sends a session/network response using throw_response()
 *
 * @retval void This method does not return a value directly.
 *         Execution result is communicated through handle.sessionData.hash[0]
 *         and the response message.
 */
    void execute(handleInfo& handle) override {
		int log_id = logEye.logTrace("getUserAccounts command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", std::string(handle.sessionData.cmd), true);

		handle.sessionData.hash[0] = 0; // fail by default

		cout << "=====" << endl;


		logEye.commentTrace(log_id, "Fetching user accounts");
		vector<account> accounts = account::getUserAccounts(process.getUserSession(handle.sessionData.sessionId).user_id);
		string text_response;
		for (auto a : accounts) {
			cout << a.getPAN() << endl;
			text_response += to_string(a.getId()) + " ";
		};
		logEye.msgTrace(log_id, "Accounts found", to_string(accounts.size()), true);
		handle.sessionData.hash[0] = 1; // success

		strncpy(handle.sessionData.msg[1], text_response.c_str(), sizeof(handle.sessionData.msg[1]));
		throw_response(handle, "Success");
		logEye.endTrace(log_id, SUCCESS, "User accounts retrieved successfully");


		return;
    }

    std::string name() const override {
        return "getUserAccounts";
    }

};