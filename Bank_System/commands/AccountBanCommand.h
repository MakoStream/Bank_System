#pragma once
#include "../Command.h"


class AccountBanCommand : public Command {
public:
	void execute(handleInfo& handle) override { // account_ban <account_number> <reason>
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

		string reason = "No reason specified";
		if (args.size() >= 3) {
			reason = args[2];
		}
		account acc = getAccount_byIBAN(IBAN);
		acc.ban(reason);
		acc.updateInFile();
		strncpy(handle.sessionData.cmd, "Account banned!", sizeof(handle.sessionData.cmd) - 1);
		handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
		WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
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
		acc.unban();
		acc.updateInFile();
		strncpy(handle.sessionData.cmd, "Account unbanned!", sizeof(handle.sessionData.cmd) - 1);
		handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
		WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
		return;

	}
	string name() const override {
		return "account_unban";
	}
};