#include "../Response.h"

struct AccountBanResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		if (handle.sessionData.hash[0] == 0) {
			cout << "Помилка: " << handle.sessionData.msg[0] << endl;
			return;
		}
		else if (handle.sessionData.hash[0] == 1) {
			cout << "Обліковий запис успішно заблоковано." << endl;
			return;
		}
		else {
			cout << "Помилка: Не вдалося заблокувати обліковий запис.\n";
		}
	};
	string name() const override {
		return "account_ban";
	}
};

struct AccountUnbanResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		if (handle.sessionData.hash[0] == 0) {
			cout << "Помилка: " << handle.sessionData.msg[0] << endl;
			return;
		}
		else if (handle.sessionData.hash[0] == 1) {
			cout << "Обліковий запис успішно розблоковано." << endl;
			return;
		}
		else {
			cout << "Помилка: Не вдалося розблокувати обліковий запис.\n";
		}
	};
	string name() const override {
		return "account_unban";
	}
};