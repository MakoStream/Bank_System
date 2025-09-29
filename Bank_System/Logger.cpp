#define _CRT_SECURE_NO_WARNINGS
#include "mainProcess.h"
#include "Logger.h"
#include <fstream>
#include <ctime>
#include <vector>
#include <string>
#include <sstream>
#include "DB_operations.h"

using namespace std; 

// Function: Logger::currentDate
// Description: Returns current date as string YYYY-MM-DD
// Requirements: <ctime>, <string>
// Required for: Logger constructor, log file naming
string Logger::currentDate() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d", localtime(&now));
    return buf;
}

// Function: Logger::currentTime
// Description: Returns current time as string HH.MM.SS
// Requirements: <ctime>, <string>
// Required for: Logger constructor, log writing
string Logger::currentTime() {
    time_t now = time(nullptr);
    char buf[20];
    strftime(buf, sizeof(buf), "%H.%M.%S", localtime(&now));
    return buf;
}

// Constructor: Logger
// Description: Initializes log file with current date and time, logs start event
// Requirements: currentDate(), currentTime(), <fstream>, <iostream>
// Required for: global logger
Logger::Logger() {
    logFileName = "logs/" + currentDate() + "_" + currentTime() + ".txt";
    ofstream logFile(logFileName, ios::app);
    if (logFile.is_open()) {
        logFile << "[START] " << currentTime() << " - Програма запущена" << endl;
        logFile.close();
    }
}


// Function: Logger::write
// Description: Writes a log message to the log file
// Requirements: currentTime(), <fstream>, <string>
// Required for: signal handling, program logging
void Logger::write(const string& message) {
    ofstream logFile(logFileName, ios::app);
    if (logFile.is_open()) {
        logFile << "[LOG] " << currentTime() << " - " << message << endl;
        logFile.close();
    }
}

// Function: Logger::cmd
// Description: Logs command execution with session_id and user_id
// Requirements: currentTime(), <fstream>, <string>, <vector>
// Required for: main(), commands execution logging
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
}

void Logger::newSession(int session_id, int client_type) {
    ostringstream oss;
    oss << "Some user with S-Id:" << session_id << " connected with use User Client";
    write(oss.str());
}

void Logger::userLoggined(Session user) {
    ostringstream oss;
    oss << "User with S-Id:" << user.sesion_id << " loggined as U-ID: " << user.user_id;
    write(oss.str());
}
void Logger::createdDB(Session user) {
    ostringstream oss;
    oss << "User (S-Id: " <<user.sesion_id << " | U-Id: "<<user.user_id <<") create DB";
    write(oss.str());
}

void Logger::AccessDanied(string cmd_name, Session user) {
    ostringstream oss;
    oss << "Executing command" << cmd_name <<" for User(S - Id: " << user.sesion_id << " | U - Id : " << user.user_id << ") is Danied";
    write(oss.str());
}

void Logger::exit() {
    write("Process terminated succesful!");
};