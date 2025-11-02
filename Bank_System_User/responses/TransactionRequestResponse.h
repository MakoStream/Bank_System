#pragma once
#include "../Response.h"

class TransactionRequestPANResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		// Implementation of response handling for TransactionRequestPAN
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		cout << "Transaction Request Response: " << (handle.sessionData.hash[0] ? "Success" : "Failure") << "\n";
		cout << "Message: " << handle.sessionData.msg[0] << "\n";
	};
	std::string name() const override {
		return "request_PAN";
	}
};

class TransactionRequestListResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		// Implementation of response handling for TransactionRequestList
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		cout << "Transaction Request List Response:\n";
		for (int i = 0; i < 10; ++i) {
			if (strlen(handle.sessionData.msg[i]) > 0) {
				cout << handle.sessionData.msg[i] << "\n";
			}
		}
	};
	std::string name() const override {
		return "request_list";
	}
};