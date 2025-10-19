#include "../Response.h"

class NewAccountResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		// Process the new account response
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);

		std::cout << handle.sessionData.msg[0] << std::endl;

		return;

	}
	std::string name() const override {
		return "new_account";
	}
};