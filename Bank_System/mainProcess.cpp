#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
//#include "Logger.h"
#include <csignal>
#include "DB_operations.h"
#include <optional>
#include "mainProcess.h"

Session emptySession = { -1, -1 };

int getNext_SId() {
    int s_id = 0;

    // Відкрити для читання
    std::ifstream in("session_id.bin", std::ios::binary);
    if (in.is_open()) {
        in.read(reinterpret_cast<char*>(&s_id), sizeof(s_id));
        in.close();
    }
    else {
        // якщо файл відсутній - створимо з 0
        std::ofstream newFile("session_id.bin", std::ios::binary);
        newFile.write(reinterpret_cast<const char*>(s_id), sizeof(s_id));
        newFile.close();
        s_id = 0;
    }

    // Змінюємо (наприклад, інкремент)
    s_id++;

    // Записати назад у файл
    std::ofstream out("session_id.bin", std::ios::binary | std::ios::trunc);
    if (!out) {
        throw std::runtime_error("Не вдалося відкрити файл для запису");
    }
    out.write(reinterpret_cast<const char*>(&s_id), sizeof(s_id));
    out.close();

    return s_id;
}

mainProcess::mainProcess(){
     
}

int mainProcess::new_session() {
    int session_id = getNext_SId();
    loggined_users.push_back({session_id, -1});
    return session_id;
}
void mainProcess::login(int session_id, char login[32], char password[32]){
    User this_user = getUser_byLogin(login);
    Session user = { session_id, this_user.getId()};

    for (Session& a : loggined_users) {
        if (a.sesion_id == session_id) {
            a = user;
            break;
        };
    };
    return;
}

Session& mainProcess::getUserSession(int session_id) {
    std::cout << "debug 4" << endl;
    for (Session& a : loggined_users) {
        if (a.sesion_id == session_id) {
            std::cout << "debug 5" << endl;
            return a;
        }
    };
    return emptySession;
};

void mainProcess::printSessions() {
    for (Session a : loggined_users) {
        cout << a.sesion_id << endl;
    };
};
