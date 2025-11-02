#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
//#include "commands.h"
#include "mainProcess.h"
#include "basic_functions.h"

#include "CommandsManager.h"

#include <csignal>
#include "User.h"
#include <thread>

using namespace std;


string configName = "config.ini";
passportData emptyPassport = { "", 0, "", "", "" };
User emptyUser(
    -1,                  // int id
    "",                  // const char* login
    "",                  // const char* password
    "",                  // const char* name
	"",                  // const char* surname
    0,                   // int phone
	emptyPassport,       // passportData passport
	0,                   // int TIN
	USER_AVAILABLE,      // userStatus status
	USER                 // userRole role
);

Logger logger;  // визначення глобальної змінної


mainProcess process;
CommandsManager manager;

// Function: HandleClient(HANDLE hPipe)
// Description: Splits a string into words separated by whitespace and returns them as a vector
// Requirements: <string>, <sstream>, <vector>
// Required for: main loop input parsing in main()

void HandleClient(HANDLE hPipe) {
    while (true) {
        sessionConstruct sessionData;
        DWORD bytesRead, bytesWritten;

        handleInfo handle = { hPipe, sessionData, bytesRead, bytesWritten };

        BOOL success = ReadFile(hPipe, &sessionData, sizeof(sessionData), &bytesRead, NULL);
        if (!success || bytesRead == 0) {
            std::cout << "Client disconnected\n";
            break;
        }
        //process.printSessions();
        if (process.compareAuthKey(sessionData, process.getUserSession(sessionData.sessionId)) == false) {
            std::cout << "Auth key mismatch for session " << sessionData.sessionId << "\n";
            strncpy(sessionData.cmd, "Auth key mismatch!", sizeof(sessionData.cmd) - 1);
            WriteFile(hPipe, &sessionData, sizeof(sessionData), &bytesWritten, NULL);
			break;
        };

        vector<string> args = split(sessionData.cmd);
        manager.execute(handle);
        
        



        // Відправляємо назад клієнту
        //WriteFile(hPipe, &sessionData, sizeof(sessionData), &bytesWritten, NULL);
    }

    CloseHandle(hPipe);
}



// Function: signalHandler
// Description: Handles system signals (SIGINT, SIGABRT, SIGTERM) and logs termination
// Requirements: <csignal>, logger
// Required for: main()
void signalHandler(int signal) {
    logger.write("Програма завершилася сигналом " + to_string(signal));
    exit(signal);
}

// Function: onExit
// Description: Called automatically at normal program exit to log termination
// Requirements: Logger
// Required for: main()
void onExit() {
    logger.exit();
}



// Function: main
// Description: Main program entry point, sets signal handlers, runs command loop
// Requirements: split(), Logger, commands class, signal handling
// Required for: entire program execution
int main()
{

    
    signal(SIGINT, signalHandler);   // Ctrl+C
    signal(SIGABRT, signalHandler);  // аварійне завершення
    signal(SIGTERM, signalHandler);  // kill процес
    atexit(onExit); // викликається при нормальному exit()
    setlocale(LC_ALL, "ukr");

	process.printConfig();
    


    const char* pipeName = R"(\\.\pipe\bankPipe123456789)";
    while (true) {
        HANDLE hPipe = CreateNamedPipeA(
            pipeName,
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            0, 0, 0, NULL
        );

        if (hPipe == INVALID_HANDLE_VALUE) {
            std::cerr << "CreateNamedPipe failed\n";
            return 1;
        }

        std::cout << "Waiting for client...\n";
        ConnectNamedPipe(hPipe, NULL);
        std::cout << "Client connected!\n";

        std::thread t(HandleClient, hPipe);
        t.detach();
    }
}
