#pragma once
#include "../Command.h"
#include "../LogEye.h"
//# include "../mainProcess.h"

/**
 * @class AccountBanCommand
 * @brief Handles banning of a bank account.
 * @details The AccountBanCommand class performs:
 * - Validates IBAN and optional reason
 * - Checks if the account exists
 * - Bans the account and updates storage
 * - Sets hash[0] = 1 on success
 * - Logs all actions via logEye
 * @note Requires: <string>, Account, handleInfo, logEye
 * @note Syntax: account_ban <IBAN> [reason]
 */
class AccountBanCommand : public Command {
public:
	/**
 * @brief Executes the account ban command.
 *
 * @details
 * Performs the ban operation for a bank account based on the provided IBAN.
 * The method:
 * - Logs the operation start and session details
 * - Parses the input command into arguments
 * - Validates the number of arguments
 * - Validates the IBAN (must be 29 characters long)
 * - Checks whether the account exists
 * - Extracts an optional ban reason
 * - Bans the account and writes the updated data to file
 * - Sets handle.sessionData.hash[0] to 1 on success (0 on failure)
 * - Sends a response back to the client
 * - Finalizes log records with SUCCESS or FAILURE status
 *
 * @param handle Reference to handleInfo containing session data,
 *               command string, and the hash array for execution status.
 *
 * @throws Throws a response using throw_response() in the following cases:
 *         - Invalid number of arguments
 *         - IBAN format is incorrect (length not equal to 29)
 *         - Account with the provided IBAN does not exist
 *
 * @note
 * Side effects:
 * - Writes log entries via logEye
 * - Updates account data file through acc.updateInFile()
 * - Modifies handle.sessionData.hash[0]
 * - Sends network/session response via throw_response()
 *
 * @retval void This method does not return a value.
 *         Execution status is communicated through handle.sessionData.hash[0]:
 *         - 0 on failure
 *         - 1 on successful account ban
 */
	void execute(handleInfo& handle) override { // account_ban <account_number> <reason>
		int log_id = logEye.logTrace("account_ban Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);



		handle.sessionData.hash[0] = 0; // fail by default
		logEye.commentTrace(log_id, "Check input requirement");
		if (args.size() < 2) {
			throw_response(handle, "Args issue");
			logEye.endTrace(log_id, FAILURE, "Args issue");
			return;
		}
		if (args[1].length() != 29) {
			throw_response(handle, "wrong iban");
			logEye.endTrace(log_id, FAILURE, "wrong iban");
			return;
		}
		char IBAN[36];
		strcpy(IBAN, args[1].c_str());
		if (!account::isAccountExist_byIBAN(IBAN)) {
			throw_response(handle, "This account is not exist");
			logEye.endTrace(log_id, FAILURE, "Account does not exist");
			return;
		};

		string reason = "No reason specified";
		if (args.size() >= 3) {
			reason = args[2];
		}
		account acc = account::getAccount_byIBAN(IBAN);
		acc.ban(reason);
		acc.updateInFile();
		handle.sessionData.hash[0] = 1; // success
		throw_response(handle, "Account banned!");

		string logMessage = "Account with IBAN " + string(IBAN) + " has been banned. Reason: " + reason;
		logEye.endTrace(log_id, SUCCESS, logMessage);
		return;

	}
	string name() const override {
		return "account_ban";
	}
};


/**
 * @class AccountUnbanCommand
 * @brief Handles unbanning of a bank account.
 * @details Validates IBAN, checks if account exists,
 * unbans the account, updates storage,
 * logs all actions via logEye.
 * @note Requires: <string>, Account, handleInfo, logEye
 * @note Syntax: account_unban <IBAN>
 */
class AccountUnbanCommand : public Command {
public:
	/**
 * @brief Executes the account unban command.
 *
 * @details
 * Performs the unban operation for a bank account based on the provided IBAN.
 * The method:
 * - Logs the operation start and session details
 * - Parses the input command into arguments
 * - Validates the number of arguments
 * - Validates the IBAN (must be 29 characters long)
 * - Checks whether the account exists
 * - Unbans the account and writes updated data to file
 * - Sets handle.sessionData.hash[0] to 1 on success (0 on failure)
 * - Sends a response back to the client
 * - Finalizes logging with SUCCESS or FAILURE status
 *
 * @param handle Reference to handleInfo containing session data,
 *               command string, and the hash array for execution status.
 *
 * @throws Throws a response via throw_response() in the following cases:
 *         - Invalid number of arguments
 *         - IBAN format is incorrect (length not equal to 29)
 *         - Account with the provided IBAN does not exist
 *
 * @note
 * Side effects:
 * - Produces log entries via logEye
 * - Updates the account file via acc.updateInFile()
 * - Modifies handle.sessionData.hash[0]
 * - Sends network/session response via throw_response()
 *
 * @retval void Does not return a value directly.
 *         Execution status is indicated through handle.sessionData.hash[0]:
 *         - 0 on failure
 *         - 1 on successful account unban
 */
	void execute(handleInfo& handle) override { // account_unban <account_number>
		int log_id = logEye.logTrace("account_unban Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);

		handle.sessionData.hash[0] = 0; // fail by default
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		logEye.commentTrace(log_id, "Check input requirement");
		if (args.size() < 2) {
			throw_response(handle, "Args issue");
			logEye.endTrace(log_id, FAILURE, "Args issue");
			return;
		}
		if (args[1].length() != 29) {
			throw_response(handle, "wrong iban");
			logEye.endTrace(log_id, FAILURE, "wrong iban");
			return;
		}
		char IBAN[36];
		strcpy(IBAN, args[1].c_str());
		if (!account::isAccountExist_byIBAN(IBAN)) {
			throw_response(handle, "This account is not exist");
			logEye.endTrace(log_id, FAILURE, "Account does not exist");
			return;
		};

		account acc = account::getAccount_byIBAN(IBAN);
		acc.unban();
		acc.updateInFile();
		handle.sessionData.hash[0] = 1; // success
		throw_response(handle, "Account unbanned!");


		string logMessage = "Account with IBAN " + string(IBAN) + " has been unbanned.";
		logEye.endTrace(log_id, SUCCESS, logMessage);
		return;

	}
	string name() const override {
		return "account_unban";
	}
};