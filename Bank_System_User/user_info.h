#pragma once
#include "windows.h"


enum cardStatus { AVAILABLE, BLOCKED };
enum cardType { DEPOSITE, DEFAULT, CREDIT };
enum balanceType { UAH, DLR, EUR };



extern struct sessionConstruct {
    int sessionId = 0;
    int hash[10];
    char cmd[256];
    char msg[5][256];
};


extern struct fronted_User {
    char name[64];
    char login[32];
};

extern struct handleInfo {
    HANDLE& hPipe;
    sessionConstruct& sessionData;
    DWORD& bytesRead;
    DWORD& bytesWritten;
};