#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
//#include "commands.h"
#include <csignal>
#include "User.h"
#include <optional>
#include "mainProcess.h"

using namespace std;




// Function: DB_create
// Description: Creates an empty binary file "users.dat"
// Requirements: <fstream>, <iostream>
// Required for: commands::createDB()
void DB_create() {
    ofstream file(process.getUserDBPath(), ios::binary); // ��������� ����� ����
    if (!file) {
        cerr << "������� ��������� �����!" << endl;
        return;
    };
    cout << "�������� ���� "<< process.getUserDBPath() << " �������� ������." << endl;
    file.close();
}

// Function: DB_newUser
// Description: Appends a User object to "users.dat"
// Requirements: User::save(), <fstream>
// Required for: commands::regUser()
void DB_newUser(User user) {
    ofstream fout(process.getUserDBPath(), ios::binary | ios::app);
    user.save(fout);
    fout.close();
}

// Function: DB_list
// Description: Reads and prints all users from "users.dat"
// Requirements: User class, <fstream>, <iostream>
// Required for: commands::execute("user_list")
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
using namespace std;

// �������, ��� ��������:
// class User { ...; void load(ifstream&); int getId(); string getLogin(); string getName(); string getSurname(); string getPhone(); }

void DB_list(char msg[5][1024], int page) {
    ifstream fin(process.getUserDBPath(), ios::binary);
    if (!fin) {
        cerr << "�� ������� ������� ���� users.dat!" << endl;
        for (int i = 0; i < 5; i++) msg[i][0] = '\0';
        return;
    }

    User u;
    int startIndex = (page - 1) * 25;
    int currentIndex = 0;
    int msgIndex = 0;
    string buffer;

    while (fin.peek() != EOF) {
        u.load(fin);
        if (fin.gcount() == 0) break;

        if (currentIndex < startIndex) {
            currentIndex++;
            continue;
        }

        stringstream ss;
        ss << u.getId() << " "
            << u.getLogin() << " "
            << u.getName() << " "
            << u.getSurname() << " "
            << u.getPhone() << "\n";

        buffer += ss.str();
        currentIndex++;

        if (currentIndex % 5 == 0 || buffer.size() > 900) {
            strncpy(msg[msgIndex], buffer.c_str(), 1023);
            msg[msgIndex][1023] = '\0';
            buffer.clear();
            msgIndex++;
            if (msgIndex >= 5) break;
        }
    }

    if (!buffer.empty() && msgIndex < 5) {
        strncpy(msg[msgIndex], buffer.c_str(), 1023);
        msg[msgIndex][1023] = '\0';
        msgIndex++;
    }

    for (; msgIndex < 5; msgIndex++) msg[msgIndex][0] = '\0';

    fin.close();
}


// Function: loadLastUser
// Description: Loads the last user from "users.dat"
// Requirements: User class, <fstream>, <iostream>
// Required for: commands::regUser()
User loadLastUser() {
    ifstream fin(process.getUserDBPath(), ios::binary);
    User empty;

    if (!fin) {
        cerr << "�� ������� ������� ����!" << endl;
        return empty;
    }

    fin.seekg(0, ios::end);
    if (fin.tellg() == 0) return empty;

    User last;
    fin.seekg(-static_cast<streamoff>(sizeof(User)), ios::end);
    fin.read(reinterpret_cast<char*>(&last), sizeof(User));
    return last;
}

// Function: isUserExist_byLogin
// Description: Checks if a login already exists in "users.dat"
// Requirements: User class, <fstream>, <cstring>
// Required for: commands::execute("reg_user")
bool isUserExist_byLogin(const char* login) {
    ifstream fin(process.getUserDBPath(), ios::binary);
    User u;
    while (fin.read(reinterpret_cast<char*>(&u), sizeof(User))) {
        if (strcmp(u.getLogin(), login) == 0) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}


bool isUserExist_byId(int id) {
    ifstream fin("users.dat", ios::binary);
    User u;
    while (fin.read(reinterpret_cast<char*>(&u), sizeof(User))) {
        if (u.getId() == id) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}


User getUser_byLogin(const char* login) {
    ifstream fin(process.getUserDBPath(), ios::binary);
    User u;
    while (fin.read(reinterpret_cast<char*>(&u), sizeof(User))) {
        if (strcmp(u.getLogin(), login) == 0) {
            fin.close();
            return u;
        }
    }
    fin.close();
    return emptyUser;
}

User getUser_byId(int id) {
    ifstream fin(process.getUserDBPath(), ios::binary);
    User u;
    while (fin.read(reinterpret_cast<char*>(&u), sizeof(User))) {
        if (u.getId()==id) {
            fin.close();
            return u;
        }
    }
    fin.close();
    return emptyUser;
}

void User::updateInFile() {
    // 1. ������� �� ������ � �����
    std::ifstream inFile(process.getUserDBPath(), std::ios::binary);
    if (!inFile) {
        std::cerr << "�������: �� ������� ������� ���� users.dat ��� �������." << std::endl;
        return;
    }

    std::vector<User> all;
    User temp;

    while (true) {
        temp.load(inFile);
        if (inFile.eof()) break;
        all.push_back(temp);
    }
    inFile.close();

    // 2. ��������� �������� �����
    bool found = false;
    for (auto& user : all) {
        if (user.getId() == this->getId()) {
            user = *this; // ��������� ���� ��'���
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "������������: ���������� �� ����� ID �� ���������." << std::endl;
        return;
    }

    // 3. ������������ ����
    std::ofstream outFile("users.dat", std::ios::binary | std::ios::trunc);
    if (!outFile) {
        std::cerr << "�������: �� ������� ������� ���� users.dat ��� ������." << std::endl;
        return;
    }

    for (auto& user : all)
        user.save(outFile);

    outFile.close();
}
