#include "../Response.h"
#include "mainWindow.h"


class GetSessionIdResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		// Process the get_session_id response
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		std::cout << "Session ID: " << handle.sessionData.sessionId << "\n"; // SIG correct, but
		w.setSessionId(handle.sessionData.sessionId);
		return;
	}

	std::string name() const override {
		return "getSID";
	}
};