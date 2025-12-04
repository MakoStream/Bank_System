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
//#include "basic_functions.h"
#include "LogEye.h"
#include "Transactions/Transactions.h"


/// @brief Represents a user session in the system
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
        if (key == "last_user_id") {
			last_user_id = std::stoi(value);
        };
        if (key == "last_audit_id") {
            last_audit_id = std::stoi(value);
        }
        if (key == "last_account_id") {
            last_account_id = std::stoi(value);
        };
        if (key == "last_transaction_id") {
            last_transaction_id = std::stoi(value);
        }
        if (key == "last_transaction_request_id") {
            last_transaction_request_id = std::stoi(value);
        }
        if (key == "account_db_path") {
            account_db_path = value;
        }
        if (key == "transaction_log_db_path") {
            transaction_log_db_path = value;
        }
        if (key == "audit_log_db_path") {
            audit_log_db_path = value;
        }
        else if (key == "user_db_path") {
            user_db_path = value;
        }
        debug = false;
    };

    
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

/**
 * @brief Saves current session and card counters to configuration file.
 *
 * @details Persists the current state of mainProcess including last session ID,
 * card counters, account, user, transaction log, and audit log paths.
 *
 * @note Uses writeConfig(), <map>, <string>.
 * @see writeConfig()
 */
void mainProcess::savecfg() {
    std::map<std::string, std::string> cfg;
    cfg["last_session_id"] = std::to_string(last_session_id);
    cfg["last_card_PAN"] = std::to_string(last_card_PAN);
    cfg["last_card_IBAN"] = std::to_string(last_card_IBAN);
	cfg["last_audit_id"] = std::to_string(last_audit_id);
	cfg["last_account_id"] = std::to_string(last_account_id);
	cfg["last_user_id"] = std::to_string(last_user_id);
	cfg["last_transaction_id"] = std::to_string(last_transaction_id);
	cfg["last_transaction_request_id"] = std::to_string(last_transaction_request_id);
    cfg["account_db_path"] = account_db_path;
    cfg["user_db_path"] = user_db_path;
	cfg["transaction_log_db_path"] = transaction_log_db_path;
	cfg["audit_log_db_path"] = audit_log_db_path;

    writeConfig(configName, cfg);
}

/**
 * @brief Prints all key-value pairs from the configuration file.
 * @details Reads the configuration and prints each key and value to the console.
 * @note Requirements: readConfig(), <map>, <iostream>
 */
void mainProcess::printConfig() {
	std::map<std::string, std::string> cfg = readConfig(configName);
	for (const auto& [key, value] : cfg) {
		std::cout << key << " = " << value << std::endl;
	}
}

/**
 * @brief Creates a new session, increments last_session_id and adds session to logged users.
 * @return int Returns the ID of the newly created session.
 * @note Requirements: savecfg(), vector<Session>
 */
int mainProcess::new_session() {
    last_session_id++;
    savecfg();
    loggined_users.push_back({last_session_id, -1});
    return last_session_id;
}

/**
 * @brief Logs in a user by setting user ID in the session structure.
 * @param session_id Session identifier.
 * @param login User login string.
 * @param password User password string.
 * @note Requirements: getUser_byLogin(), vector<Session>
 */
void mainProcess::login(int session_id, char login[32], char password[32]){
    User this_user = User::getUser_byLogin(login);
    Session user = { session_id, this_user.getId()};

    for (Session& a : loggined_users) {
        if (a.session_id == session_id) {
            a = user;
            break;
        };
    };
    return;
}

/**
 * @brief Logs out a session by resetting its user_id and auth_key.
 * @param session_id Session identifier to logout.
 * @note Requirements: vector<Session>
 */
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


/**
 * @brief Returns reference to a session by session_id, or emptySession if not found.
 * @param session_id Session identifier.
 * @return Session& Reference to session object.
 * @note Requirements: vector<Session>
 */
Session& mainProcess::getUserSession(int session_id) {
    for (Session& a : loggined_users) {
        if (a.session_id == session_id) {
            return a;
        }
    };
    return emptySession;
};


/**
 * @brief Prints all logged sessions to console.
 * @note Requirements: vector<Session>, <iostream>
 */
void mainProcess::printSessions() { // this doesnot work when debug on !!!!!
    for (Session a : loggined_users) {
        cout << a.session_id << " | " << a.user_id << " | " << a.auth_key << endl;
    };
};


/**
 * @brief Returns reference to a session by session_id, or emptySession if not found.
 * @param session_id Session identifier.
 * @return Session& Reference to session object.
 */
Session& mainProcess::getSessionByID(int session_id) {
    for (Session& a : loggined_users) {
        if (a.session_id == session_id) {
            return a;
        }
    }
    return emptySession;
}


/**
 * @brief Generates random 40-character auth_key for a session and copies it to sessionData.
 * @param session Session object to assign key.
 * @param sessionData sessionConstruct object to copy key.
 * @note Requirements: <random>, <cstring>, Session, sessionConstruct
 */
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

/**
 * @brief Compares auth_key from sessionConstruct with session.
 * @param sc sessionConstruct object.
 * @param s Session object.
 * @return true If keys match.
 * @return false Otherwise.
 * @note Requirements: Session, sessionConstruct, <cstring>
 */
bool mainProcess::compareAuthKey(const sessionConstruct& sc, const Session& s) {
    return std::strncmp(sc.auth_key, s.auth_key, 40) == 0;
}

/**
 * @brief Increments and returns the last PAN card counter.
 * @return int Updated PAN value.
 */
int mainProcess::incrementCardPAN() {
    last_card_PAN++;
    savecfg();
    return last_card_PAN;
}

/**
 * @brief Increments and returns the last IBAN card counter.
 * @return int Updated IBAN value.
 */
int mainProcess::incrementCardIBAN() {
    last_card_IBAN++;
    savecfg();
    return last_card_IBAN;
}

int mainProcess::incrementUserID() {
	last_user_id++;
	savecfg();
	return last_user_id;
}

/**
 * @brief Increments and returns the last account ID.
 * @return int Updated account ID.
 */
int mainProcess::incrementAccountID() {
    last_account_id++;
    savecfg();
    return last_account_id;
}

/**
 * @brief Increments and returns the last audit ID.
 * @return int Updated audit ID.
 */
int mainProcess::incrementAuditID() {
    last_audit_id++;
    savecfg();
    return last_audit_id;
}

/**
 * @brief Increments and returns the last transaction ID.
 * @return int Updated transaction ID.
 */
int mainProcess::incrementTransactionID() {
    last_transaction_id++;
    savecfg();
    return last_transaction_id;
}

/**
 * @brief Increments and returns the last transaction request ID.
 * @return int Updated transaction request ID.
 */
int mainProcess::incrementTransactionRequestID() {
    last_transaction_request_id++;
    savecfg();
    return last_transaction_request_id;
}

/**
 * @brief Returns account database path.
 * @return string Path string.
 */
string mainProcess::getAccountDBPath() { return account_db_path; }

/**
 * @brief Returns user database path.
 * @return string Path string.
 */
string mainProcess::getUserDBPath() { return user_db_path; }

/**
 * @brief Returns transaction log database path.
 * @return string Path string.
 */
string mainProcess::getTransactionLogDBPath() { return transaction_log_db_path; }

/**
 * @brief Returns audit log database path.
 * @return string Path string.
 */
string mainProcess::getAuditLogDBPath() { return audit_log_db_path; }

/**
 * @brief Returns vector of logged-in sessions.
 * @return vector<Session> Vector of sessions.
 */
vector<Session> mainProcess::getSessions() { return loggined_users; }


/**
 * @brief Enables debug mode and reloads configuration.
 * @return true Always returns true after setting debug mode.
 */
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
        if (key == "last_user_id") {
			last_user_id = std::stoi(value);
        };
        if (key == "last_card_IBAN") {
            last_card_IBAN = std::stoi(value);
        }
        if (key == "account_db_path") {
            account_db_path = value;
        }
        if (key == "transaction_log_db_path") {
            transaction_log_db_path = value;
        }
        if (key == "audit_log_db_path") {
            audit_log_db_path = value;
        }
        if (key == "last_audit_id") {
            last_audit_id = std::stoi(value);
        }
        if (key == "last_account_id") {
			last_account_id = std::stoi(value);
        };
		if (key == "last_transaction_id") {
			last_transaction_id = std::stoi(value);
		}
        if (key == "last_transaction_request_id") {
            last_transaction_request_id = std::stoi(value);
        }
        else if (key == "user_db_path") {
            user_db_path = value;
        };
    }
	loggined_users.clear();

    Transaction::stopTransactionThread();
	Transaction::startTransactionThread();

    return true;
}
/**
 * @brief Returns debug mode status.
 * @return true If debug mode is enabled.
 * @return false Otherwise.
 */
bool mainProcess::debugStatus() { return debug; };


