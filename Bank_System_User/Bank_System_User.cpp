#define _CRT_SECURE_NO_WARNINGS
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <iostream>
//#include <windows.h>
#include <string>
#include <thread>
#include <chrono>
#include "user_info.h"
#include "basic_functions.h"
#include "ResponseManager.h"
#include <vector>

ResponseManager response_manager;

fronted_User emptyUser = { "", "", "" };
fronted_User currentUser = emptyUser;


using namespace std;

void handleOp(sessionConstruct& sessionData, sessionConstruct& response, HANDLE& hPipe, const string& input, DWORD& bytesWritten, DWORD& bytesRead) {

    strncpy(sessionData.cmd, input.c_str(), sizeof(sessionData.cmd) - 1);
    sessionData.cmd[sizeof(sessionData.cmd) - 1] = '\0';

    vector<string> args = split(input);

    //WriteFile(hPipe, &sessionData, sizeof(sessionData), &bytesWritten, NULL);
    //ReadFile(hPipe, &response, sizeof(response), &bytesRead, NULL);

	handleInfo handle = { hPipe, sessionData, bytesRead, bytesWritten };
    response_manager.get_response(handle);

    // Оновлення сесії після відповіді сервера
    //sessionData = response;

    /*if (args[0] == "account_list" || args[0] == "user_list") {
        for (int i = 0; i < 5; i++) {
			cout << response.msg[i] << endl;
        };
    };*/
    
};

int main() {
    setlocale(LC_ALL, "ukr");
    cout << "Міні-банківська система (CLI)\n";

    const char* pipeName = R"(\\.\pipe\bankPipe123456789)";
    HANDLE hPipe = INVALID_HANDLE_VALUE;

    cout << "Очікування з'єднання із сервером...\n";

    // Цикл повторних спроб підключення
    while (true) {
        hPipe = CreateFileA(
            pipeName,
            GENERIC_READ | GENERIC_WRITE,
            0,
            NULL,
            OPEN_EXISTING,
            0,
            NULL
        );

        if (hPipe != INVALID_HANDLE_VALUE) {
            cout << "Підключено до сервера!\n";
            break;
        }

        DWORD err = GetLastError();
        if (err == ERROR_PIPE_BUSY) {
            cout << "Сервер зайнятий, очікування черги...\n";
            WaitNamedPipeA(pipeName, 2000);
        }
        else {
            cout << "Немає з'єднання. Повтор через 2 секунди...\n";
            this_thread::sleep_for(chrono::seconds(2));
        }
    }

    sessionConstruct sessionData{};
    sessionConstruct response{};
    DWORD bytesWritten = 0;
    DWORD bytesRead = 0;

    // Початковий запит SID
    strncpy(sessionData.cmd, "getSID", sizeof(sessionData.cmd) - 1);
    sessionData.cmd[sizeof(sessionData.cmd) - 1] = '\0';

    handleOp(sessionData, response, hPipe, sessionData.cmd, bytesWritten, bytesRead);

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input.empty()) continue;
        handleOp(sessionData, response, hPipe, input, bytesWritten, bytesRead);
        Sleep(1000);
    }

    CloseHandle(hPipe);
    return 0;
}
