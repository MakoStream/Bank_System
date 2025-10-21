#define _CRT_SECURE_NO_WARNINGS
#include "ResponseManager.h"

// here need to include all response headers (responses/)
#include "responses/LoginResponse.h"
#include "responses/LogoutResponse.h"
#include "responses/GetSessionIdResponse.h"
#include "responses/AccountListResponse.h"
#include "responses/UserListResponse.h"
#include "responses/NewAccountResponse.h"
#include "responses/NewUserResponse.h"
#include "responses/AccountVerifyResponse.h"
#include "responses/AccountBanResponse.h"
#include "responses/CreateDBResponse.h"
#include "responses/UserInfoResponse.h"
#include "responses/MeResponse.h"
#include "responses/DebugOnResponse.h"
#include "responses/UnitTestResponse.h"

// ======================================================

ResponseManager::ResponseManager() {
	// here need to add all responses to vector (responses/)
	// responses.push_back(std::make_unique<YourResponseClass>());
	responses.push_back(std::make_unique<LoginResponse>());
	responses.push_back(std::make_unique<LogoutResponse>());
	responses.push_back(std::make_unique<GetSessionIdResponse>());
	responses.push_back(std::make_unique<AccountListResponse>());
	responses.push_back(std::make_unique<UserListResponse>());
	responses.push_back(std::make_unique<NewAccountResponse>());
	responses.push_back(std::make_unique<NewUserResponse>());
	responses.push_back(std::make_unique<AccountVerifyResponse>());
	responses.push_back(std::make_unique<AccountBanResponse>());
	responses.push_back(std::make_unique<AccountUnbanResponse>());
	responses.push_back(std::make_unique<CreateDBResponse>());
	responses.push_back(std::make_unique<UserInfoResponse>());
	responses.push_back(std::make_unique<MeResponse>());
	responses.push_back(std::make_unique<DebugOnResponse>());
	responses.push_back(std::make_unique<UnitTestResponse>());
}

void ResponseManager::get_response(handleInfo& handle) {
	//cout << handle.sessionData.cmd << endl;
    string cmdStr(handle.sessionData.cmd);
    std::vector<std::string> args = split(cmdStr);
    string cmdName = args[0];
    for (auto& cmd : responses) {
		//cout << cmd->name() << endl;
        if (cmd->name() == cmdName) {
			//cout << cmd->need_execute() << endl;
			if (cmd->need_execute()) {
				
				WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);
			};
            cmd->get_response(handle);
            strncpy(handle.sessionData.cmd, "", sizeof(handle.sessionData.cmd) - 1);
            handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
            return;
        }
    }
    std::cout << "Невідома команда: " << cmdName << "\n";
	ReadFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesRead, NULL);
};