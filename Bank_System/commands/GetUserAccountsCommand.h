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
    void execute(handleInfo& handle) override {
		int log_id = logEye.logTrace("getUserAccounts command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", std::string(handle.sessionData.cmd), true);

		handle.sessionData.hash[0] = 0; // fail by default


		logEye.commentTrace(log_id, "Fetching user accounts");
		vector<account> accounts = getUserAccounts(process.getUserSession(handle.sessionData.sessionId).user_id);
		string text_response;
		for (auto a : accounts) {
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