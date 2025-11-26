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
	/**
 * @brief Executes the command to update account PIN or CVV.
 *
 * @details
 * Validates the card PAN and input parameters, then updates the account information. The method:
 * - Logs the start of execution, session ID, user ID, and input command
 * - Parses the input arguments: PAN, type (PIN/CVV), and value
 * - Validates the PAN length (16 digits) and ensures it is numeric
 * - Retrieves the account using getAccount_byCardNumber()
 * - Updates the PIN if type is "PIN" and value length is 4
 * - Updates the CVV if type is "CVV" and value length is 3
 * - Sends a success or failure response to the client
 * - Completes logging with SUCCESS or FAILURE status
 *
 * @param handle Reference to handleInfo containing session data,
 *               command string, and hash/message arrays.
 *
 * @throws Throws a response using throw_response() in case of:
 *         - Missing arguments
 *         - Invalid PAN
 *         - Invalid type or value length for PIN/CVV
 *
 * @note
 * Side effects:
 * - Modifies the account data (PIN or CVV)
 * - Produces trace logs via logEye
 * - Sends session/network response using throw_response()
 *
 * @retval void This method does not return a direct value.
 *         Execution result is communicated through the response message.
 */

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
		account acc = account::getAccount_byCardNumber(args[1].c_str());

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