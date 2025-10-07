#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
#include <csignal>
#include <unordered_map>

using namespace std;

// Enums: cardStatus, cardType, balanceType
// Description: Enumerations for card availability, card types, and balance currencies
// Requirements: none
// Required for: User class, commands, DB operations
enum cardStatus { AVAILABLE, BLOCKED };
enum cardType { DEPOSITE, DEFAULT, CREDIT };
enum balanceType { UAH, DLR, EUR };
enum userStatus { USER_AVAILABLE, USER_BLOCKED, USER_NONVERIFED};
enum userRole { USER, ADMIN };


extern std::unordered_map<std::string, cardType> cardMap;
extern std::unordered_map<std::string, balanceType> balanceMap;

// 1 dollar = 0.85 euro = 41.24 uah
// 1 euro = 1.17 dollar = 48.37 uah 
// 1 uah = 0.024 dollar = 0.021 euro


struct passportData {
	char series[2];
	int number;
	char issuedBy[64];
	char issuedDate[16];
	char birthDate[16];
};
extern passportData emptyPassport;


// Class: User
// Description: Represents a bank system user with credentials, balance, card info, and status
// Requirements: <string>, <fstream>, userStatus, userRole
// Required for: DB operations, commands, sessions
class User {  // ��� �����������
    int id;
    char login[32];
    char password[32];
    char name[64];
    int phone;
	passportData passport;                  // �������� ��� ��� �����������
	int TIN;                                // ���������������� ����� �������� �������
	userStatus status = USER_NONVERIFED;
	userRole role = USER;

public:

    User() {} // ������� ����������� ��� load

    // �����������
	User(int _id, const char* _login, const char* _password, const char* _name, int _phone, passportData _passport, int _TIN,
        userStatus _status = USER_AVAILABLE, userRole _role = USER)
		: id(_id), phone(_phone), status(_status), role(_role), TIN(_TIN), passport(_passport)
    {
        strncpy(login, _login, sizeof(login) - 1);
        login[sizeof(login) - 1] = '\0';
        strncpy(password, _password, sizeof(password) - 1);
        password[sizeof(password) - 1] = '\0';
        strncpy(name, _name, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
    }

    // ����� �-��� ��� ������ ����������
    int getId()const { return id; };
    const char* getName()const { return name; };
    const char* getLogin()const { return login; };
    int getPhone() const { return phone; }

    // ������� ��� �������� ��������
    int transaction(User user, double ammount) {
        return 0;
    };

    // ����� ������� ��� ��������� ��������
    void ban() {status = USER_BLOCKED;};
    void unban() {status = USER_AVAILABLE;};

    // ������� ��� �������� ����������� ������
    bool checkPassword(const char c_password[32]) { return std::strcmp(password, c_password) == 0;};

    // ��� ������ �� ���������� ����� � users.dat
    void save(ofstream& fout) const {
        fout.write(reinterpret_cast<const char*>(&id), sizeof(id));
        fout.write(login, sizeof(login));
        fout.write(password, sizeof(password));
        fout.write(name, sizeof(name));
        fout.write(reinterpret_cast<const char*>(&phone), sizeof(phone));
        fout.write(reinterpret_cast<const char*>(&status), sizeof(status));
		fout.write(reinterpret_cast<const char*>(&role), sizeof(role));
    }

    void load(ifstream& fin) {
        fin.read(reinterpret_cast<char*>(&id), sizeof(id));
        fin.read(login, sizeof(login));
        fin.read(password, sizeof(password));
        fin.read(name, sizeof(name));
        fin.read(reinterpret_cast<char*>(&phone), sizeof(phone));
        fin.read(reinterpret_cast<char*>(&status), sizeof(status));
		fin.read(reinterpret_cast<char*>(&role), sizeof(role));
    }

};

extern User emptyUser;

void DB_list();
void DB_create();
void DB_newUser(User user);
User loadLastUser();
bool isUserExist_byLogin(const char* login);
User getUser_byLogin(const char* login);
bool isUserExist_byId(int id);
User getUser_byId(int id);
