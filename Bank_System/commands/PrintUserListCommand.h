#pragma once
#include "../Command.h"
#include "../User.h"

#include <iostream>
#include "../mainProcess.h"
#include "../LogEye.h"

/**
 * @class PrintUserListCommand
 * @brief Prints a paginated list of all users.
 * @details Validates page number, prints users page by page,
 * logs all actions via logEye.
 * @note Requires: handleInfo, logEye
 * @note Syntax: user_list <page>
 */
class PrintUserListCommand : public Command {
    Logger* logger;
public:
    void execute(handleInfo& handle) override {
		int log_id = logEye.logTrace("user_list Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);


        string input(handle.sessionData.cmd);
        vector<string> args = split(input);
        int page;
        if (args.size() < 2) { page = 1; }
        else {
            if (!isStringDigit(args[1])) {
				throw_response(handle, "Page number is incorect");
				logEye.endTrace(log_id, FAILURE, "Page number is incorect");
                return;
            };
            page = stoi(args[1]);
        };
		logEye.commentTrace(log_id, "Printing user list, page: " + to_string(page));
        DB_list(handle.sessionData.msg, page);
        //strncpy(handle.sessionData.cmd, "User list is printed!", sizeof(handle.sessionData.cmd) - 1);
        //handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
        WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
		logEye.endTrace(log_id, SUCCESS, "User list printed successfully");
        return;
    }

    std::string name() const override {
        return "user_list";
    }
};
