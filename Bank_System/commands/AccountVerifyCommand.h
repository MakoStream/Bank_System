#pragma once
#include "../Command.h"


class AccountVerifyCommand : public Command {
public:
	void execute(handleInfo& handle) override { // account_verify <account_number>
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		if (args.size() < 2) {
			strncpy(handle.sessionData.cmd, "Args issue", sizeof(handle.sessionData.cmd) - 1);
			handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
			WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
			return;
		}
		if (args[1].length() != 29) {
			strncpy(handle.sessionData.cmd, "wrong iban", sizeof(handle.sessionData.cmd) - 1);
			handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
			WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
			return;
		}
		char IBAN[36];
		strcpy(IBAN, args[1].c_str());
		if (!isAccountExist_byIBAN(IBAN)) {
			strncpy(handle.sessionData.cmd, "This account is not exist", sizeof(handle.sessionData.cmd) - 1);
			handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
			WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
			return;
		};

		account acc = getAccount_byIBAN(IBAN);
		acc.verify();
		acc.updateInFile();

		strncpy(handle.sessionData.cmd, "Account verifed!", sizeof(handle.sessionData.cmd) - 1);
		handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
		WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
		string account_number = args[1];
		return;
	};

	string name() const override {
		return "account_verify";
	}
};