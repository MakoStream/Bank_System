#include "../Command.h"
#include "../mainProcess.h"

class userLogoutCommand : public Command {
public:
	void execute(handleInfo& handle) override {
		process.logout(handle.sessionData.sessionId);


		handle.sessionData.sessionId = -1;
		handle.sessionData.auth_key[0] = '\0';
		throw_response(handle, "Logged out successfully.");
		return;
	};
	string name() const override {
		return "logout";
	};
};