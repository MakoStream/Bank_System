#pragma once
#include <string>
#include <vector>
#include "DB_op.h"
#include "mainProcess.h"

using namespace std;

class Logger {
private:
    std::string logFileName;

    std::string currentDate();
    std::string currentTime();

public:
    Logger();
    void write(const std::string& message);
    void cmd(int session_id, int user_id, vector<string>& args);
    void newSession(int session_id, int client_type);
    void userLoggined(Session user);
    void createdDB(Session user);
    void AccessDanied(string command_name, Session user);
    void exit();
};

extern Logger logger;