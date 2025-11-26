#pragma once
#include "../Command.h"
#include "../Account.h"
#include "../Audit/Audit.h"
#include "../mainProcess.h"
#include "../LogEye.h"

using namespace std;


/**
 * @class TransactionRequestPANCommand
 * @brief Processes a transaction request between two accounts.
 * @details Validates PANs, CVV, PIN, and amount,
 * executes transaction, logs all actions via logEye.
 * @note Requires: handleInfo, logEye, Account, <vector>, <string>
 * @note Syntax: request_PAN <PAN_from> <PAN_to> <amount> <CVV> <PIN> <comment>
 */
class TransactionRequestPANCommand : public Command {
public:
	/**
 * @brief Executes a transaction request between two accounts using PANs.
 *
 * @details
 * Validates input parameters and executes a transaction request. The method:
 * - Logs the start of execution, session ID, user ID, and input command
 * - Parses input arguments: PAN_from, PAN_to, amount, CVV, PIN, and optional comment
 * - Validates PAN lengths (16 digits), CVV (3 digits), PIN (4 digits), and amount format
 * - Checks that both source and destination accounts exist
 * - Calls process.transaction_request() to execute the transaction
 * - Sets handle.sessionData.hash[0] to 1 on success
 * - Sends a response to the client
 * - Completes logging with SUCCESS or FAILURE status
 *
 * @param handle Reference to handleInfo containing session data,
 *               command string, and hash/message arrays.
 *
 * @throws Throws a response via throw_response() in case of:
 *         - Missing arguments
 *         - Invalid PAN, CVV, PIN, or amount
 *         - Non-existent accounts
 *
 * @note
 * Side effects:
 * - Initiates a transaction request via process.transaction_request()
 * - Produces trace logs via logEye
 * - Sends session/network response using throw_response()
 *
 * @retval void Execution result is communicated through handle.sessionData.hash[0]
 *         and the response message.
 */
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
		account acc_from = account::getAccount_byCardNumber(args[1].c_str());
		account acc_to = account::getAccount_byCardNumber(args[2].c_str());

		if (!account::isAccountExist_byCardNumber(args[2].c_str())) {
			throw_response(handle, "Account_to not exist");
			logEye.endTrace(log_id, FAILURE, "Account_to not exist");
			return;
		}
		if (!account::isAccountExist_byCardNumber(args[1].c_str())) {
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


/**
 * @class TransactionRequestListCommand
 * @brief Prints a paginated list of transaction requests.
 * @details Validates page number, prints requests page by page,
 * logs all actions via logEye.
 * @note Requires: handleInfo, logEye, <vector>, <string>
 * @note Syntax: request_list <page>
 */
class TransactionRequestListCommand : public Command {
public:
	/**
 * @brief Executes the command to print a paginated list of transaction requests.
 *
 * @details
 * Fetches and prints transaction requests page by page. The method:
 * - Logs the start of execution, session ID, user ID, and input command
 * - Parses the input command and validates the page number
 * - Defaults to page 1 if no page number is provided
 * - Calls printTransactions() to fetch and format transaction requests for the requested page
 * - Writes the formatted transaction list to the client via WriteFile()
 * - Completes logging with SUCCESS or FAILURE status
 *
 * @param handle Reference to handleInfo containing session data,
 *               command string, message buffers, and pipe information.
 *
 * @throws Throws a response via throw_response() if the page number is invalid (non-numeric).
 *
 * @note
 * Side effects:
 * - Produces trace logs via logEye
 * - Sends transaction request list data to the client through WriteFile()
 * - May modify handle.sessionData.msg to hold the transaction list
 *
 * @retval void This method does not return a direct value.
 *         Output is delivered via handle.sessionData.msg and the pipe.
 */
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