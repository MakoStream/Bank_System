#pragma once
#include "../Command.h"
#include "../User.h"
#include "../basic_functions.h"
#include "../mainProcess.h"
#include "../LogEye.h"

class UserInfoCommand : public Command {
public:
	void execute(handleInfo& handle) override { // user_info <id/login>
		int log_id = logEye.logTrace("user_info Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);


		string input(handle.sessionData.cmd);
		vector<string> args = split(input);

		handle.sessionData.hash[0] = 0; // fail
		User user;
		logEye.commentTrace(log_id, "Check input requirement");
		if (args.size() < 2) {
			throw_response(handle, "Args issue. Write 'user_info <id/login>'");
			logEye.endTrace(log_id, FAILURE, "Args issue. Write 'user_info <id/login>'");
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
			logEye.endTrace(log_id, FAILURE, "Can't find user");
		};

		logEye.commentTrace(log_id, "Prepare response data");
		handle.sessionData.hash[0] = 1; // succes
		handle.sessionData.hash[1] = user.getId(); // id
		handle.sessionData.hash[2] = user.getPhone(); // phone
		handle.sessionData.hash[3] = user.getTIN(); // TIN
		string text_data = string(user.getLogin()) + " " + user.getName() + " " + user.getSurname();
		strncpy(handle.sessionData.msg[1], text_data.c_str(), sizeof(handle.sessionData.msg[1]) - 1);
		throw_response(handle, "");
		logEye.endTrace(log_id, SUCCESS, "User info retrieved successfully");
		return;
	};
	string name()const override {
		return "user_info";
	};
};