#include "../Command.h"
#include "../mainProcess.h"

class DebugOnCommand : public Command {

public:
	void execute(handleInfo& handle) override {
		process.debugOn();
		throw_response(handle, "Debug activated");
		return;
	};
	string name() const override {
		return "debug";
	};
};