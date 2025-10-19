#include "../Response.h"

class NewUserResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		// Process the new user response
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);

		cout << handle.sessionData.msg[0] << endl;
		return;
	};
	std::string name() const override {
		return "reg_user";
	}
};