#include "../Response.h"

class LoginResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		// Process the login response
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);

		if (handle.sessionData.hash[0] == 1) {
			strncpy(currentUser.login, handle.sessionData.msg[0], sizeof(handle.sessionData.msg[0]) - 1);
			strncpy(currentUser.name, handle.sessionData.msg[1], sizeof(handle.sessionData.msg[1]) - 1);
			strncpy(currentUser.surname, handle.sessionData.msg[2], sizeof(handle.sessionData.msg[2]) - 1);
			cout << "Hello, " << currentUser.name << " " << currentUser.surname << "!\n" << endl;
			return;
		}
		else {
			cout << "Login failed: " << handle.sessionData.msg[0] << "\n" << endl;
			return;
		};
	}
	std::string name() const override {
		return "login";
	}
};