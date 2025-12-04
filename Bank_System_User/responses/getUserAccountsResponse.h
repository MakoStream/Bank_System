#include "../Response.h"
#include "../user_info.h"

struct getUserAccountsResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		if (handle.sessionData.hash[0] == 0) {
			cout << "Помилка: " << handle.sessionData.msg[0] << endl;
			return;
		}
		else if (handle.sessionData.hash[0] == 1) {
			cout << handle.sessionData.msg[1] << endl;
			for (string a : split(string(handle.sessionData.msg[1]))) {
				currentUser.accounts_id.push_back(stoi(a));
			};
			return;
		}
	};
	string name() const override {
		return "getUserAccounts";
	}
};
