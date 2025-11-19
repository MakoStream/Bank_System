#pragma once

#include "../Command.h"
#include "../User.h"
#include <iostream>
#include "../mainProcess.h"
#include "../LogEye.h"

/**
 * @class getSessionIdCommand
 * @brief Generates a new session ID for the client.
 * @details Logs session creation and provides session ID.
 * @note Requires: handleInfo, logEye
 * @note Syntax: getSID
 */
class getSessionIdCommand : public Command {

public:
    /**
 * @brief Executes the session ID generation command.
 *
 * @details
 * Creates a new session for the client.
 * The method:
 * - Logs the start of the command
 * - Records the raw input command
 * - Generates a new session ID via process.new_session()
 * - Stores the session ID in handle.sessionData.sessionId
 * - Sends the session ID back to the client
 * - Finishes logging with SUCCESS status
 *
 * @param handle Reference to handleInfo containing session data
 *               and the buffer for command execution results.
 *
 * @note
 * Side effects:
 * - Modifies handle.sessionData.sessionId
 * - Produces log entries via logEye
 * - Sends a session/network response through throw_response()
 *
 * @retval void This method does not return a direct value.
 *         The result (new session ID) is delivered through throw_response().
 */
    void execute(handleInfo& handle) override {
        int log_id = logEye.logTrace("getSID command");
        logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);


        handle.sessionData.sessionId = process.new_session();
		logEye.msgTrace(log_id, "generated session ID", std::to_string(handle.sessionData.sessionId), true);

		throw_response(handle, "New session ID generated: " + std::to_string(handle.sessionData.sessionId));
		logEye.endTrace(log_id, SUCCESS, "New session Id generated");
        return;
    }

    std::string name() const override {
        return "getSID";
    }

};