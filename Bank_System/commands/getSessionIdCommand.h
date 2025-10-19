#pragma once

#include "../Command.h"
#include "../User.h"
#include <iostream>
#include "../mainProcess.h"

class getSessionIdCommand : public Command {

public:
    void execute(handleInfo& handle) override {
        handle.sessionData.sessionId = process.new_session();
		throw_response(handle, "New session ID generated: " + std::to_string(handle.sessionData.sessionId));
        return;
    }

    std::string name() const override {
        return "getSID";
    }

};