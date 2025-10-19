#pragma once
#include "../Command.h"
#include "../User.h"
#include <iostream>




class RegisterUserCommand : public Command {
public:
	void execute(handleInfo& handle) override { // reg_user login password name surname phone //example: reg_user user1 pass1 John Doe 1234567890
        string input (handle.sessionData.cmd);
		vector<string> args = split(input);

		handle.sessionData.hash[0] = 0; // default fail
		cout << endl;
        if (args.size() < 6) {
			throw_response(handle, "not enough arguments for reg_user");
			return;
        }

        if (args[1].size() > 32 or args[2].size() > 32 or args[3].size() > 64) {
			throw_response(handle, "one of the arguments is too long");
            return;
        };

        int phone;
        if (isPhone(args[5])) {
            phone = stoi(args[5]);
        }
        else {
			throw_response(handle, "phone number is invalid");
            return;
        };


        const char* newUserLogin = args[1].c_str();
        if (isUserExist_byLogin(newUserLogin)) {
			throw_response(handle, "user with this login already exists");
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

		handle.sessionData.hash[0] = 1; // success
		throw_response(handle, "user registered successfully");
		return;

    }

    std::string name() const override {
        return "reg_user";
    };
};
