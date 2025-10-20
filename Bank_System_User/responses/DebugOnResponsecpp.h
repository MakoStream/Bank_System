#include "../Response.h"

class DebugOnResponse :public Response {

public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		std::cout << "\033[2J\033[H";
		cout << handle.sessionData.msg[0] << endl;
	};
	string name() const override { return "debug"; };
};