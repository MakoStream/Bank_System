#pragma once
#include "../Command.h"


class AccountVerifyCommand : public Command {
public:
	void execute(handleInfo& handle) override { // account_verify <account_number>
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		if (args.size() < 2) {
			throw_response(handle, "Not enough arguments");
			return;
		}
		if (args[1].length() != 29) {
			throw_response(handle, "IBAN is incorect");
			return;
		}
		char IBAN[36];
		strcpy(IBAN, args[1].c_str());
		if (!isAccountExist_byIBAN(IBAN)) {
			throw_response(handle, "Account with this IBAN does not exist");
			return;
		};

		account acc = getAccount_byIBAN(IBAN);
		acc.verify();
		acc.updateInFile();

		throw_response(handle, "Account verified successfully");
		string account_number = args[1];
		return;
	};

	string name() const override {
		return "account_verify";
	}
};