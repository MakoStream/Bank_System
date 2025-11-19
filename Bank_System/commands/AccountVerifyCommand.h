#pragma once
#include "../Command.h"
#include "../LogEye.h"


/**
 * @class AccountVerifyCommand
 * @brief Verifies a bank account.
 * @details Validates IBAN, checks if account exists,
 * performs verification, updates storage,
 * logs all actions via logEye.
 * @note Requires: <string>, Account, handleInfo, logEye
 * @note Syntax: account_verify <IBAN>
 */

class AccountVerifyCommand : public Command {
public:
	/**
 * @brief Executes the account verification command.
 *
 * @details
 * Verifies a bank account using the provided IBAN.
 * The method:
 * - Logs the start of the operation and session details
 * - Parses the input command into arguments
 * - Validates the presence of required arguments
 * - Validates the IBAN format (must be 29 characters long)
 * - Checks whether an account with the specified IBAN exists
 * - Retrieves the account data
 * - Performs account verification (acc.verify())
 * - Updates the account information in storage (acc.updateInFile())
 * - Sets handle.sessionData.hash[0] to 1 on success
 * - Sends a response back to the client
 * - Completes the trace log with SUCCESS or FAILURE status
 *
 * @param handle Reference to handleInfo containing session data,
 *               the command string, and the hash array for reporting status.
 *
 * @throws Throws a response using throw_response() when:
 *         - Required arguments are missing
 *         - IBAN format is invalid
 *         - No account exists with the provided IBAN
 *
 * @note
 * Side effects:
 * - Produces trace logs through logEye
 * - Modifies handle.sessionData.hash[0]
 * - Updates the account record on disk
 * - Sends a network/session response via throw_response()
 *
 * @retval void Does not return a value directly.
 *         Execution success or failure is communicated through handle.sessionData.hash[0]:
 *         - 0 on failure
 *         - 1 on successful verification
 */
	void execute(handleInfo& handle) override { // account_verify <account_number>
		int log_id = logEye.logTrace("account_verify Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);

		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		logEye.commentTrace(log_id, "Check input requirement");
		if (args.size() < 2) {
			throw_response(handle, "Not enough arguments");
			logEye.endTrace(log_id, FAILURE, "Not enough arguments");
			return;
		}
		if (args[1].length() != 29) {
			throw_response(handle, "IBAN is incorect");
			logEye.endTrace(log_id, FAILURE, "IBAN is incorect");
			return;
		}
		char IBAN[36];
		strcpy(IBAN, args[1].c_str());
		if (!isAccountExist_byIBAN(IBAN)) {
			throw_response(handle, "Account with this IBAN does not exist");
			logEye.endTrace(log_id, FAILURE, "Account with this IBAN does not exist");
			return;
		};

		logEye.commentTrace(log_id, "Verifying account with IBAN: " + string(IBAN));
		account acc = getAccount_byIBAN(IBAN);
		acc.verify();
		acc.updateInFile();
		handle.sessionData.hash[0] = 1; // success
		

		throw_response(handle, "Account verified successfully");
		//string account_number = args[1];
		logEye.endTrace(log_id, SUCCESS, "Account verified successfully for IBAN: " + string(IBAN));
		return;
	};

	string name() const override {
		return "account_verify";
	}
};