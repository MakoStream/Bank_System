#pragma once
#include "../Response.h"

class SetAccountResponse : public Response {
public:
	void get_response(handleInfo& handle) {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		cout << handle.sessionData.msg[0] << endl;
		return;
	}
	string name() const override {
		return "set_account";
	}
};