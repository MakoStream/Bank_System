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
#include "Audit/Audit.h"


extern string configName;

typedef struct Session {
    int session_id;
    int user_id;
    char auth_key[41];
};
struct sessionConstruct {
    int sessionId = -1;
    int userId = -1;
    int hash[10];
    char cmd[256];
    char auth_key[41];
	//vector <string> msg;
	char msg[10][1024];
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
	int last_audit_id;
	int last_account_id;
	int last_transaction_id;
	int last_transaction_request_id;

	// default db paths
	string account_db_path;
	string user_db_path;
    string transaction_log_db_path;
    string audit_log_db_path;
	bool debug;

    std::vector<Session> loggined_users;
    void savecfg();
public:

	mainProcess();
	// Session management
    int new_session();
	void setDebugMode(bool mode);
	void printConfig();
    void login(int session_id, char login[32], char password[32]);
	void logout(int session_id);
    void printSessions();
    Session& getUserSession(int session_id);
    void generateAuthKey(Session& session, sessionConstruct& sessionData);
    Session& getSessionByID(int session_id);
    bool compareAuthKey(const sessionConstruct& sc, const Session& s);
	void transferBridge(account& from, account& to, double amount);
    vector <Session> getSessions();

	// Audit management

	//increments();
	int incrementCardPAN();
	int incrementCardIBAN();
	int incrementAccountID();
	int incrementAuditID();
	int incrementTransactionID();
	int incrementTransactionRequestID();
	int getLastSessionID();
    
	// Getters for DB paths
	string getAccountDBPath();
	string getUserDBPath();
	string getTransactionLogDBPath();
	string getAuditLogDBPath();


	// Debug mode
	bool debugOn();
    bool debugStatus();


	void transaction_request(handleInfo handle, account& from, account& to, double ammount, const char* PIN, const char* CVV, operations op_type, string comment = "#");  // need args: handle info, from_account, to_account, amount
    void check_requests();

	void allow_transaction(int transaction_id, handleInfo& user_handle, string comment);
};


extern mainProcess process;


