#pragma once
#include "../Command.h"
#include "../LogEye.h"
#include "../Transactions/Transactions.h"

class GetUserTransactionsCommand : public Command {
public:

	void execute(handleInfo& handle) override { // get_user_transactions
		int log_id = logEye.logTrace("get_user_transactions Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);

		handle.sessionData.hash[0] = 0; // fail by default


		logEye.commentTrace(log_id, "Retrieving user transactions");
		int user_id = process.getUserSession(handle.sessionData.sessionId).user_id;
		vector<int> transaction_ids = Transaction::getUserTransactions(user_id);
		logEye.msgTrace(log_id, "Found transaction IDs count", to_string(transaction_ids.size()), true);

		if (transaction_ids.empty()) {
			throw_response(handle, "No transactions found for user.");
			logEye.endTrace(log_id, FAILURE, "No transactions found for user.");
			return;
		}

		logEye.commentTrace(log_id, "Preparing response message with transaction IDs");
		// Prepare response message
		string response_msg;
		for (int id : transaction_ids) {
			response_msg += to_string(id) + " ";
		}
		handle.sessionData.hash[0] = 1; // success

		throw_response(handle, response_msg);
		logEye.endTrace(log_id, SUCCESS, "User transactions retrieved successfully.");
		return;

	}
	string name() const override {
		return "get_user_transactions";
	}
};