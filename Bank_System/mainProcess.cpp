#define _CRT_SECURE_NO_WARNINGS
#include "mainProcess.h"
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



Session emptySession = { -1, -1, ""};

mainProcess::mainProcess(){
	cout << configName << endl;
    std::map<std::string, std::string> cfg = readConfig(configName);
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
        debug = false;
	}
}
int getNext_SId() {  // USELESS: must be deleted later
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

void mainProcess::savecfg() {
    std::map<std::string, std::string> cfg;
    cfg["last_session_id"] = std::to_string(last_session_id);
    cfg["last_card_PAN"] = std::to_string(last_card_PAN);
    cfg["last_card_IBAN"] = std::to_string(last_card_IBAN);
    cfg["account_db_path"] = account_db_path;
    cfg["user_db_path"] = user_db_path;

    writeConfig("config.ini", cfg);
}

void mainProcess::printConfig() {
	std::map<std::string, std::string> cfg = readConfig(configName);
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

void mainProcess::logout(int session_id) {
    for (auto& it : loggined_users) {
		if (it.session_id == session_id) {
			it.user_id = -1;
			it.auth_key[0] = '\0';
			break;
		};

    };
	return;
};

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

    // Заповнюємо ключ у session
    for (int i = 0; i < 40; ++i)
        session.auth_key[i] = charset[dist(generator)];
    session.auth_key[40] = '\0';

    // Копіюємо ключ у sessionData
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
vector <Session> mainProcess::getSessions() {
    return loggined_users;
};

bool mainProcess::debugOn() {
	debug = true;
	configName = "config_debug.ini";
    cout << configName << endl;
    std::map<std::string, std::string> cfg = readConfig(configName);
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
        };
    }
	loggined_users.clear();

    return true;
}

bool mainProcess::debugStatus() { return debug; };