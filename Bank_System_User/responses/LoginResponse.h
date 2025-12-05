#include "../Response.h"
#include "../mainWindow.h"

class LoginResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
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


			string cmdText = "getUserAccounts";
			strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
			handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';

			response_manager.get_response(handle);
			for (int a : currentUser.accounts_id) {
				string cmdText = "account_info_id " + to_string(a);
				strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
				handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';

				response_manager.get_response(handle);

				vector<string> args = split(handle.sessionData.msg[0]);
				//w.AddAccountBoxItem(args[1], "UAH", stoi(args[3]));
				w.addUserCardsBoxItem(args[1], a);
			};

			w.hideLoginWindow();
			w.showMainMenuWindow();

			w.setUserId(handle.sessionData.userId);
			//w.ClientUserDataUpdate();
			w.setMessage("Login successful!");
			return;
		}
		else {
			cout << "Login failed: " << handle.sessionData.msg[0] << "\n" << endl;
			w.setMessage(handle.sessionData.msg[0]);
			return;
		};
	}
	std::string name() const override {
		return "login";
	}
};