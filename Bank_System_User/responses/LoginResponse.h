#include "../Response.h"
#include "../login.h"

class LoginResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		// Process the login response
		cout << "debug 1" << endl;
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);

		if (handle.sessionData.hash[0] == 1) {
			strncpy(currentUser.login, handle.sessionData.msg[0], sizeof(currentUser.login) - 1);
			currentUser.login[sizeof(currentUser.login) - 1] = '\0';

			strncpy(currentUser.name, handle.sessionData.msg[1], sizeof(currentUser.name) - 1);
			currentUser.name[sizeof(currentUser.name) - 1] = '\0';

			strncpy(currentUser.surname, handle.sessionData.msg[2], sizeof(currentUser.surname) - 1);
			currentUser.surname[sizeof(currentUser.surname) - 1] = '\0';
			cout << "Hello, " << currentUser.name << " " << currentUser.surname << "!\n" << endl;

			w.setUserId(handle.sessionData.userId);
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