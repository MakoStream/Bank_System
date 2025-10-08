#pragma once
#include <iostream>
#include "../Command.h"
#include "../mainProcess.h"
#include "windows.h"




class getSId : public processCommand {
	mainProcess& process;
public:
	getSId(mainProcess& p) : process(p) {}
	void execute(CMD_FS cmd_name, sessionConstruct &sessionData, HANDLE& hPipe, DWORD& bytesWritten) override {
		sessionData.sessionId = process.new_session();
		sessionData.cmd_fs = NONE;
		WriteFile(hPipe, &sessionData, sizeof(sessionData), &bytesWritten, NULL);
		return;
	}
	CMD_FS name() const override {
		return GET_SID;
	};
};