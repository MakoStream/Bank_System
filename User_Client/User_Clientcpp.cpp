#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <thread>
#include <fstream>
#include <sstream>
//#include "Logger.h"
//#include "commands.h"
#include <csignal>
//#include "DB_op.h"

using namespace std;

enum CMD_FS { NONE, GET_SID, LOGIN, LOGOUT };

struct sessionConstruct {
    int sessionId = 0;
    int hash[10];
    CMD_FS cmd_fs = GET_SID;
    char cmd[256];
};

void handleOp(sessionConstruct& sessionData, sessionConstruct& response, HANDLE& hPipe, string& input, DWORD& bytesWritten, DWORD& bytesRead) {
    strncpy(sessionData.cmd, input.c_str(), sizeof(sessionData.cmd) - 1);
    WriteFile(hPipe, &sessionData, sizeof(sessionData), &bytesWritten, NULL);


    ReadFile(hPipe, &response, sizeof(response), &bytesRead, NULL);

    //Тут всякі операції після отримання данних в reponse
    cout << "Gain " << response.cmd << " " << response.cmd_fs << endl;
    sessionData = response;
    //===================

};
//handleOp(sessionData, response, hPipe, input, bytesWritten, bytesRead)


int main()
{
    setlocale(LC_ALL, "ukr");


    cout << "Міні-банківська система (CLI)\n";
    cout << "Введіть команду (help для списку):\n";

    const char* pipeName = R"(\\.\pipe\bankPipe123456789)";

    HANDLE hPipe = CreateFileA(
        pipeName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
        std::cerr << "Cannot connect to server\n";
        return 1;
    }

    int counter = 0;
    string input;
    sessionConstruct sessionData;
    DWORD bytesWritten;
    sessionConstruct response;
    DWORD bytesRead;


    handleOp(sessionData, response, hPipe, input, bytesWritten, bytesRead);
    while (true) {


        cout << "> ";
        getline(cin, input);

        if (input.empty()) continue;


        handleOp(sessionData, response, hPipe, input, bytesWritten, bytesRead);
        //===================


        counter++;
        Sleep(1000); // пауза 1 секунда між відправками
    }

    CloseHandle(hPipe);
    return 0;
}

