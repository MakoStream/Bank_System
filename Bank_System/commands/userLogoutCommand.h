#include "../Command.h"
#include "../mainProcess.h"
#include "../LogEye.h"


/** @class userLogoutCommand
 *  @brief Command class for user logout.
 *  @details Handles:
 *   - Terminating the current user session
 *   - Logging the logout action via Logger
 *  @note Requires: Session, LogEye
 *  @note Syntax: logout
 *  @used For:
 *   - Ending active sessions safely
 */
class userLogoutCommand : public Command {
public:
	/**
 * @class userLogoutCommand
 * @brief Handles user logout from the system.
 *
 * @details
 * The userLogoutCommand performs the following actions:
 * - Logs session ID, user ID, and input command via logEye.
 * - Terminates the current user session using process.logout().
 * - Clears session-specific data such as userId and authentication key.
 * - Sends a logout confirmation message via throw_response().
 * - Logs the successful logout action using logEye.
 *
 * @param handle Reference to handleInfo containing:
 *   - sessionData (session ID, input command, userId, auth_key)
 *   - Pipe information for communication
 * @note Syntax: logout
 * @note Side Effects:
 *   - Logs all actions using logEye
 *   - Updates session state to reflect logout
 *   - Clears authentication key
 * @retval void Execution result indicated by the logout message
 */
	void execute(handleInfo& handle) override {
		int log_id = logEye.logTrace("logout Command");
		logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
		logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);

		process.logout(handle.sessionData.sessionId);
		logEye.commentTrace(log_id, "User logged out successfully, session cleared.");


		handle.sessionData.userId = -1;
		handle.sessionData.auth_key[0] = '\0';
		throw_response(handle, "Logged out successfully.");
		logEye.endTrace(log_id, SUCCESS, "User logged out successfully.");
		return;
	};
	string name() const override {
		return "logout";
	};
};