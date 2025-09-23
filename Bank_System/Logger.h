#pragma once
#include <string>
#include <vector>
#include "DB_op.h"

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
    void userLoggined(int session_id, int user_id);
    void exit();
};

extern Logger logger;