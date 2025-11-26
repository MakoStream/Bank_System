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
    /**
 * @brief Executes the command to print a paginated list of all users.
 *
 * @details
 * Fetches and prints a list of users, page by page. The method:
 * - Logs the start of execution, session ID, user ID, and input command
 * - Parses the input command and validates the page number
 * - Defaults to page 1 if no page number is provided
 * - Checks that the page number is numeric
 * - Calls DB_list() to fetch and format user data for the requested page
 * - Writes the formatted user list to the client via WriteFile()
 * - Completes logging with SUCCESS or FAILURE status
 *
 * @param handle Reference to handleInfo containing session data,
 *               command string, message buffers, and pipe information.
 *
 * @throws Throws a response via throw_response() if the page number is invalid (non-numeric).
 *
 * @note
 * Side effects:
 * - Produces trace logs via logEye
 * - Sends user list data to the client through WriteFile()
 * - May modify handle.sessionData.msg to hold the user list
 *
 * @retval void This method does not return a direct value.
 *         Output is delivered via handle.sessionData.msg and the pipe.
 */
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
        User::DB_list(handle.sessionData.msg, page);
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
