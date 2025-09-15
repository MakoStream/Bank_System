#define _CRT_SECURE_NO_WARNINGS
#include "Logger.h"
#include <fstream>
#include <ctime>
#include <vector>
#include <string>
#include "DB_op.h"

using namespace std; 

string Logger::currentDate() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return buf;
}

string Logger::currentTime() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%H.%M.%S", localtime(&now));
    return buf;
}

Logger::Logger() {
    logFileName = "logs/" + currentDate() + "_" + currentTime() + ".txt";
    ofstream logFile(logFileName, ios::app);
    if (logFile.is_open()) {
        logFile << "[START] " << currentTime() << " - Програма запущена" << endl;
        logFile.close();
    }
}

void Logger::write(const string& message) {
    ofstream logFile(logFileName, ios::app);
    if (logFile.is_open()) {
        logFile << "[LOG] " << currentTime() << " - " << message << endl;
        logFile.close();
    }
}

void Logger::cmd(int session_id, int user_id, vector<string>& args) {
    ofstream logFile(logFileName, ios::app);
    string commandText = "";
    for (string a : args) {
        commandText += a + " ";
    };
    if (logFile.is_open()) {
        logFile << "[CMD] " << currentTime() << " - (sid: " << session_id << " | uid: " << user_id << 
            ") executed the command '" << commandText << "'"
            << endl;
        logFile.close();
    }
};