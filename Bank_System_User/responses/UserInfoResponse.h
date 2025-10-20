#include "../Response.h"
#include "../user_info.h"

class UserInfoResponse : public Response {
public:
	void get_response(handleInfo& handle) override {
		ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
		if (handle.sessionData.hash[0] == 1) {
			vector <string> text_data = split(string(handle.sessionData.msg[1]));
			int* hash = handle.sessionData.hash;
			cout << "Information about " << text_data[0] << endl;
			cout << "U-id: " << hash[1]
				<< "\nlogin: " << text_data[0]
				<< "\nName: " << text_data[1]
				<< "\nSurname: " << text_data[2]
				<< "\nPhone: " << hash[2]
				<< "\nTIN: " << hash[3] << endl;
		};
	}
	std::string name() const override {
		return "user_info";
	}
};