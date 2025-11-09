#pragma once

#include "../Command.h"
#include "../User.h"
#include <iostream>
#include "../mainProcess.h"
#include "../LogEye.h"

class getSessionIdCommand : public Command {

public:
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