/**
@file Bank_System.cpp
@brief Main module of the bank system server.
@details This file defines the central logic of the bank server. It initializes
global objects, configures logging, loads system settings, registers signal
handlers and exit callbacks, and starts the main loop. The module creates a
named pipe for inter-process communication and spawns a new thread for each
client connection using HandleClient(). This file orchestrates the entire
runtime behavior of the bank system.
@note This is the core file required for the entire system to function.
*/

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
#include "LogEye.h"

#include "CommandsManager.h"

#include <csignal>
#include "User.h"
#include <thread>

using namespace std;

LogEye logEye;
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

/**
@brief Handles a single client request through a named pipe.
@details The function splits an input string into words separated by
whitespace, converts them into a vector of strings, and passes the
processed data to the main loop.
@param hPipe Handle to the pipe through which client data is received.
@note Required headers: <string>, <sstream>, <vector>.
@note Used for input parsing in the main loop (main()).
*/

void HandleClient(HANDLE hPipe) {
    while (true) {
        sessionConstruct sessionData;
        DWORD bytesRead, bytesWritten;

        handleInfo handle = { hPipe, sessionData, bytesRead, bytesWritten };

        BOOL success = ReadFile(hPipe, &sessionData, sizeof(sessionData), &bytesRead, NULL);
        if (!success || bytesRead == 0) {
            //std::cout << "Client disconnected\n";
			logEye.info("Client disconnected from the pipe.");
            break;
        }
        //process.printSessions();
        if (process.compareAuthKey(sessionData, process.getUserSession(sessionData.sessionId)) == false) {
            //std::cout << "Auth key mismatch for session " << sessionData.sessionId << "\n";
			logEye.warning("Auth key mismatch for session " + to_string(sessionData.sessionId) + ".");
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



/**
@brief Handles system termination signals and logs the shutdown event.
@details The function processes incoming system signals such as
SIGINT, SIGABRT, and SIGTERM. When a signal is received, it writes
a log message and terminates the program using exit().
@param signal The system signal number that triggered the handler.
@note Required headers: <csignal>.
@note Requires a logger instance.
@note Used in main() to ensure clean shutdown and logging.
*/
void signalHandler(int signal) {
    logger.write("Програма завершилася сигналом " + to_string(signal));
    exit(signal);
}

/**
@brief Called automatically at normal program termination.
@details This function is executed when the program exits normally and writes a termination record using the logger.
@note Requires a logger instance.
@note Used in main() to log program termination.
*/
void onExit() {
    logger.exit();
}



/**
@brief Main program entry point.
@details Initializes the application, sets up signal handlers, registers exit callbacks, loads configuration, starts logging, and enters the main loop that accepts clients through a named pipe. Each client connection is handled by a separate thread using HandleClient().
@note Requirements: split(), Logger, commands class, signal handling.
@note Required for entire program execution.
*/
int main()
{

    
    signal(SIGINT, signalHandler);   // Ctrl+C
    signal(SIGABRT, signalHandler);  // аварійне завершення
    signal(SIGTERM, signalHandler);  // kill процес
    atexit(onExit); // викликається при нормальному exit()
    setlocale(LC_ALL, "ukr");

	process.printConfig();
    

	logEye.info("Bank system started.");
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

        //std::cout << "Waiting for client...\n";
        ConnectNamedPipe(hPipe, NULL);
        //std::cout << "Client connected!\n";
		logEye.info("Client connected to the pipe.");

        std::thread t(HandleClient, hPipe);
        t.detach();
    }
}
