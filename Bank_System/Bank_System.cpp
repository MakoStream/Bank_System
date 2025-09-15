#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
#include "commands.h"
#include <csignal>
#include "DB_op.h"

using namespace std;


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

Logger logger; // ���������



// Function: signalHandler
// Description: Handles system signals (SIGINT, SIGABRT, SIGTERM) and logs termination
// Requirements: <csignal>, Logger
// Required for: main()
void signalHandler(int signal) {
    logger.write("�������� ����������� �������� " + std::to_string(signal));
    exit(signal);
}

// Function: onExit
// Description: Called automatically at normal program exit to log termination
// Requirements: Logger
// Required for: main()
void onExit() {
    logger.write("�������� �����������");
}



struct Session {
    int id;
    User user;
};

class mainProcessClass {
    
};

class Admin {  // ��� ������������
    int id;
    char login[32];
    char password[32];
};




// Function: main
// Description: Main program entry point, sets signal handlers, runs command loop
// Requirements: split(), Logger, commands class, signal handling
// Required for: entire program execution
int main()
{
    signal(SIGINT, signalHandler);   // Ctrl+C
    signal(SIGABRT, signalHandler);  // ������� ����������
    signal(SIGTERM, signalHandler);  // kill ������
    atexit(onExit); // ����������� ��� ����������� exit()
    setlocale(LC_ALL, "ukr");

    commands cmd;
    cout << "̳�-��������� ������� (CLI)\n";
    cout << "������ ������� (help ��� ������):\n";

    string input;
    while (true) {
        cout << "> ";
        getline(cin, input);

        if (input.empty()) continue;

        vector<string> args = split(input);
        cmd.execute(args);
        logger.cmd(00000000, 0000000, args);
    }
}
