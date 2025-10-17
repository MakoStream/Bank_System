#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
//#include "Logger.h"
#include <csignal>
#include "User.h"
#include <optional>
#include "mainProcess.h"

Session emptySession = { -1, -1 };

int getNext_SId() {
    int s_id = 0;

    // ³������ ��� �������
    std::ifstream in("session_id.bin", std::ios::binary);
    if (in.is_open()) {
        in.read(reinterpret_cast<char*>(&s_id), sizeof(s_id));
        in.close();
    }
    else {
        // ���� ���� ������� - �������� � 0
        std::ofstream newFile("session_id.bin", std::ios::binary);
        newFile.write(reinterpret_cast<const char*>(s_id), sizeof(s_id));
        newFile.close();
        s_id = 0;
    }

    // ������� (���������, ���������)
    s_id++;

    // �������� ����� � ����
    std::ofstream out("session_id.bin", std::ios::binary | std::ios::trunc);
    if (!out) {
        throw std::runtime_error("�� ������� ������� ���� ��� ������");
    }
    out.write(reinterpret_cast<const char*>(&s_id), sizeof(s_id));
    out.close();

    return s_id;
}

mainProcess::mainProcess(){
    std::map<std::string, std::string> cfg = readConfig("config.ini");
	for (const auto& [key, value] : cfg) {
        if (key == "last_session_id") {
			last_session_id = std::stoi(value);
        };
		if (key == "last_card_PAN") {
			last_card_PAN = std::stoi(value);
		}
		if (key == "last_card_IBAN") {
			last_card_IBAN = std::stoi(value);
		}
		if (key == "account_db_path") {
			account_db_path = value;
		}
		else if (key == "user_db_path") {
			user_db_path = value;
		}
		else if (key == "account_db_debug_path") {
			account_db_debug_path = value;
		}
		else if (key == "user_db_debug_path") {
			user_db_debug_path = value;
		}
        debug = false;
	}
}

void mainProcess::savecfg() {
    std::map<std::string, std::string> cfg;
    cfg["last_session_id"] = std::to_string(last_session_id);
    cfg["last_card_PAN"] = std::to_string(last_card_PAN);
    cfg["last_card_IBAN"] = std::to_string(last_card_IBAN);
    cfg["account_db_path"] = account_db_path;
    cfg["user_db_path"] = user_db_path;
    cfg["account_db_debug_path"] = account_db_debug_path;
    cfg["user_db_debug_path"] = user_db_debug_path;

    writeConfig("config.ini", cfg);
}

void mainProcess::printConfig() {
	std::map<std::string, std::string> cfg = readConfig("config.ini");
	for (const auto& [key, value] : cfg) {
		std::cout << key << " = " << value << std::endl;
	}
}

int mainProcess::new_session() {
    last_session_id++;
    savecfg();
    loggined_users.push_back({last_session_id, -1});
    return last_session_id;
}
void mainProcess::login(int session_id, char login[32], char password[32]){
    User this_user = getUser_byLogin(login);
    Session user = { session_id, this_user.getId()};

    for (Session& a : loggined_users) {
        if (a.session_id == session_id) {
            a = user;
            break;
        };
    };
    return;
}

Session& mainProcess::getUserSession(int session_id) {
    for (Session& a : loggined_users) {
        if (a.session_id == session_id) {
            return a;
        }
    };
    return emptySession;
};

void mainProcess::printSessions() {
    for (Session a : loggined_users) {
        cout << a.session_id << " | " << a.user_id << " | " << a.auth_key << endl;
    };
};

Session& mainProcess::getSessionByID(int session_id) {
    for (Session& a : loggined_users) {
        if (a.session_id == session_id) {
            return a;
        }
    }
    return emptySession;
}
void mainProcess::generateAuthKey(Session& session, sessionConstruct& sessionData) {
    static std::mt19937 generator(std::random_device{}());
    static const char charset[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::uniform_int_distribution<int> dist(0, sizeof(charset) - 2);

    // ���������� ���� � session
    for (int i = 0; i < 40; ++i)
        session.auth_key[i] = charset[dist(generator)];
    session.auth_key[40] = '\0';

    // ������� ���� � sessionData
    std::strcpy(sessionData.auth_key, session.auth_key);
    return;
}

bool mainProcess::compareAuthKey(const sessionConstruct& sc, const Session& s) {
    return std::strncmp(sc.auth_key, s.auth_key, 40) == 0;
}

void mainProcess::transferBridge(account& from, account& to, double amount) {
	from.transfer(to, amount);
}
int mainProcess::incrementCardPAN() {
	last_card_PAN++;
	savecfg();
	return last_card_PAN;
}
int mainProcess::incrementCardIBAN() {
	last_card_IBAN++;
	savecfg();
	return last_card_IBAN;
}

string mainProcess::getAccountDBPath() {return account_db_path;}
string mainProcess::getUserDBPath() { return user_db_path; }
string mainProcess::getAccountDBDebugPath() { return account_db_debug_path; }
string mainProcess::getUserDBDebugPath() { return user_db_debug_path; }