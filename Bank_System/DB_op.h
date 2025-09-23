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


extern std::unordered_map<std::string, cardType> cardMap;
extern std::unordered_map<std::string, balanceType> balanceMap;

// 1 dollar = 0.85 euro = 41.24 uah
// 1 euro = 1.17 dollar = 48.37 uah 
// 1 uah = 0.024 dollar = 0.021 euro


// Class: User
// Description: Represents a bank system user with credentials, balance, card info, and status
// Requirements: <string>, <fstream>, cardType, balanceType, cardStatus
// Required for: DB operations, commands, sessions
class User {  // Дані користувача
    int id;
    char login[32];
    char password[32];
    char name[64];
    int phone;
    cardType type = DEFAULT;
    balanceType balance_Type;
    float balance;
    cardStatus status = AVAILABLE;


public:

    User() {} // порожній конструктор для load

    User(int id, const char* login, const char* password, const char* name,
        int phone, cardType type, balanceType balance_Type, double balance)
        : id(id), type(type), balance_Type(balance_Type), balance(balance), status(AVAILABLE), phone(phone)
    {
        strncpy_s(this->login, login, sizeof(this->login) - 1);
        this->login[sizeof(this->login) - 1] = '\0';
        strncpy_s(this->password, password, sizeof(this->password) - 1);
        this->password[sizeof(this->password) - 1] = '\0';
        strncpy_s(this->name, name, sizeof(this->name) - 1);
        this->name[sizeof(this->name) - 1] = '\0';
    }

    // Базові ф-ції для виводу інформації
    int getId()const { return id; };
    double getBalance()const { return balance; };
    balanceType getBalanceType() const { return balance_Type; };
    const char* getName()const { return name; };
    const char* getLogin()const { return login; };
    string getCardTypeStr() const {
        switch (type) {
        case DEFAULT: return "DEFAULT";
        case DEPOSITE: return "DEPOSITE";
        case CREDIT: return "CREDIT";
        }
        return "UNKNOWN";
    }
    string getBalanceTypeStr() const {
        switch (balance_Type) {
        case UAH: return "UAH";
        case DLR: return "DLR";
        case EUR: return "EUR";
        }
        return "UNKNOWN";
    }
    string getStatusStr() const {
        switch (status) {
        case AVAILABLE: return "AVAILABLE";
        case BLOCKED: return "BLOCKED";
        }
        return "UNKNOWN";
    }
    int getPhone() const { return phone; }


    // Функція щоб разлогінити користувача з сессії
    int logout() {
        return 0;
    };

    // Функції для платіжних операцій
    int transaction(User user, double ammount) {

        return 0;
    };

    // Базові функції для керування статусом
    int ban() {
        status = BLOCKED;
        return 0;
    };
    int unban() {
        status = AVAILABLE;
        return 0;
    };

    // Для запису та считування даних з users.dat
    void save(ofstream& fout) const {
        fout.write(reinterpret_cast<const char*>(&id), sizeof(id));
        fout.write(login, sizeof(login));
        fout.write(password, sizeof(password));
        fout.write(name, sizeof(name));
        fout.write(reinterpret_cast<const char*>(&phone), sizeof(phone));
        fout.write(reinterpret_cast<const char*>(&type), sizeof(type));
        fout.write(reinterpret_cast<const char*>(&balance_Type), sizeof(balance_Type));
        fout.write(reinterpret_cast<const char*>(&balance), sizeof(balance));
        fout.write(reinterpret_cast<const char*>(&status), sizeof(status));
    }

    void load(ifstream& fin) {
        fin.read(reinterpret_cast<char*>(&id), sizeof(id));
        fin.read(login, sizeof(login));
        fin.read(password, sizeof(password));
        fin.read(name, sizeof(name));
        fin.read(reinterpret_cast<char*>(&phone), sizeof(phone));
        fin.read(reinterpret_cast<char*>(&type), sizeof(type));
        fin.read(reinterpret_cast<char*>(&balance_Type), sizeof(balance_Type));
        fin.read(reinterpret_cast<char*>(&balance), sizeof(balance));
        fin.read(reinterpret_cast<char*>(&status), sizeof(status));
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
