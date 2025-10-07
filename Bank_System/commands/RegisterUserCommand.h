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
	void execute(const std::vector<std::string>& args, handleInfo& handle) override { // reg_user login password name surname phone //example: reg_user user1 pass1 John Doe 1234567890
        for (string a : args) {
			cout << a << " ";
        };
		cout << endl;
        if (args.size() < 6) {
            cout << "Недостатньо аргументів для reg_user!" << endl;
            return;
        }

        if (args[1].size() > 32 or args[2].size() > 32 or args[3].size() > 64) {
            cout << "some args are to large" << endl;
            return;
        };

        int phone;
        if (isPhone(args[5])) {
            phone = stoi(args[5]);
        }
        else {
            cout << "phone number is incorect" << endl;
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
			args[1].c_str(), // login
			args[2].c_str(), // password
			args[3].c_str(), // name
			args[4].c_str(), // surname
            phone,
			emptyPassport,
			0,
			USER_NONVERIFED,
            USER
        );
        DB_newUser(newUser);
		cout << "User " << newUserLogin << " registered!" << endl;


		strncpy(handle.sessionData.cmd, "User registered!", sizeof(handle.sessionData.cmd) - 1);
		handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
		WriteFile(handle.hPipe, &handle.sessionData, sizeof(handle.sessionData), &handle.bytesWritten, NULL);

    }

    std::string name() const override {
        return "reg_user";
    };
};
