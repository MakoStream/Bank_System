#pragma once
#include "windows.h"
#include <vector>
#include <string>

using namespace std;


enum cardStatus { AVAILABLE, BLOCKED };
enum cardType { DEPOSITE, DEFAULT, CREDIT };
enum balanceType { UAH, DLR, EUR };



extern struct sessionConstruct {
    int sessionId = -1;
    int userId = -1;
    int hash[10];
    char cmd[256];
    char auth_key[41];
    //vector <string> msg;
	char msg[10][1024];
};


extern struct fronted_User {
    char name[64];
	char surname[64];
    char login[32];
	vector<int> accounts_id;
};

extern struct handleInfo {
    HANDLE hPipe;
    sessionConstruct& sessionData;
    DWORD bytesRead;
    DWORD bytesWritten;
};

extern fronted_User currentUser;
extern fronted_User emptyUser;
