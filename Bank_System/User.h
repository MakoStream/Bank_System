/**
@file User.h
@brief Classes and structures for managing users in the banking system.
@details Contains passportData structure, User class, and database-related functions for user management.
*/

#define _CRT_SECURE_NO_WARNINGS
#pragma once
//#include "mainProcess.h"
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

/**
@struct passportData
@brief Passport information used for user verification.
*/
struct passportData {
	char series[2];
	int number;
	char issuedBy[64];
	char issuedDate[16];
	char birthDate[16];
};
extern passportData emptyPassport;


/**
@class User
@brief Represents a bank system user.
@details Stores credentials, passport data, tax ID, status, and role.
@note Requirements: <fstream>, userStatus, userRole
@note Required for: database operations, commands, sessions
*/
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
    /**
    @brief Default constructor for loading users from file.
    */
    User() {} // порожній конструктор для load

    /**
    @brief Constructs a User object with specified parameters.
    */
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

    /** @name Getters */
    ///@{
    int getId() const { return id; }                         ///< Returns user ID
    const char* getName() const { return name; }            ///< Returns user's first name
    const char* getSurname() const { return surname; }      ///< Returns user's surname
    const char* getLogin() const { return login; }          ///< Returns user's login
    int getPhone() const { return phone; }                  ///< Returns user's phone number
    int getTIN() const { return TIN; }                      ///< Returns user's tax ID
    userRole getRole() const { return role; }               ///< Returns user's role
    userStatus getStatus() const { return status; }         ///< Returns user's account status
    passportData getPassport() const { return passport; }   ///< Returns user's passport data
    ///@}



    /** @name Status management */
    ///@{
    void ban() { status = USER_BLOCKED; }                   ///< Sets user status to blocked
    void unban() { status = USER_AVAILABLE; }              ///< Sets user status to available
    ///@}

    /** @name Password verification */
    ///@{
    bool checkPassword(const char c_password[32]) { return std::strcmp(password, c_password) == 0; }
    ///< Checks if given password matches the user's password
    ///@}

    /** @name File operations */
    ///@{
    void save(ofstream& fout) const {                        ///< Writes user data to binary file
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

    void load(ifstream& fin) {                               ///< Reads user data from binary file
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

    void updateInFile();                                      ///< Updates this user object in the binary file
    ///@}
};

extern User emptyUser;


/**
@brief Reads and prints a page of users from "users.dat".
@details Loads users from the binary file and formats their information into a 5-element string array for display. Supports pagination.
@note Requirements: User class, <fstream>, <iostream>
@note Required for: commands::execute("user_list")
*/
void DB_list(char msg[5][1024], int page);

/**
@brief Creates an empty binary file "users.dat".
@details This function creates a new binary file to store user data. If the file already exists, it will be overwritten.
@note Requirements: <fstream>, <iostream>
@note Required for: commands::createDB()
*/
void DB_create();

/**
@brief Appends a User object to "users.dat".
@details This function writes a User object to the end of the binary file for persistent storage.
@note Requirements: User::save(), <fstream>
@note Required for: commands::regUser()
*/
void DB_newUser(User user);

/**
@brief Loads the last user from "users.dat".
@details Seeks to the end of the binary file and reads the last User object for further processing.
@note Requirements: User class, <fstream>, <iostream>
@note Required for: commands::regUser()
*/
User loadLastUser();


/**
@brief Checks if a login already exists in "users.dat".
@details Iterates through the binary user file and compares logins to prevent duplicates.
@note Requirements: User class, <fstream>, <cstring>
@note Required for: commands::execute("reg_user")
*/
bool isUserExist_byLogin(const char* login);

/**
@brief Retrieves a User object by login.
@details Searches the binary file for a user matching the specified login and returns the User object.
@note Requirements: User class, <fstream>, <cstring>
@note Required for: any function needing User data by login
*/
User getUser_byLogin(const char* login);

/**
@brief Checks if a user with a given ID exists in "users.dat".
@details Iterates through the binary user file and compares IDs.
@note Requirements: User class, <fstream>
@note Required for: any function needing user validation
*/
bool isUserExist_byId(int id);

/**
@brief Retrieves a User object by ID.
@details Searches the binary file for a user matching the specified ID and returns the User object.
@note Requirements: User class, <fstream>
@note Required for: any function needing User data by ID
*/
User getUser_byId(int id);
