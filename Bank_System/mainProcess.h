#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
//#include "Command.h"
#include <csignal>
#include <unordered_map>

extern enum CMD_FS { NONE, GET_SID, LOGIN, LOGOUT, GET_INFO };

typedef struct Session {
    int sesion_id;
    int user_id;
};

extern Session emptySession;

class mainProcess {
    std::vector<Session> loggined_users;
public:
	mainProcess();
    int new_session();
    void login(int session_id, char login[32], char password[32]);
    void printSessions();
    Session& getUserSession(int session_id);
};
extern mainProcess process;