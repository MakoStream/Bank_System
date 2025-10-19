#include "../Response.h"


class AccountListResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		cout << "Список рахунків:\n";
		for (int i = 0; i < 5; i++) {
			if (strlen(handle.sessionData.msg[i]) > 0) {
				cout << handle.sessionData.msg[i] << endl;
			}
		}
	};
	string name() const override {
		return "account_list";
	};
};