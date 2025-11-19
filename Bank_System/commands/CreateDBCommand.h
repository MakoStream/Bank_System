#pragma once
#include "../mainProcess.h"
#include "../Logger.h"
#include "../User.h"
#include "../Command.h"
#include <iostream>
#include "../Audit/Audit.h"
#include "../LogEye.h"

/**
 * @class CreateDBCommand
 * @brief Creates specified database tables.
 * @details Supports creation of users, accounts, transactions, and audit databases.
 * Logs all actions via logEye.
 * @note Requires: <string>, handleInfo, logEye
 * @note Syntax: create_db <users|accounts|transactions|audit>
 */
class CreateDBCommand : public Command {
public:
    /**
 * @brief Executes the database creation command.
 *
 * @details
 * Creates one of the supported database structures based on the argument
 * provided by the user. The command can create the following databases:
 * - users
 * - accounts
 * - transactions
 * - audit
 *
 * The method:
 * - Logs the operation start and session details
 * - Parses the command into arguments
 * - Checks the requested database type
 * - Invokes the corresponding database creation function
 * - Sends an appropriate response back to the client
 * - Sets handle.sessionData.hash[0] to 1 on success (0 on failure)
 * - Finalizes logging with SUCCESS or FAILURE status
 *
 * @param handle Reference to handleInfo containing session data,
 *               the raw command string, and the hash array used to report
 *               command execution status.
 *
 * @throws Throws a response via throw_response() when:
 *         - The provided database type is unknown
 *
 * @note
 * Side effects:
 * - Logs traces via logEye
 * - Modifies handle.sessionData.hash[0]
 * - Creates physical database files/tables depending on the option
 * - Sends session/network output through throw_response()
 *
 * @retval void Does not return a value directly.
 *         Command result is communicated through handle.sessionData.hash[0]:
 *         - 0 on failure
 *         - 1 on successful database creation
 */
    void execute(handleInfo& handle) override {
		int log_id = logEye.logTrace("create_db Command");
        logEye.msgTrace(log_id, "Session Id", to_string(handle.sessionData.sessionId), true);
        logEye.msgTrace(log_id, "User Id", to_string(process.getUserSession(handle.sessionData.sessionId).user_id), true);
		logEye.msgTrace(log_id, "input data", string(handle.sessionData.cmd), true);

        string input(handle.sessionData.cmd);
        vector<string> args = split(input);
		handle.sessionData.hash[0] = 0; // fail by default

		logEye.commentTrace(log_id, "Check input requirement");
        if (args[1] == "users") {
            DB_create();
            //std::cout << "База даних створена!\n" 
            throw_response(handle, "User database created!");
			logEye.endTrace(log_id, SUCCESS, "User database created successfully");
            return;
        }
        else if (args[1] == "accounts") {
            DB_create_accounts();
            //std::cout << "База даних створена!\n" 
            throw_response(handle, "Accounts database created!");
			logEye.endTrace(log_id, SUCCESS, "Accounts database created successfully");
            return;
        }
        else if (args[1] == "transactions") {
            create_db_transaction_log();
            throw_response(handle, "Transactions database created!");
			logEye.endTrace(log_id, SUCCESS, "Transactions database created successfully");
            return;
		}
		else if (args[1] == "audit") {
			create_db_audit_log();
			throw_response(handle, "Audit database created!");
			logEye.endTrace(log_id, SUCCESS, "Audit database created successfully");
			return;
		}
		else {
			throw_response(handle, "Unknown database type!");
			logEye.endTrace(log_id, FAILURE, "Unknown database type");
			return;
        };
        
    }

    std::string name() const override {
        return "create_db";
    }
};