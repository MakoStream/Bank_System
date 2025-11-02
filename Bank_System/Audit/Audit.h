#pragma once
#include "../Account.h"
#include <fstream>
#include <cstring>
#include "../basic_functions.h"

enum operations {
    COMMISION,                // комісія за операції
    CREDIT_EXPANSION,         // відсотки за кредит
    DEPOSIT_PAYDAY,           // відсотки за депозит
    TRANSACTION,              // Транзакція
    SELF_TRANSACTION,         // Транзакція між власними картками
    TAX,                      // Податок
    PAYDAY,                   // Заробітна плата
    IBAN_TRANSACTION,         // Транзакція по IBAN
    ROLLBACK,                 // Повернення коштів
};

enum transaction_status {
    SUCCES,
    ALLOWED,
    DENIED,
    REQUEST,

    ROLLBACK_REQUEST,
    ROLLBACK_DONE,

    FAIL,
    FAIL_NO_MONEY,
    FAIL_WRONG_ACCOUNT,
    FAIL_CARD_BLOCKED,
    FAIL_YOUR_CARD_BLOCKED,
    FAIL_WRONG_CVV,
    
    SUSPECT_REQUEST
};

class TransactionLog {
    int id;
    int transaction_id;
    operations operation_type;
    transaction_status status;

    char time[25];
    bool other_currency;  
	bool blocked_money;   // use for requests

    // account from
    char from_IBAN[36];
    char from_cardNumber[16];
    double from_currency_before;
    double from_currency_after;
    // account to
    char to_IBAN[36];
    char to_cardNumber[16];
    double to_currency_before;
    double to_currency_after;

    // !! Інформація про стан рахунку додається вже під час затвердження

    // requested by
    bool other_bank_user;
    int user_id;

    // currency info
    double currency;
	balanceType currency_type;

    char allowed_by[30];
    char comment[255];

public:
    TransactionLog() {
        id = -1;
		transaction_id = -1;
        operation_type = TRANSACTION;
        status = REQUEST;
        strcpy_s(time, sizeof(time), getTimestamp().c_str());
        other_currency = false;
        blocked_money = false;

        strcpy_s(from_IBAN, sizeof(from_IBAN), "");
        strcpy_s(from_cardNumber, sizeof(from_cardNumber), "");
        strcpy_s(to_IBAN, sizeof(to_IBAN), "");
        strcpy_s(to_cardNumber, sizeof(to_cardNumber), "");

        other_bank_user = false;
        user_id = -1;
        currency = 0.0;

        strcpy_s(allowed_by, sizeof(allowed_by), "");
        strcpy_s(comment, sizeof(comment), "");
    }

    // ---------------------- Конструктор з параметрами ----------------------
    TransactionLog(
        int _id,
		int _transaction_id,
        operations op, transaction_status st,
        bool otherCur, bool blocked,
        const char* fromIban, const char* fromCard,
        const char* toIban, const char* toCard,
        bool otherBank, int uid, double curr,
        const char* allowedBy, const char* comm
    ) {
        id = _id;
		transaction_id = _transaction_id;
        operation_type = op;
        status = st;
        strcpy_s(time, sizeof(time), getTimestamp().c_str());
        other_currency = otherCur;
        blocked_money = blocked;

        strcpy_s(from_IBAN, sizeof(from_IBAN), fromIban);
        strcpy_s(from_cardNumber, sizeof(from_cardNumber), fromCard);
        strcpy_s(to_IBAN, sizeof(to_IBAN), toIban);
        strcpy_s(to_cardNumber, sizeof(to_cardNumber), toCard);

        other_bank_user = otherBank;
        user_id = uid;
        currency = curr;

        strcpy_s(allowed_by, sizeof(allowed_by), allowedBy);
        strcpy_s(comment, sizeof(comment), comm);
    }

    // ---------------------- Методи керування статусом ----------------------
    bool allow() {
        status = ALLOWED;
        blocked_money = false;
        return true;
    }

    bool deny() {
        status = DENIED;
        blocked_money = false;
        return true;
    }

    void fail(transaction_status fail_status) {
        status = fail_status;
        blocked_money = false;
    }

    bool request(transaction_status request_status) {
        status = request_status;
        blocked_money = true;
        return true;
    }

    // ---------------------- Запис у файл ----------------------
    void saveToFile(std::ostream& out) const {
        out.write(reinterpret_cast<const char*>(&id), sizeof(id));
        out.write(reinterpret_cast<const char*>(&transaction_id), sizeof(transaction_id));
        out.write(reinterpret_cast<const char*>(&operation_type), sizeof(operation_type));
        out.write(reinterpret_cast<const char*>(&status), sizeof(status));
        out.write(time, sizeof(time));
        out.write(reinterpret_cast<const char*>(&other_currency), sizeof(other_currency));
        out.write(reinterpret_cast<const char*>(&blocked_money), sizeof(blocked_money));
        out.write(from_IBAN, sizeof(from_IBAN));
        out.write(from_cardNumber, sizeof(from_cardNumber));
        out.write(to_IBAN, sizeof(to_IBAN));
        out.write(to_cardNumber, sizeof(to_cardNumber));
        out.write(reinterpret_cast<const char*>(&other_bank_user), sizeof(other_bank_user));
        out.write(reinterpret_cast<const char*>(&user_id), sizeof(user_id));
        out.write(reinterpret_cast<const char*>(&currency), sizeof(currency));
        out.write(allowed_by, sizeof(allowed_by));
        out.write(comment, sizeof(comment));
    }

    // ---------------------- Завантаження з файлу ----------------------
    void loadFromFile(std::istream& in) {
        in.read(reinterpret_cast<char*>(&id), sizeof(id));
        in.read(reinterpret_cast<char*>(&transaction_id), sizeof(transaction_id));
        in.read(reinterpret_cast<char*>(&operation_type), sizeof(operation_type));
        in.read(reinterpret_cast<char*>(&status), sizeof(status));
        in.read(time, sizeof(time));
        in.read(reinterpret_cast<char*>(&other_currency), sizeof(other_currency));
        in.read(reinterpret_cast<char*>(&blocked_money), sizeof(blocked_money));
        in.read(from_IBAN, sizeof(from_IBAN));
        in.read(from_cardNumber, sizeof(from_cardNumber));
        in.read(to_IBAN, sizeof(to_IBAN));
        in.read(to_cardNumber, sizeof(to_cardNumber));
        in.read(reinterpret_cast<char*>(&other_bank_user), sizeof(other_bank_user));
        in.read(reinterpret_cast<char*>(&user_id), sizeof(user_id));
        in.read(reinterpret_cast<char*>(&currency), sizeof(currency));
        in.read(allowed_by, sizeof(allowed_by));
        in.read(comment, sizeof(comment));
    }

    // ---------------------- Вивід у консоль ----------------------
    void print() const {
        std::cout << "ID: " << id
            << " | Operation: " << operation_type
            << " | Status: " << status
            << " | Time: " << time
            << " | From: " << from_cardNumber
            << " | To: " << to_cardNumber
            << " | Amount: " << currency
            << " | Allowed by: " << allowed_by
            << " | Comment: " << comment
            << std::endl;
    };
    bool updateInFile();
};

//====================================================================
//====================================================================

enum action {
    COMMAND_BAN_ACC, COMMAND_UNBAN_ACC, COMMAND_VERIFY_ACC, COMMAND_CHANGE_PIN, COMMAND_CHANGE_CVV,
    COMMAND_BAN_USER, COMMAND_UNBAN_USER, COMMAND_CHANGE_PASSWORD, COMMAND_CHANGE_PASSPORT,
    TRANSACTION_REQUEST, TRANSACTION_STATUS_CHANGE, 
    CONNECTED, DISCONECTED, LOGINNED, LOGIN_FAIL,
};

// ------------------------- Enum -> String -------------------------
const std::map<action, std::string> actionToString = {
    { COMMAND_BAN_ACC, "COMMAND_BAN_ACC" },
    { COMMAND_UNBAN_ACC, "COMMAND_UNBAN_ACC" },
    { COMMAND_VERIFY_ACC, "COMMAND_VERIFY_ACC" },
    { COMMAND_CHANGE_PIN, "COMMAND_CHANGE_PIN" },
    { COMMAND_CHANGE_CVV, "COMMAND_CHANGE_CVV" },
    { COMMAND_BAN_USER, "COMMAND_BAN_USER" },
    { COMMAND_UNBAN_USER, "COMMAND_UNBAN_USER" },
    { COMMAND_CHANGE_PASSWORD, "COMMAND_CHANGE_PASSWORD" },
    { COMMAND_CHANGE_PASSPORT, "COMMAND_CHANGE_PASSPORT" },
    { TRANSACTION_REQUEST, "TRANSACTION_REQUEST" },
    { TRANSACTION_STATUS_CHANGE, "TRANSACTION_STATUS_CHANGE" },
    { CONNECTED, "CONNECTED" },
    { DISCONECTED, "DISCONECTED" },
    { LOGINNED, "LOGINNED" },
    { LOGIN_FAIL, "LOGIN_FAIL" }
};

// ------------------------- String -> Enum -------------------------
const std::map<std::string, action> stringToAction = {
    { "COMMAND_BAN_ACC", COMMAND_BAN_ACC },
    { "COMMAND_UNBAN_ACC", COMMAND_UNBAN_ACC },
    { "COMMAND_VERIFY_ACC", COMMAND_VERIFY_ACC },
    { "COMMAND_CHANGE_PIN", COMMAND_CHANGE_PIN },
    { "COMMAND_CHANGE_CVV", COMMAND_CHANGE_CVV },
    { "COMMAND_BAN_USER", COMMAND_BAN_USER },
    { "COMMAND_UNBAN_USER", COMMAND_UNBAN_USER },
    { "COMMAND_CHANGE_PASSWORD", COMMAND_CHANGE_PASSWORD },
    { "COMMAND_CHANGE_PASSPORT", COMMAND_CHANGE_PASSPORT },
    { "TRANSACTION_REQUEST", TRANSACTION_REQUEST },
    { "TRANSACTION_STATUS_CHANGE", TRANSACTION_STATUS_CHANGE },
    { "CONNECTED", CONNECTED },
    { "DISCONECTED", DISCONECTED },
    { "LOGINNED", LOGINNED },
    { "LOGIN_FAIL", LOGIN_FAIL }
};

class AuditLog {
    int id; 
    // init by user at time
    int user_id;   //user PM
    int session_id;

    char time[25];

    // Log info
    action Action;
    char info[124];

    bool success;               // Було виконано успішно чи ні
    char error_message[128];    // Якщо дія завершилась з помилкою


public:
	AuditLog() {
		id = -1;
		user_id = -1;
		session_id = -1;
		strcpy_s(time, sizeof(time), getTimestamp().c_str());
		Action = CONNECTED;
		strcpy_s(info, sizeof(info), "");
		success = true;
		strcpy_s(error_message, sizeof(error_message), "");
	}
    AuditLog(int _id, int _userID, int _sessionID, action _act, const char* _info, bool _success = true, const char* _errorMessage = "")
    {
        id = _id;
        user_id = _userID;
        session_id = _sessionID;
        Action = _act;
        strncpy_s(info, sizeof(info), _info, _TRUNCATE);
        success = _success;
        strncpy_s(error_message, sizeof(error_message), _errorMessage, _TRUNCATE);
        strncpy_s(time, sizeof(time), getTimestamp().c_str(), _TRUNCATE);
    }

	// ---------------------- Запис у файл ----------------------
    void saveToFile(std::ostream& out) const {
        out.write(reinterpret_cast<const char*>(&id), sizeof(id));
        out.write(reinterpret_cast<const char*>(&user_id), sizeof(user_id));
        out.write(reinterpret_cast<const char*>(&session_id), sizeof(session_id));
        out.write(time, sizeof(time));
        out.write(reinterpret_cast<const char*>(&Action), sizeof(Action));
        out.write(info, sizeof(info));
        out.write(reinterpret_cast<const char*>(&success), sizeof(success));
        out.write(error_message, sizeof(error_message));
    };
	// ---------------------- Завантаження з файлу ----------------------
    void loadFromFile(std::istream& in) {
        in.read(reinterpret_cast<char*>(&id), sizeof(id));
        in.read(reinterpret_cast<char*>(&user_id), sizeof(user_id));
        in.read(reinterpret_cast<char*>(&session_id), sizeof(session_id));
        in.read(time, sizeof(time));
        in.read(reinterpret_cast<char*>(&Action), sizeof(Action));
        in.read(info, sizeof(info));
        in.read(reinterpret_cast<char*>(&success), sizeof(success));
        in.read(error_message, sizeof(error_message));
    };
	// ---------------------- Вивід у консоль ----------------------
	void print() const {
		std::cout << "ID: " << id
			<< " | User ID: " << user_id
			<< " | Session ID: " << session_id
			<< " | Time: " << time
			//<< " | Action: " << actionToString.at(Action)
			<< " | Info: " << info
			<< " | Success: " << (success ? "Yes" : "No")
			<< " | Error Message: " << error_message
			<< std::endl;
	};
	bool updateInFile();
};


void create_db_transaction_log();
void create_db_audit_log();
void TRAN_addTransaction(
    int id,
    int transaction_id,
    operations op,
    transaction_status st,
    bool otherCur,
    bool blocked,
    const char* fromIban,
    const char* fromCard,
    const char* toIban,
    const char* toCard,
    bool otherBank,
    int userID,
    double amount,
    const char* allowedBy,
    const char* comment
);

