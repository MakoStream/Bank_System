#include "../Command.h"
#include "../mainProcess.h"

class UnitTestCommand : public Command {
public:
	void execute(handleInfo& handle) override {  // unit_test <client_type>
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		string client_type = "user";

		handle.sessionData.hash[0] = 0; // fail by default
		if (args.size() >= 2) {
			client_type = args[1];
		};

		if (process.debugStatus()) {
			if (client_type == "user") {
				handle.sessionData.hash[0] = 1; // allow to run UnitTest
				throw_response(handle, "");
				return;
			}
			else if (client_type == "server") {
				//here i must run Server Client unit test
				return;
			};
		}
		else {
			throw_response(handle, "You can't run UnitTest without debug mode. Run this first ('debug' command)");
			return;
		};
		
	};
	string name()const override {
		return "unit_test";
	};

};