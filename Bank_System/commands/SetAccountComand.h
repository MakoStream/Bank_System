#pragma once
#include "../Account.h"
#include "../Command.h"

class SetAccountCommand : public Command {
public:
	void execute(handleInfo& handle) override { // set_account <PAN> <PIN/CVV> <password>
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		if (args.size() < 4) { // insufficient arguments
			throw_response(handle, "Args issue");
			return;
		}
		if (args[1].size() != 16 or !isStringDigit(args[1])) { // PAN length check
			throw_response(handle, "wrong PAN");
			return;
		}
		account acc = getAccount_byCardNumber(args[1].c_str());

		if (args[2] == "PIN" && args[3].size()==4) {
			acc.setPIN(args[3].c_str());
			throw_response(handle, "PIN updated successfully");
			return;
		}
		else if (args[2] == "CVV" && args[3].size() == 3) {
			acc.setCVV(args[3].c_str());
			throw_response(handle, "CVV updated successfully");
			return;
		}
		else {
			throw_response(handle, "Wrong parameter for PIN/CVV or invalid length");
			return;
		}
		
	}
	string name() const override {
		return "set_account";
	}
};