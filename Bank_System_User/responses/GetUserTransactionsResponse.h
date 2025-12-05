#pragma once
#include "../Response.h"

class GetUserTransactionsResponse : public Response { // get_user_transactions
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		if (handle.sessionData.hash[0] == 0) {
			cout << "Error: " << handle.sessionData.msg[0] << endl;
			return;
		};
		cout << handle.sessionData.msg[0] << endl;
		return;
	}
	string name() const override {
		return "get_user_transactions";
	};
};