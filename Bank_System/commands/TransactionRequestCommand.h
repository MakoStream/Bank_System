#pragma once
#include "../Command.h"
#include "../Account.h"
#include "../Audit/Audit.h"
#include "../mainProcess.h"
#include "../LogEye.h"

using namespace std;

class TransactionRequestPANCommand : public Command {
public:
	void execute(handleInfo& handle) override { // transaction_request <PAN_from> <PAN_to> <ammount> <CVV> <PIN> <comment>
		int log_id = logEye.logTrace("transaction_request_PAN Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);

		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		handle.sessionData.hash[0] = 0; // fail by default

		logEye.commentTrace(log_id, "Check input requirement");
		if (args.size() < 6) {
			throw_response(handle, "Args issue");
			logEye.endTrace(log_id, FAILURE, "Args issue");
			return;
		}
		if (args[1].size() != 16 || !isStringDigit(args[1])) { // PAN length check
			throw_response(handle, "wrong PAN_from");
			logEye.endTrace(log_id, FAILURE, "wrong PAN_from");
			return;
		}
		if (args[2].size() != 16 || !isStringDigit(args[2])) { // PAN length check
			throw_response(handle, "wrong PAN_to");
			logEye.endTrace(log_id, FAILURE, "wrong PAN_to");
			return;
		}
		if (args[4].size() != 3 or !isStringDigit(args[4])) {
			throw_response(handle, "wrong CVV");
			logEye.endTrace(log_id, FAILURE, "wrong CVV");
			return;
		};
		if (args[5].size() != 4 or !isStringDigit(args[5])) {
			throw_response(handle, "wrong PIN");
			logEye.endTrace(log_id, FAILURE, "wrong PIN");
			return;
		};
		if (!isStringDigit(args[3])) {
			throw_response(handle, "wrong ammount");
			logEye.endTrace(log_id, FAILURE, "wrong ammount");
			return;
		}

		logEye.commentTrace(log_id, "Check if accounts exist and process transaction request");
		double ammount = stod(args[3]);
		account acc_from = getAccount_byCardNumber(args[1].c_str());
		account acc_to = getAccount_byCardNumber(args[2].c_str());

		if (!isAccountExist_byCardNumber(args[2].c_str())) {
			throw_response(handle, "Account_to not exist");
			logEye.endTrace(log_id, FAILURE, "Account_to not exist");
			return;
		}
		if (!isAccountExist_byCardNumber(args[1].c_str())) {
			throw_response(handle, "Account_from not exist");
			logEye.endTrace(log_id, FAILURE, "Account_from not exist");
			return;
		}

		handle.sessionData.hash[0] = 1; // success
		
		process.transaction_request(handle, acc_from, acc_to, ammount, args[4].c_str(), args[5].c_str(), TRANSACTION, args.size() >= 7 ? args[6] : ""); // issue here
		throw_response(handle, "Transaction request processed");
		logEye.endTrace(log_id, SUCCESS, "Transaction request processed from PAN: " + string(args[1]) + " to PAN: " + string(args[2]));
		return;
	}
	std::string name() const override {
		return "request_PAN";
	}
};

class TransactionRequestListCommand : public Command {
public:
	void execute(handleInfo& handle) override { // transaction_request_list <page>
		int log_id = logEye.logTrace("transaction_request_list Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);


		string input(handle.sessionData.cmd);
		vector<string> args = split(input);
		int page;
		if (args.size() < 2) { page = 1; }
		else {
			if (!isStringDigit(args[1])) {
				throw_response(handle, "Page number is incorect");
				logEye.endTrace(log_id, FAILURE, "Page number is incorect");
				return;
			};
			page = stoi(args[1]);
		};

		printTransactions(handle.sessionData.msg, page);
		logEye.commentTrace(log_id, "Printing transaction request list, page: " + to_string(page));
		//strncpy(handle.sessionData.cmd, "Account list is printed!", sizeof(handle.sessionData.cmd) - 1);
		//handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
		WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
		logEye.endTrace(log_id, SUCCESS, "Transaction request list printed successfully");
		return;
	}
	std::string name() const override {
		return "request_list";
	}
};