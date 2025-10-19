#include "../Response.h"

class CreateDBResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		if (handle.sessionData.hash[0] == 0) {
			cout << "Error: " << handle.sessionData.msg[0] << endl;
			return;
		}
		else if (handle.sessionData.hash[0] == 1) {
			cout << "Database created successfully." << endl;
			return;
		}
		else {
			cout << "Error: Failed to create database.\n";
		}
	};
	string name() const override {
		return "create_db";
	}
};