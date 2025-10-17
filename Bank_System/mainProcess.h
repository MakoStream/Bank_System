#define _CRT_SECURE_NO_WARNINGS
#pragma once
//#include <iostream>
//#include "Command.h"
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
#include <csignal>
#include <unordered_map>
#include <random>
#include "Account.h"
#include "basic_functions.h"
//#include "Command.h"


typedef struct Session {
    int session_id;
    int user_id;
    char auth_key[41];
};
struct sessionConstruct {
    int sessionId = 0;
    int hash[10];
    char cmd[256];
    char auth_key[41];
    char msg[5][1024];
};

struct handleInfo {
    HANDLE& hPipe;
    sessionConstruct& sessionData;
    DWORD& bytesRead;
    DWORD& bytesWritten;
};

extern Session emptySession;

class mainProcess {
	int last_session_id;
    int last_card_PAN;
	int last_card_IBAN;

	// default db paths
	string account_db_path;
	string user_db_path;
	// db path for debug mode/unit tests
	string account_db_debug_path;
	string user_db_debug_path;

	bool debug;

    std::vector<Session> loggined_users;
    void savecfg();
public:
	mainProcess();
    int new_session();
	void setDebugMode(bool mode);
	void printConfig();
    void login(int session_id, char login[32], char password[32]);
    void printSessions();
    Session& getUserSession(int session_id);
    void generateAuthKey(Session& session, sessionConstruct& sessionData);
    Session& getSessionByID(int session_id);
    bool compareAuthKey(const sessionConstruct& sc, const Session& s);
	void transferBridge(account& from, account& to, double amount);

	//int incrementSessionID();
	int incrementCardPAN();
	int incrementCardIBAN();

	string getAccountDBPath();
	string getUserDBPath();
	string getAccountDBDebugPath();
	string getUserDBDebugPath();
};
extern mainProcess process;