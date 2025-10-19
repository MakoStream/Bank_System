#include "../Response.h"


class UserListResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		cout << "Список користувачів:\n";
		for (int i = 0; i < 5; i++) {
			if (strlen(handle.sessionData.msg[i]) > 0) {
				cout << handle.sessionData.msg[i] << endl;
			}
		}
	};
	string name() const override {
		return "user_list";
	};
};