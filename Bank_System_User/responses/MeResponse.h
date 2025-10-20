#pragma once
#include "../Response.h"
#include "../ResponseManager.h"


class MeResponse : public Response {
public:
	bool need_execute() const override { 
		cout << "debug" << endl;
		return false; }
	void get_response(handleInfo& handle) override {
		string cmdText = "user_info " + to_string(handle.sessionData.userId);
		cout << cmdText << endl;
		strncpy(handle.sessionData.cmd, cmdText.c_str(), sizeof(handle.sessionData.cmd) - 1);
		cout << handle.sessionData.cmd << endl;
		response_manager.get_response(handle);
	};
	string name() const override {
		return "me";
	};
};
