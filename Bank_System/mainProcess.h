/**
@file mainProcess.h
@brief Declares mainProcess class for session, user, transaction, and configuration management.
@details Provides functions to manage sessions, login/logout, generate authentication keys,
handle transactions, debug mode, and persistence of counters in configuration files.
@note Required headers: <windows.h>, <string>, <vector>, <fstream>, <sstream>, <random>, Account.h, basic_functions.h, Audit/Audit.h, Logger.h
*/

#define _CRT_SECURE_NO_WARNINGS
#pragma once
//#include <iostream>
//#include "Command.h"
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
#include <csignal>
#include <unordered_map>
#include <random>
#include "Account.h"
#include "basic_functions.h"
//#include "Command.h"
#include "Audit/Audit.h"


extern string configName;

/**
@brief Represents a user session.
*/
typedef struct Session {
	int session_id; /**< Unique session identifier */
	int user_id; /**< Associated user ID */
	char auth_key[41]; /**< Authentication key (40 chars + null terminator) */
} Session;



/**
 @brief Structure for client session data used in pipes.
 */
struct sessionConstruct {
	int sessionId = -1; /**< Unique session ID */
	int userId = -1; /**< Associated user ID */
	int hash[10]; /**< Hash array (unused/optional) */
	char cmd[256]; /**< Command string sent by client */
	char auth_key[41]; /**< Authentication key (40 chars + null terminator) */
	char msg[10][1024]; /**< Array of messages for client */
};


/**
 @brief Helper structure to pass pipe handles and session data.
 */
struct handleInfo {
	HANDLE& hPipe; /**< Named pipe handle */
	sessionConstruct& sessionData; /**< Session data structure */
	DWORD& bytesRead; /**< Number of bytes read from pipe */
	DWORD& bytesWritten; /**< Number of bytes written to pipe */
};

extern Session emptySession;

/**
 @class mainProcess
 @brief Main class for session, user, transaction, and configuration management.
 @details The mainProcess class handles:
 - User session management (create, login, logout, generate authentication keys)
 - Database operations (file paths, incrementing PAN, IBAN, transaction IDs)
 - Transaction handling and logging via Audit
 - Debug mode management
 - Reading and writing configuration files
@note Requires: readConfig(), writeConfig(), <vector>, <string>, <map>, Logger, sessionConstruct, Session, User, account
@required For all core program operations (CommandsManager, HandleClient, userLoginCommand, transaction_request, etc.)
@used For:
 - Processing client sessions
 - User login/logout
 - Creating and approving transactions
 - Configuration file management
 */
class mainProcess {
    int last_session_id; /**< Last session identifier */
    int last_card_PAN; /**< Last card PAN */
    int last_card_IBAN; /**< Last card IBAN */
    int last_audit_id; /**< Last audit record ID */
    int last_account_id; /**< Last account ID */
    int last_transaction_id; /**< Last transaction ID */
    int last_transaction_request_id; /**< Last transaction request ID */

    std::string account_db_path; /**< Account database file path */
    std::string user_db_path; /**< User database file path */
    std::string transaction_log_db_path; /**< Transaction log file path */
    std::string audit_log_db_path; /**< Audit log file path */
    bool debug; /**< Debug mode flag */

    std::vector<Session> loggined_users; /**< Vector of active sessions */

    void savecfg(); /**< Saves current counters to configuration file */

public:
    /**
    * @brief Initializes mainProcess by reading configuration values from configName.
    * @details Reads the configuration file specified by configName and loads
    * last session ID, card counters, database paths, and other persistent variables.
    * @note Uses readConfig() and <map>, <string>, <iostream>.
    * @see readConfig()
    */
    mainProcess();

    // Session management
    int new_session(); /**< Creates a new session, adds to logged users, returns session ID */
    void setDebugMode(bool mode); /**< Sets debug mode */
    void printConfig(); /**< Prints all key-value pairs from configuration */
    void login(int session_id, char login[32], char password[32]); /**< Logs in a user into a session */
    void logout(int session_id); /**< Logs out a session */
    void printSessions(); /**< Prints all active sessions */
    Session& getUserSession(int session_id); /**< Returns reference to session by ID */
    void generateAuthKey(Session& session, sessionConstruct& sessionData); /**< Generates a 40-character auth_key */
    Session& getSessionByID(int session_id); /**< Returns session by ID */
    bool compareAuthKey(const sessionConstruct& sc, const Session& s); /**< Compares session auth keys */
    void transferBridge(account& from, account& to, double amount); /**< Performs internal transfer */
    std::vector<Session> getSessions(); /**< Returns all sessions */

    // Audit management
    int incrementCardPAN(); /**< Increments PAN counter and saves config */
    int incrementCardIBAN(); /**< Increments IBAN counter and saves config */
    int incrementAccountID(); /**< Increments account ID and saves config */
    int incrementAuditID(); /**< Increments audit ID and saves config */
    int incrementTransactionID(); /**< Increments transaction ID and saves config */
    int incrementTransactionRequestID(); /**< Increments transaction request ID and saves config */
    int getLastSessionID(); /**< Returns last session ID */

    // Database paths
    std::string getAccountDBPath(); /**< Returns account database path */
    std::string getUserDBPath(); /**< Returns user database path */
    std::string getTransactionLogDBPath(); /**< Returns transaction log file path */
    std::string getAuditLogDBPath(); /**< Returns audit log file path */

    // Debug mode
    bool debugOn(); /**< Enables debug mode and reloads configuration */
    bool debugStatus(); /**< Returns debug mode status */

    // Transactions
    void transaction_request(handleInfo handle, account& from, account& to, double ammount, const char* PIN, const char* CVV, operations op_type, std::string comment = "#"); /**< Creates a transaction request */
    void check_requests(); /**< Checks all pending transaction requests */
    void allow_transaction(int transaction_id, handleInfo& user_handle, std::string comment); /**< Approves a transaction request */
};

extern mainProcess process;


