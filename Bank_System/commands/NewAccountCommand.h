#pragma once
#include "../Command.h"
#include "../User.h"
#include <iostream>
#include "../mainProcess.h"
#include "../LogEye.h"



/**
 * @class NewAccountCommand
 * @brief Creates a new bank account for a user.
 * @details Validates user ID, currency, card type, and account type,
 * adds account to storage, logs all actions via logEye.
 * @note Requires: handleInfo, logEye, Account
 * @note Syntax: new_account <user_id> <currency> <card_type> <account_type>
 */

class NewAccountCommand : public Command {

public:
	void execute(handleInfo& handle) override { // new_account owner curence_type cardtype account_type //example: new_account 0 USD DEPOSITE 316
		int log_id = logEye.logTrace("new_account Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);
		
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		
		handle.sessionData.hash[0] = 0; // default fail
		// ряд перевірик краще винести в окрему функцію
		logEye.commentTrace(log_id, "Check input requirement");
		if (args.size() < 3) {
			throw_response(handle, "Недостатньо аргументів");
			logEye.endTrace(log_id, FAILURE, "Недостатньо аргументів");
			return;
		}
		int userId = stoi(args[1]);
		if (!isStringDigit(args[1])) {
			throw_response(handle, "user_id is incorect");
			logEye.endTrace(log_id, FAILURE, "user_id is incorect");
			return;
		};
		// Перевірка валюти
		if (balanceMap.find(args[2]) == balanceMap.end()) {
			throw_response(handle, "Некоректний тип валюти");
			logEye.endTrace(log_id, FAILURE, "Некоректний тип валюти");
			return;
		}

		// Перевірка типу картки
		if (cardMap.find(args[3]) == cardMap.end()) {
			throw_response(handle, "Некоректний тип картки");
			logEye.endTrace(log_id, FAILURE, "Некоректний тип картки");
			return;
		}
		if (!isStringDigit(args[4])) {
			throw_response(handle, "account_type is incorect");
			logEye.endTrace(log_id, FAILURE, "account_type is incorect");
			return;
		};
		if (!isUserExist_byId(userId)) {
			throw_response(handle, "Користувача з таким ID не існує");
			logEye.endTrace(log_id, FAILURE, "Користувача з таким ID не існує");
			return;
		};


		// Всі перевірки пройшли успішно, створюємо рахунок
		logEye.commentTrace(log_id, "All checks passed, creating account");
		balanceType cur = balanceMap[args[2]];
		cardType cType = cardMap[args[3]];
		short account_type = static_cast<short>(std::stoi(args[4]));
		
		logEye.commentTrace(log_id, "Adding account for user ID: " + to_string(userId));
		ACC_addAccount(userId, cur, cType, account_type);

		handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
		handle.sessionData.hash[0] = 1;
		throw_response(handle, "Рахунок успішно створено");

		logEye.endTrace(log_id, SUCCESS, "Account created successfully for user ID: " + to_string(userId));
		return;
	};

	string name() const override {
		return "new_account";
	};
};