#pragma once
#include "../Command.h"
#include "../User.h"
#include "../basic_functions.h"

class UserInfoCommand : public Command {
public:
	void execute(handleInfo& handle) override { // user_info <id/login>
		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		handle.sessionData.hash[0] = 0; // fail
		User user;
		if (args.size() < 2) {
			throw_response(handle, "Args issue. Write 'user_info <id/login>'");
			return;
		};
		if (isStringDigit(args[1])) {
			user = getUser_byId(stoi(args[1]));
		};
		if (user.getId()==-1) {
			user = getUser_byLogin(args[1].c_str());
		};
		if (user.getId() == -1) {
			throw_response(handle, "Can't find user");
		};
		handle.sessionData.hash[0] = 1; // succes
		handle.sessionData.hash[1] = user.getId(); // id
		handle.sessionData.hash[2] = user.getPhone(); // phone
		handle.sessionData.hash[3] = user.getTIN(); // TIN
		string text_data = string(user.getLogin()) + " " + user.getName() + " " + user.getSurname();
		strncpy(handle.sessionData.msg[1], text_data.c_str(), sizeof(handle.sessionData.msg[1]) - 1);
		throw_response(handle, "");
		return;
	};
	string name()const override {
		return "user_info";
	};
};