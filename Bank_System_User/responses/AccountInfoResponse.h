#pragma once
#include "../Response.h"

class AccountInfoResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		if (handle.sessionData.hash[0] == 0){
			cout << "Error: " << handle.sessionData.msg[0] << endl;
			return;
		};
		cout << handle.sessionData.msg[0] << endl;
		return;
	}
	string name() const override {
		return "account_info";
	};
};