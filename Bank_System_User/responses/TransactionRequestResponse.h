#pragma once
#include <algorithm>
#include "../Response.h"
#include "../mainWindow.h"

class TransactionRequestPANResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		// Implementation of response handling for TransactionRequestPAN
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		cout << "Transaction Request Response: " << (handle.sessionData.hash[0] ? "Success" : "Failure") << "\n";
		cout << "Message: " << handle.sessionData.msg[0] << "\n";
		w.setMessage(handle.sessionData.msg[0]);
		w.hideTransactionWindow();
	};
	std::string name() const override {
		return "request_PAN";
	}
};

class TransactionRequestIBANResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		// Implementation of response handling for TransactionRequestPAN
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		cout << "Transaction Request Response: " << (handle.sessionData.hash[0] ? "Success" : "Failure") << "\n";
		cout << "Message: " << handle.sessionData.msg[0] << "\n";
		w.setMessage(handle.sessionData.msg[0]);
		w.hideTransactionWindow();
	};
	std::string name() const override {
		return "request_IBAN";
	}
};

class TransactionRequestListResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		// Implementation of response handling for TransactionRequestList
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
	};
	std::string name() const override {
		return "request_list";
	}
};

class TransactionInfoResponse : public Response {
public:
	void get_response(handleInfo& handle) override {   //transaction_info <id>
		// Implementation of response handling for TransactionRequestList
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);

		vector<string> args = split(handle.sessionData.msg[0]);
		// return format:
		// ID from_account_PAN to_account_PAN from_account_id to_account_id amount status timestamp comment
		if (handle.sessionData.hash[0] == 1) {
			for (string a : args) {
				cout << a << " ";
			};
			try {
				int fromId = stoi(args[1]);
				int toId = stoi(args[2]);

				float amount = stof(args[5]);

				bool isFromUser = std::find(currentUser.accounts_id.begin(), currentUser.accounts_id.end(), fromId) != currentUser.accounts_id.end();
				bool isToUser = std::find(currentUser.accounts_id.begin(), currentUser.accounts_id.end(), toId) != currentUser.accounts_id.end();

				cout << "debug 2: " << args[0] << endl;
				if (isFromUser && isToUser) {
					w.AddTransactionBoxItem(args[4], args[3], -amount, "UAH");
					w.AddTransactionBoxItem(args[3], args[4], amount, "UAH");
				}
				else if (isFromUser) {
					w.AddTransactionBoxItem(args[3], args[4], -amount, "UAH");
				}
				else if (isToUser) {
					w.AddTransactionBoxItem(args[3], args[4], amount, "UAH");
				}
			}
			catch (std::exception& e) {
				cout << "Transaction parsing error: " << e.what();
			}
		}
		else {
			cout << "Error: " << handle.sessionData.msg[0] << endl;
		};

	};
	std::string name() const override {
		return "transaction_info";
	}
};