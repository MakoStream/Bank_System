#include "../Command.h"
#include "../mainProcess.h"
#include <filesystem>



/**
 * @class UnitTestCommand
 * @brief Executes unit tests for user or server clients.
 * @details Checks debug mode, deletes old databases if needed,
 * runs tests, logs all actions via logEye.
 * @note Requires: handleInfo, logEye, <filesystem>
 * @note Syntax: unit_test <client_type>
 */

class UnitTestCommand : public Command {
public:
	/**
 * @class UnitTestCommand
 * @brief Executes unit tests for user or server clients.
 *
 * @details
 * The UnitTestCommand performs the following steps:
 * - Logs session ID, user ID, and input command via logEye
 * - Removes existing user and account databases to prepare for fresh tests
 * - Parses input arguments to determine client type ("user" or "server")
 * - Checks if debug mode is enabled using process.debugStatus()
 * - Allows execution of unit tests only in debug mode
 * - Sets handle.sessionData.hash[0] to 1 if allowed
 * - Sends response to client indicating whether tests can be run
 * - Logs success or failure using logEye
 *
 * @param handle Reference to handleInfo containing session data,
 *               command string, hash/message arrays, and pipe info.
 *
 * @throws Throws a response via throw_response() if:
 *         - Debug mode is off
 *         - Invalid client type input is provided
 *
 * @note
 * Side effects:
 * - Deletes existing user and account databases
 * - Produces trace logs via logEye
 * - Sends session/network response using throw_response()
 *
 * @retval void Execution result is indicated via handle.sessionData.hash[0]
 *         and the response message.
 */
	void execute(handleInfo& handle) override {  // unit_test <client_type>
		int log_id = logEye.logTrace("unit_test Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);



		//================================
		cout << handle.sessionData.sessionId << endl;

		//================================

		logEye.commentTrace(log_id, "Remove existing databases");
		std::filesystem::remove("../" + process.getUserDBPath());
		std::filesystem::remove("../" + process.getAccountDBPath());



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
				logEye.endTrace(log_id, SUCCESS, "Allow to run UnitTest");
				throw_response(handle, "");
				return;
			}
			else if (client_type == "server") {
				//here i must run Server Client unit test
				handle.sessionData.hash[0] = 1; // allow to run UnitTest
				logEye.endTrace(log_id, SUCCESS, "Allow to run UnitTest for server client");


				return;
			};
		}
		else {
			throw_response(handle, "You can't run UnitTest without debug mode. Run this first ('debug' command)");
			logEye.endTrace(log_id, FAILURE, "User can't run UnitTest when debug mode OFF.");
			return;
		};
		
	};
	string name()const override {
		return "unit_test";
	};

};