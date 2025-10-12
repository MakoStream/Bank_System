#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <chrono>
#include "user_info.h"

using namespace std;

void handleOp(sessionConstruct& sessionData, sessionConstruct& response, HANDLE& hPipe, const string& input, DWORD& bytesWritten, DWORD& bytesRead){
    strncpy(sessionData.cmd, input.c_str(), sizeof(sessionData.cmd) - 1);
    sessionData.cmd[sizeof(sessionData.cmd) - 1] = '\0';

    WriteFile(hPipe, &sessionData, sizeof(sessionData), &bytesWritten, NULL);
    ReadFile(hPipe, &response, sizeof(response), &bytesRead, NULL);

    // ��������� ��� ���� ������ �������
    sessionData = response;
}

int main() {
    setlocale(LC_ALL, "ukr");
    cout << "̳�-��������� ������� (CLI)\n";

    const char* pipeName = R"(\\.\pipe\bankPipe123456789)";
    HANDLE hPipe = INVALID_HANDLE_VALUE;

    cout << "���������� �'������� �� ��������...\n";

    // ���� ��������� ����� ����������
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
            cout << "ϳ�������� �� �������!\n";
            break;
        }

        DWORD err = GetLastError();
        if (err == ERROR_PIPE_BUSY) {
            cout << "������ ��������, ���������� �����...\n";
            WaitNamedPipeA(pipeName, 2000);
        }
        else {
            cout << "���� �'�������. ������ ����� 2 �������...\n";
            this_thread::sleep_for(chrono::seconds(2));
        }
    }

    sessionConstruct sessionData{};
    sessionConstruct response{};
    DWORD bytesWritten = 0;
    DWORD bytesRead = 0;

    // ���������� ����� SID
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
