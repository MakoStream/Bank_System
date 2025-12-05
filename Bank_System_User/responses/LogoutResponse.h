#include "../Response.h"
#include "../mainWindow.h"

class LogoutResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		if (handle.sessionData.userId == -1) {
			cout << "Successful logout" << endl;
			currentUser.accounts_id.clear();
			w.setMessage("Successful logout");
			w.setUserId(-1);
			w.clearUserCardsBox();
			w.hideMainMenuWindow();
			w.showLoginWindow();
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