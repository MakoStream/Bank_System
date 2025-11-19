#pragma once
#include "../Command.h"
#include "../mainProcess.h"
#include "../basic_functions.h"
#include "../Account.h"
#include "../LogEye.h"

/**
 * @class AccountInfoCommand
 * @brief Retrieves detailed information about an account by card number.
 * @details Validates card number, checks if account exists,
 * retrieves IBAN, balance, currency, and owner information,
 * logs all actions via logEye.
 * @note Requires: <string>, Account, handleInfo, logEye
 * @note Syntax: account_info <card_number>
 */
class AccountInfoCommand : public Command {
public:
	/**
 * @brief Executes the account information retrieval command.
 *
 * @details
 * Retrieves detailed account information based on a provided card number (PAN).
 * The method:
 * - Logs operation start and session details
 * - Parses the input command into arguments
 * - Validates the number of arguments
 * - Validates the card number length (must be 16 digits)
 * - Checks whether an account with the given card number exists
 * - Fetches the account and retrieves:
 *     - PAN
 *     - IBAN
 *     - Balance
 *     - Currency type
 *     - Account owner user ID
 * - Returns the formatted account information to the client
 * - Sets handle.sessionData.hash[0] to 1 on success (0 on failure)
 * - Finalizes logging with SUCCESS or FAILURE status
 *
 * @param handle Reference to handleInfo containing session data,
 *               raw command string, and the hash array for execution status.
 *
 * @throws Throws a response via throw_response() in case of:
 *         - Missing arguments
 *         - Invalid card number length
 *         - Account with the provided PAN not found
 *
 * @note
 * Side effects:
 * - Produces trace logs via logEye
 * - Modifies handle.sessionData.hash[0]
 * - Sends a session/network response through throw_response()
 *
 * @retval void This method does not return a direct value.
 *         Execution result is communicated via handle.sessionData.hash[0]:
 *         - 0 on failure
 *         - 1 on successful data retrieval
 */
	void execute(handleInfo& handle) override {  // accountInfo <account_PAN>
		int log_id = logEye.logTrace("account_info Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);

		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		handle.sessionData.hash[0] = 0;
		logEye.commentTrace(log_id, "Check input requirement");
		if (args.size() < 2) {
			throw_response(handle, "Not enough arguments");
			logEye.endTrace(log_id, FAILURE, "Not enough arguments");
			return;
		}
		if (args[1].length() != 16) {
			throw_response(handle, "Card number is incorect");
			logEye.endTrace(log_id, FAILURE, "Card number is incorect");
			return;
		}
		char cardNumber[17];
		strcpy(cardNumber, args[1].c_str());
		if (!isAccountExist_byCardNumber(cardNumber)) {
			throw_response(handle, "Account with this card number does not exist");
			logEye.endTrace(log_id, FAILURE, "Account with this card number does not exist");
			return;
		};


		logEye.commentTrace(log_id, "Fetching account information for card number: " + string(cardNumber));
		account acc = getAccount_byCardNumber(cardNumber);
		string response = "Account Information:\n";
		response += "PAN: " + string(acc.getPAN()) + "\n";
		response += "IBAN: " + string(acc.getIBAN()) + "\n";
		response += "Balance: " + to_string(acc.getBalance()) + "\n";
		response += "Currency: " + balanceMapToString[acc.getBalanceType()] + "\n";
		response += "Owner User ID: " + to_string(acc.getUserID()) + "\n";

		handle.sessionData.hash[0] = 1;
		throw_response(handle, response);

		logEye.endTrace(log_id, SUCCESS, "Account information retrieved successfully for card number: " + string(cardNumber));
		return;
	};
	string name() const override {
		return "account_info";
	};
};