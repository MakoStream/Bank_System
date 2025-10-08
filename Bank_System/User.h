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


enum userStatus { USER_AVAILABLE, USER_BLOCKED, USER_NONVERIFED};
enum userRole { USER, ADMIN };


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
class User {  // Дані користувача
    int id;
    char login[32];
    char password[32];
    char name[64];
    char surname[64];
    int phone;
	passportData passport;                  // додаткові дані для верифікації
	int TIN;                                // ідентифікаційний номер платника податків
	userStatus status = USER_NONVERIFED;
	userRole role = USER;

public:

    User() {} // порожній конструктор для load

    // Конструктор
	User(int _id, const char* _login, const char* _password, const char* _name, const char* _surname, int _phone, passportData _passport, int _TIN,
        userStatus _status = USER_AVAILABLE, userRole _role = USER)
		: id(_id), phone(_phone), passport(_passport), TIN(_TIN), status(_status), role(_role)
    {
        strncpy(login, _login, sizeof(login) - 1);
        login[sizeof(login) - 1] = '\0';
        strncpy(password, _password, sizeof(password) - 1);
        password[sizeof(password) - 1] = '\0';
        strncpy(name, _name, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0';
        strncpy(surname, _surname, sizeof(surname) - 1);
        surname[sizeof(surname) - 1] = '\0';
    }

    // Базові ф-ції для виводу інформації
    int getId()const { return id; };
    const char* getName()const { return name; };
    const char* getSurname()const { return surname; };
    const char* getLogin()const { return login; };
    int getPhone() const { return phone; }

    // Базові функції для керування статусом
    void ban() {status = USER_BLOCKED;};
    void unban() {status = USER_AVAILABLE;};

    // функція для перевірки правильності паролю
    bool checkPassword(const char c_password[32]) { return std::strcmp(password, c_password) == 0;};

    // Для запису та считування даних з users.dat
    void save(ofstream& fout) const {
        fout.write(reinterpret_cast<const char*>(&id), sizeof(id));
        fout.write(login, sizeof(login));
        fout.write(password, sizeof(password));
        fout.write(name, sizeof(name));
        fout.write(surname, sizeof(surname));
        fout.write(reinterpret_cast<const char*>(&phone), sizeof(phone));
        fout.write(reinterpret_cast<const char*>(&passport), sizeof(passport));
        fout.write(reinterpret_cast<const char*>(&TIN), sizeof(TIN));
        fout.write(reinterpret_cast<const char*>(&status), sizeof(status));
        fout.write(reinterpret_cast<const char*>(&role), sizeof(role));
    }

    void load(ifstream& fin) {
        fin.read(reinterpret_cast<char*>(&id), sizeof(id));
        fin.read(login, sizeof(login));
        fin.read(password, sizeof(password));
        fin.read(name, sizeof(name));
        fin.read(surname, sizeof(surname));
        fin.read(reinterpret_cast<char*>(&phone), sizeof(phone));
        fin.read(reinterpret_cast<char*>(&passport), sizeof(passport));
        fin.read(reinterpret_cast<char*>(&TIN), sizeof(TIN));
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
