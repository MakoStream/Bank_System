#include "../Command.h"
#include "../mainProcess.h"
#include "../LogEye.h"

class DebugOnCommand : public Command {

public:
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