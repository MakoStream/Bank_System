#pragma once
#include "../Command.h"
#include "../DB_operations.h"
#include <iostream>

inline bool isStringDigit(const std::string& s) {
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
inline bool isPhone(const std::string& s) {
    if (s.empty()) return false;
    if (s.size() != 10) return false;
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return true;
}


class RegisterUserCommand : public Command {
public:
    void execute(const std::vector<std::string>& args, handleInfo& handle) override {
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
            newUserId = lastUser.getId() + 1;
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
        DB_newUser(newUser);

    }

    std::string name() const override {
        return "reg_user";
    };
};
