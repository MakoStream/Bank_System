#pragma once
#include "../Command.h"
#include "../mainProcess.h"
#include "../basic_functions.h"
#include "../Account.h"
#include "../LogEye.h"


class AccountInfoCommand : public Command {
public:
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