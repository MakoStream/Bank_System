#pragma once

#include "../Command.h"
#include "../User.h"
#include <iostream>
#include "../mainProcess.h"

class getSessionIdCommand : public Command {

public:
    void execute(handleInfo& handle) override {
        handle.sessionData.sessionId = process.new_session();
        WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
        return;
    }

    std::string name() const override {
        return "getSID";
    }

};