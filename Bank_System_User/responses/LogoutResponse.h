#include "../Response.h"

class LogoutResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		if (handle.sessionData.sessionId == -1) {
			cout << "Successful logout" << endl;
		}
		else {
			cout << "Logout failed: No active session" << endl;
		};
		return;
	};
	string name() const override {
		return "logout";
	};
};