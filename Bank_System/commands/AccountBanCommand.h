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
		if (!isAccountExist_byIBAN(IBAN)) {
			throw_response(handle, "This account is not exist");
			logEye.endTrace(log_id, FAILURE, "Account does not exist");
			return;
		};

		string reason = "No reason specified";
		if (args.size() >= 3) {
			reason = args[2];
		}
		account acc = getAccount_byIBAN(IBAN);
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
		if (!isAccountExist_byIBAN(IBAN)) {
			throw_response(handle, "This account is not exist");
			logEye.endTrace(log_id, FAILURE, "Account does not exist");
			return;
		};

		account acc = getAccount_byIBAN(IBAN);
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