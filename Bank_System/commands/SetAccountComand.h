#pragma once
#include "../Account.h"
#include "../Command.h"
//#include "../basic_functions.h"
#include "../mainProcess.h"
#include "../LogEye.h"

/**
 * @class SetAccountCommand
 * @brief Updates account PIN or CVV.
 * @details Validates PAN and input type, updates PIN or CVV in account,
 * logs all actions via logEye.
 * @note Requires: handleInfo, logEye, Account, <vector>, <string>
 * @note Syntax: set_account <PAN> <PIN/CVV> <value>
 */

class SetAccountCommand : public Command {
public:
	void execute(handleInfo& handle) override { // set_account <PAN> <PIN/CVV> <password>
		int log_id = logEye.logTrace("set_account Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);


		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		logEye.commentTrace(log_id, "Check input requirements");
		if (args.size() < 4) { // insufficient arguments
			throw_response(handle, "Args issue");
			logEye.endTrace(log_id, FAILURE, "Args issue");
			return;
		}
		if (args[1].size() != 16 or !isStringDigit(args[1])) { // PAN length check
			throw_response(handle, "wrong PAN");
			logEye.endTrace(log_id, FAILURE, "wrong PAN");
			return;
		}

		//logEye.commentTrace(log_id, "Check if account exists");
		account acc = getAccount_byCardNumber(args[1].c_str());

		if (args[2] == "PIN" && args[3].size()==4) {
			acc.setPIN(args[3].c_str());
			throw_response(handle, "PIN updated successfully");
			logEye.endTrace(log_id, SUCCESS, "PIN updated successfully for PAN: " + string(args[1]));
			return;
		}
		else if (args[2] == "CVV" && args[3].size() == 3) {
			acc.setCVV(args[3].c_str());
			throw_response(handle, "CVV updated successfully");
			logEye.endTrace(log_id, SUCCESS, "CVV updated successfully for PAN: " + string(args[1]));
			return;
		}
		else {
			throw_response(handle, "Wrong parameter for PIN/CVV or invalid length");
			logEye.endTrace(log_id, FAILURE, "Wrong parameter for PIN/CVV or invalid length for PAN: " + string(args[1]));
			return;
		}
		
	}
	string name() const override {
		return "set_account";
	}
};