#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <csignal>
#include "commands.h"
#include "DB_op.h"
#include <cctype> 
#include <algorithm> 
#include <unordered_map>


using namespace std;

// Function: isStringDigit
// Description: Checks if a string represents a valid floating point number
// Requirements: <string>, <cctype>
// Required for: input validation in commands::execute
bool isStringDigit(const std::string& s) {
    if (s.empty()) return false;
    int dotCount = 0;
    for (char c : s) {
        if (c == '.') {
            dotCount++;
            if (dotCount > 1) return false;
        }
        else if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Function: isPhone
// Description: Validates if a string is a 10-digit phone number
// Requirements: <string>, <cctype>
// Required for: commands::execute("reg_user")
bool isPhone(const std::string& s) {
    if (s.empty()) return false;
    if (s.size() != 10) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}

commands::commands() {}

// Function: commands::execute
// Description: Executes CLI commands like create_db, reg_user, user_list
// Requirements: DB_op functions, User, balanceMap, cardMap
// Required for: main() loop
void commands::execute(const vector<string>& args) {
    string command = args[0];
    // create_DB
    if (command == "create_db") { createDB(); }

    // reg_user
    else if (command == "reg_user") {  // reg_user <login> <password> <name> <phone> <cardType> <balanceType>
        if (args.size() < 7) {
            cout << "Недостатньо аргументів для reg_user!" << endl;
            return;
        }
        
        if (args[1].size() > 32 or args[2].size() > 32 or args[3].size() > 64) {
            cout << "some args are to large" << endl;
            return;
        };

        int phone;
        if (isPhone(args[4])) {
            phone = stoi(args[4]);
        }
        else {
            cout << "phone number is incorect" << endl;
            return;
        };

        // Конвертація рядка в enum cardType
        cardType newUserCardType;
        if (cardMap.find(args[5]) != cardMap.end()) {
            newUserCardType = cardMap[args[5]];
        }
        else {
            cout << "Невідомий тип карти!" << endl;
            return;
        }

        // Конвертація рядка в enum balanceType
        balanceType newUserBalanceType;
        if (balanceMap.find(args[6]) != balanceMap.end()) {
            newUserBalanceType = balanceMap[args[6]];
        }
        else {
            cout << "Невідома валюта!" << endl;
            return;
        };
        const char* newUserLogin = args[1].c_str();
        if (isUserExist_byLogin(newUserLogin)) {
            cout << newUserLogin << " already exist" << endl;
            return;
        };
        int newUserId = 0;

        User lastUser = loadLastUser();
        if (lastUser.getId() >= 0) {
            newUserId = lastUser.getId()+1;
        };
        User newUser(
            newUserId,
            args[1].c_str(),
            args[2].c_str(),
            args[3].c_str(),
            phone,
            newUserCardType,
            newUserBalanceType,
            0
        );
        regUser(newUser);
    }

    // user_list
    else if (command == "user_list") {
        DB_list();
    }
    return;

}

// Function: commands::regUser
// Description: Registers a new user and appends to database
// Requirements: DB_newUser(), User class
// Required for: execute("reg_user")
void commands::regUser(User user) {
    DB_newUser(user);
    cout << "Новий користувач створений: " << user.getName() << endl;
}



// Function: commands::createDB
// Description: Creates the database file
// Requirements: DB_create()
// Required for: execute("create_db")
void commands::createDB() {
    DB_create();
    return;
}