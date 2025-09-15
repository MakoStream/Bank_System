#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
#include "commands.h"
#include <csignal>
#include "DB_op.h"
#include <optional>

using namespace std;

unordered_map<string, balanceType> balanceMap = {
    {"UAH", UAH},
    {"DLR", DLR},
    {"EUR", EUR}
};
unordered_map<string, cardType> cardMap = {
            {"DEFAULT", DEFAULT},
            {"DEPOSITE", DEPOSITE},
            {"CREDIT", CREDIT}
};


void DB_create() {
    ofstream file("users.dat", ios::binary); // ��������� ����� ����
    if (!file) {
        cerr << "������� ��������� �����!" << endl;
        return;
    };
    cout << "�������� ���� users.dat �������� ������." << endl;
    file.close();
};

void DB_newUser(User user) {
    ofstream fout("users.dat", ios::binary | ios::app);
    user.save(fout);
    fout.close();
};

void DB_list() {
    ifstream fin("users.dat", ios::binary);
    if (!fin) {
        cerr << "�� ������� ������� ���� users.dat!" << endl;
        return;
    }

    User u;
    while (fin.read(reinterpret_cast<char*>(&u), sizeof(User))) {
        // ���� ��� ���� � ���� �����
        cout
            << u.getId() << " "
            << u.getLogin() << " "
            << u.getName() << " "
            << u.getPhone() << " "
            << u.getCardTypeStr() << " "
            << u.getBalanceTypeStr() << " "
            << u.getBalance() << " "
            << u.getStatusStr()
            << endl;
    }

    fin.close();
}

User loadLastUser() {
    std::ifstream fin("users.dat", std::ios::binary);
    User empty; // ������ ����������, ���� ���� �������

    if (!fin) {
        std::cerr << "�� ������� ������� ����!" << std::endl;
        return empty;
    }

    fin.seekg(0, std::ios::end);
    std::streampos size = fin.tellg();
    if (size == 0) {
        return empty; // ���� �������
    }

    User last;
    fin.seekg(-static_cast<std::streamoff>(sizeof(User)), std::ios::end);
    fin.read(reinterpret_cast<char*>(&last), sizeof(User));
    return last;
}

bool isUserExist_byLogin(const char* login) {
    ifstream fin("users.dat", ios::binary);
    User u;
    while (fin.read(reinterpret_cast<char*>(&u), sizeof(User))) {
        if (strcmp(u.getLogin(), login) == 0) { // ��������� ���� �����
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
};