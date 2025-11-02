#pragma once
#include "../Command.h"
#include "../mainProcess.h"
#include "../basic_functions.h"
#include "../Account.h"


class AccountInfoCommand : public Command {
public:
	void execute(handleInfo& handle) override {  // accountInfo <account_PAN>
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);
		handle.sessionData.hash[0] = 0;
		if (args.size() < 2) {
			throw_response(handle, "Not enough arguments");
			return;
		}
		if (args[1].length() != 16) {
			throw_response(handle, "Card number is incorect");
			return;
		}
		char cardNumber[17];
		strcpy(cardNumber, args[1].c_str());
		if (!isAccountExist_byCardNumber(cardNumber)) {
			throw_response(handle, "Account with this card number does not exist");
			return;
		};
		account acc = getAccount_byCardNumber(cardNumber);
		string response = "Account Information:\n";
		response += "PAN: " + string(acc.getPAN()) + "\n";
		response += "IBAN: " + string(acc.getIBAN()) + "\n";
		response += "Balance: " + to_string(acc.getBalance()) + "\n";
		response += "Currency: " + balanceMapToString[acc.getBalanceType()] + "\n";
		response += "Owner User ID: " + to_string(acc.getUserID()) + "\n";

		handle.sessionData.hash[0] = 1;
		throw_response(handle, response);
	}; // тут помилка
	string name() const override {
		return "account_info";
	};
};