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

#include "processCommands.h"

#include "CommandsManager.h"
#include "commands/createDBCommand.h"
#include "commands/RegisterUserCommand.h"
#include "commands/PrintUserListCommand.h"

#include <csignal>
#include "DB_operations.h"
#include <thread>

using namespace std;

User emptyUser(
    -1,                  // int id
    "",                  // const char* login
    "",                  // const char* password
    "",                  // const char* name
    0,                   // int phone
    DEFAULT,                // cardType type
    UAH,                // balanceType balance_Type
    0.0                  // double balance
);

Logger logger;  // визначення глобальної змінної


// Function: split
// Description: Splits a string into words separated by whitespace and returns them as a vector
// Requirements: <string>, <sstream>, <vector>
// Required for: main loop input parsing in main()
vector<string> split(const string& input) {
    vector<string> tokens;
    istringstream iss(input);
    string word;
    while (iss >> word) {
        tokens.push_back(word);
    };
    return tokens;
};

mainProcess process;
CommandsManager manager;
CMD_Manager cmd_manager(process);


void HandleClient(HANDLE hPipe) {
    while (true) {
        sessionConstruct sessionData;
        DWORD bytesRead, bytesWritten;

        handleInfo handle = { hPipe, sessionData, bytesRead, bytesWritten };
        // Читаємо структуру від клієнта
        BOOL success = ReadFile(hPipe, &sessionData, sizeof(sessionData), &bytesRead, NULL);
        if (!success || bytesRead == 0) {
            std::cout << "Client disconnected\n";
            break;
        }


        // Модифікуємо дані
        /*cout << sessionData.cmd << endl;
        strcpy(sessionData.cmd, "hallo");*/

        vector<string> args = split(sessionData.cmd);
        if (sessionData.cmd_fs == GET_SID) {
            cmd_manager.execute(sessionData.cmd_fs, sessionData, hPipe, bytesWritten);
            process.printSessions();
        }
        
        
        else {
            manager.execute(args[0], args, handle);
            process.printSessions();
        };
        



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
