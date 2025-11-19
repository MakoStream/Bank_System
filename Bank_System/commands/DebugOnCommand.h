#include "../Command.h"
#include "../mainProcess.h"
#include "../LogEye.h"

/**
 * @class DebugOnCommand
 * @brief Activates debug mode for the system.
 * @details Turns on debug mode, prints configuration,
 * logs all actions via logEye.
 * @note Requires: handleInfo, logEye
 * @note Syntax: debug
 */
class DebugOnCommand : public Command {

public:
	/**
 * @brief Executes the debug activation command.
 *
 * @details
 * Enables the system's debug mode.
 * The method:
 * - Logs the start of the debug command execution
 * - Records session ID, user ID, and raw input
 * - Activates debug mode through process.debugOn()
 * - Prints the current system configuration (process.printConfig())
 * - Sends a confirmation response to the client
 * - Finalizes the trace with SUCCESS status
 *
 * @param handle Reference to handleInfo containing session data
 *               and the command string.
 *
 * @note
 * Side effects:
 * - Modifies global/system state by enabling debug mode
 * - Prints system configuration to the console or log output
 * - Produces log entries via logEye
 * - Sends a session/network response using throw_response()
 *
 * @retval void This method does not return a value directly.
 *         No status is reported via hash[0] for this command.
 */
	void execute(handleInfo& handle) override {
		int log_id = logEye.logTrace("debug Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);
		

		process.debugOn();
		logEye.warning("Debug ON!");
		throw_response(handle, "Debug activated");
		process.printConfig();
		logEye.endTrace(log_id, SUCCESS, "Debug activated");
		return;
	};
	string name() const override {
		return "debug";
	};
};