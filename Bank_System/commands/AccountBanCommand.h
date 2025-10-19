#pragma once
#include "../Command.h"


class AccountBanCommand : public Command {
public:
	void execute(handleInfo& handle) override { // account_ban <account_number> <reason>
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		handle.sessionData.hash[0] = 0; // fail by default
		if (args.size() < 2) {
			throw_response(handle, "Args issue");
			return;
		}
		if (args[1].length() != 29) {
			throw_response(handle, "wrong iban");
			return;
		}
		char IBAN[36];
		strcpy(IBAN, args[1].c_str());
		if (!isAccountExist_byIBAN(IBAN)) {
			throw_response(handle, "This account is not exist");
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
		return;

	}
	string name() const override {
		return "account_ban";
	}
};

class AccountUnbanCommand : public Command {
public:
	void execute(handleInfo& handle) override { // account_unban <account_number>
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		if (args.size() < 2) {
			throw_response(handle, "Args issue");
			return;
		}
		if (args[1].length() != 29) {
			throw_response(handle, "wrong iban");
			return;
		}
		char IBAN[36];
		strcpy(IBAN, args[1].c_str());
		if (!isAccountExist_byIBAN(IBAN)) {
			throw_response(handle, "This account is not exist");
			return;
		};

		account acc = getAccount_byIBAN(IBAN);
		acc.unban();
		acc.updateInFile();
		handle.sessionData.hash[0] = 1; // success
		throw_response(handle, "Account unbanned!");
		return;

	}
	string name() const override {
		return "account_unban";
	}
};