#pragma once
#include "../Command.h"
#include "../User.h"
#include <iostream>


class NewAccountCommand : public Command {

public:
	void execute(handleInfo& handle) override { // new_account owner curence_type cardtype account_type //example: new_account 0 USD DEPOSITE 316
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		int userId = stoi(args[1]);

		// ряд перевірик краще винести в окрему функцію
		if (args.size() < 3) {
			cout << "Недостатньо аргументів для new_account!" << endl;
			return;
		}
		if (!isStringDigit(args[1])) {
			cout << "userId is incorect" << endl;
			return;
		};
		// Перевірка валюти
		if (balanceMap.find(args[2]) == balanceMap.end()) {
			cout << "Некоректний тип валюти: " << args[2] << endl;
			return;
		}

		// Перевірка типу картки
		if (cardMap.find(args[3]) == cardMap.end()) {
			cout << "Некоректний тип картки: " << args[3] << endl;
			return;
		}
		if (!isStringDigit(args[4])) {
			cout << "account_type is incorect" << endl;
			return;
		};
		if (!isUserExist_byId(userId)) {
			cout << userId << " not exist" << endl;
			return;
		};


		balanceType cur = balanceMap[args[2]];
		cardType cType = cardMap[args[3]];
		short account_type = static_cast<short>(std::stoi(args[4]));
		
		ACC_addAccount(userId, cur, cType, account_type);

		strncpy(handle.sessionData.cmd, "Account registered!", sizeof(handle.sessionData.cmd) - 1);
		handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
		WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);

		return;
	};

	string name() const override {
		return "new_account";
	};
};