#include "../Response.h"

class AccountVerifyResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		if (handle.sessionData.hash[0] == 0) {
			cout << "Помилка: " << handle.sessionData.msg[0] << endl;
			return;
		}
		else if (handle.sessionData.hash[0] == 1) {
			cout << "Обліковий запис успішно верифіковано." << endl;
			return;
		}
		else {
			cout << "Помилка: Не вдалося верифікувати обліковий запис.\n";
		}
	};
	string name() const override {
		return "account_verify";
	}
};