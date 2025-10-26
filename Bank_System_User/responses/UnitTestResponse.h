#pragma once
#include "../Response.h"
#include "../UnitTest/UserClientTest.h"


class UnitTestResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		cout << "debug 2" << endl;
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		std::string input(handle.sessionData.cmd);
		std::vector<std::string> args = split(input);

		std::string client_type = "user";
		if (args.size() >= 2) {
			client_type = args[1];
		};
		if (handle.sessionData.hash[0] == 0) {
			cout << "ERROR: " << handle.sessionData.msg[0] << endl;
			return;
		}
		else if (handle.sessionData.hash[0] == 1 && client_type == "user") {
			//run UnitTest(user) here
			cout << "debug 1" << endl;
			runUnitTest(handle);
			return;
		};
		cout << "debug 2" << endl;
	};
	string name() const override {
		return "unit_test";
	};

};