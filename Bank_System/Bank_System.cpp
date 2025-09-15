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

vector<string> split(const string& input) {
    vector<string> tokens;
    istringstream iss(input);
    string word;
    while (iss >> word) {
        tokens.push_back(word);
    };
    return tokens;
};

Logger logger; // глобально

void signalHandler(int signal) {
    logger.write("Програма завершилася сигналом " + std::to_string(signal));
    exit(signal);
}
void onExit() {
    logger.write("Програма завершилася");
}




struct Session {
    int id;
    User user;
};

class mainProcessClass {
    
};

class Admin {  // Дані адміністратора
    int id;
    char login[32];
    char password[32];
};



int main()
{
    signal(SIGINT, signalHandler);   // Ctrl+C
    signal(SIGABRT, signalHandler);  // аварійне завершення
    signal(SIGTERM, signalHandler);  // kill процес
    atexit(onExit); // викликається при нормальному exit()
    setlocale(LC_ALL, "ukr");

    commands cmd;
    cout << "Міні-банківська система (CLI)\n";
    cout << "Введіть команду (help для списку):\n";

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
