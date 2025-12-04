#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//#include <windows.h>
#include <string>
#include <thread>
#include <chrono>
#include "user_info.h"
#include "basic_functions.h"
#include "ResponseManager.h"
#include <vector>

#include <QApplication>
#include "mainWindow.h"


QThread* workerThread = nullptr;
Worker* worker = nullptr;

//#define byte unsigned char

handleInfo* handleP = nullptr;

//========================
int argc = 0;
char* argv[1] = { nullptr };
QApplication a(argc, argv);

MainWindow w;

//============================

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

    
    cout << sessionData.sessionId << endl;
    response_manager.get_response(*handleP);

};

int main() {

    workerThread = new QThread();
    worker = new Worker();
    worker->moveToThread(workerThread);
    workerThread->start();


    setlocale(LC_ALL, "ukr");
    cout << "Міні-банківська система (CLI)\n";
    HANDLE hPipe = INVALID_HANDLE_VALUE;

    const char* pipeName = R"(\\.\pipe\bankPipe123456789)";
    //HANDLE hPipe = INVALID_HANDLE_VALUE;

    while (true) {
        hPipe = CreateFileA(pipeName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (hPipe != INVALID_HANDLE_VALUE) {
            cout << "Підключено до сервера!\n";
            break;
        }
        DWORD err = GetLastError();
        if (err == ERROR_PIPE_BUSY) WaitNamedPipeA(pipeName, 2000);
        else this_thread::sleep_for(chrono::seconds(2));
    }

    sessionConstruct sessionData{};
    sessionConstruct response{};
    DWORD bytesWritten = 0;
    DWORD bytesRead = 0;

    handleInfo handle = { hPipe, sessionData, bytesRead, bytesWritten };
    handleP = &handle;
    

	w.hideRegWindow();
	w.hideMainMenuWindow();
    w.show();

    // Потік для CLI
    std::thread cliThread([&]() {

        strncpy(sessionData.cmd, "getSID", sizeof(sessionData.cmd) - 1);
        sessionData.cmd[sizeof(sessionData.cmd) - 1] = '\0';

        handleOp(sessionData, response, hPipe, sessionData.cmd, bytesWritten, bytesRead);

        string input;
        while (true) {
            cout << "> ";
            getline(cin, input);
            if (input.empty()) continue;
            handleOp(sessionData, response, hPipe, input, bytesWritten, bytesRead);
            this_thread::sleep_for(chrono::seconds(1));
        }

        CloseHandle(hPipe);
        });

    cliThread.detach();

    return a.exec(); // GUI працює в головному потоці
}
