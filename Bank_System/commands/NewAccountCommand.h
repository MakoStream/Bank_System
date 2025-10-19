#pragma once
#include "../Command.h"
#include "../User.h"
#include <iostream>


class NewAccountCommand : public Command {

public:
	void execute(handleInfo& handle) override { // new_account owner curence_type cardtype account_type //example: new_account 0 USD DEPOSITE 316
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		
		handle.sessionData.hash[0] = 0; // default fail
		// ряд перевірик краще винести в окрему функцію
		if (args.size() < 3) {
			throw_response(handle, "Недостатньо аргументів");
			return;
		}
		int userId = stoi(args[1]);
		if (!isStringDigit(args[1])) {
			throw_response(handle, "user_id is incorect");
			return;
		};
		// Перевірка валюти
		if (balanceMap.find(args[2]) == balanceMap.end()) {
			throw_response(handle, "Некоректний тип валюти");
			return;
		}

		// Перевірка типу картки
		if (cardMap.find(args[3]) == cardMap.end()) {
			throw_response(handle, "Некоректний тип картки");
			return;
		}
		if (!isStringDigit(args[4])) {
			throw_response(handle, "account_type is incorect");
			return;
		};
		if (!isUserExist_byId(userId)) {
			throw_response(handle, "Користувача з таким ID не існує");
			return;
		};


		balanceType cur = balanceMap[args[2]];
		cardType cType = cardMap[args[3]];
		short account_type = static_cast<short>(std::stoi(args[4]));
		
		ACC_addAccount(userId, cur, cType, account_type);

		handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
		handle.sessionData.hash[0] = 1;
		throw_response(handle, "Рахунок успішно створено");

		return;
	};

	string name() const override {
		return "new_account";
	};
};