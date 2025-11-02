#include "Audit.h"
#include "../mainProcess.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//====================================================================
// Operations map
// ------------------------- Enum -> String -------------------------
const std::map<operations, std::string> operationToString = {
    { COMMISION, "COMMISION" },
    { CREDIT_EXPANSION, "CREDIT_EXPANSION" },
    { DEPOSIT_PAYDAY, "DEPOSIT_PAYDAY" },
    { TRANSACTION, "TRANSACTION" },
    { SELF_TRANSACTION, "SELF_TRANSACTION" },
    { TAX, "TAX" },
    { PAYDAY, "PAYDAY" },
    { IBAN_TRANSACTION, "IBAN_TRANSACTION" },
    { ROLLBACK, "ROLLBACK" }
};

// ------------------------- String -> Enum -------------------------
const std::map<std::string, operations> stringToOperation = {
    { "COMMISION", COMMISION },
    { "CREDIT_EXPANSION", CREDIT_EXPANSION },
    { "DEPOSIT_PAYDAY", DEPOSIT_PAYDAY },
    { "TRANSACTION", TRANSACTION },
    { "SELF_TRANSACTION", SELF_TRANSACTION },
    { "TAX", TAX },
    { "PAYDAY", PAYDAY },
    { "IBAN_TRANSACTION", IBAN_TRANSACTION },
    { "ROLLBACK", ROLLBACK }
};
//====================================================================
// Transaction Status map
// ------------------------- Enum -> String -------------------------
const std::map<transaction_status, std::string> transactionStatusToString = {
    { SUCCES, "SUCCES" },
    { ALLOWED, "ALLOWED" },
    { DENIED, "DENIED" },
    { REQUEST, "REQUEST" },
    { ROLLBACK_REQUEST, "ROLLBACK_REQUEST" },
    { ROLLBACK_DONE, "ROLLBACK_DONE" },
    { FAIL, "FAIL" },
    { FAIL_NO_MONEY, "FAIL_NO_MONEY" },
    { FAIL_WRONG_ACCOUNT, "FAIL_WRONG_ACCOUNT" },
    { FAIL_CARD_BLOCKED, "FAIL_CARD_BLOCKED" },
    { FAIL_YOUR_CARD_BLOCKED, "FAIL_YOUR_CARD_BLOCKED" },
    { FAIL_WRONG_CVV, "FAIL_WRONG_CVV" },
    { FAIL_WRONG_PIN, "FAIL_WRONG_PIN" },
    { SUSPECT_REQUEST, "SUSPECT_REQUEST" }
};
// ------------------------- String -> Enum -------------------------
const std::map<std::string, transaction_status> stringToTransactionStatus = {
    { "SUCCES", SUCCES },
    { "ALLOWED", ALLOWED },
    { "DENIED", DENIED },
    { "REQUEST", REQUEST },
    { "ROLLBACK_REQUEST", ROLLBACK_REQUEST },
    { "ROLLBACK_DONE", ROLLBACK_DONE },
    { "FAIL", FAIL },
    { "FAIL_NO_MONEY", FAIL_NO_MONEY },
    { "FAIL_WRONG_ACCOUNT", FAIL_WRONG_ACCOUNT },
    { "FAIL_CARD_BLOCKED", FAIL_CARD_BLOCKED },
    { "FAIL_YOUR_CARD_BLOCKED", FAIL_YOUR_CARD_BLOCKED },
    { "FAIL_WRONG_CVV", FAIL_WRONG_CVV },
    { "FAIL_WRONG_PIN", FAIL_WRONG_PIN },
    { "SUSPECT_REQUEST", SUSPECT_REQUEST }
};
//====================================================================


// ---------- TransactionLog::updateInFile ----------
bool TransactionLog::updateInFile() {
	string filePath = process.getTransactionLogDBPath();
    // 1) Відкрити для послідовного читання
    std::ifstream in(filePath, std::ios::binary);
    if (!in) {
        std::cerr << "Помилка: не вдалося відкрити файл транзакцій для читання: " << filePath << std::endl;
        return false;
    }

    TransactionLog temp;
    std::streampos pos;
    bool found = false;

    while (true) {
        pos = in.tellg();
        // Викликаємо існуючу функцію читання (яка приймає ifstream& або istream&)
        temp.loadFromFile(in);
        if (!in.good()) break; // кінець файлу або помилка

        if (temp.id == this->id) {
            found = true;
            break;
        }
    }

    in.close();

    if (!found) {
        std::cerr << "Попередження: транзакцію з ID = " << id << " не знайдено у файлі: " << filePath << std::endl;
        return false;
    }

    // 2) Відкрити файл для оновлення і перезаписати запис на знайденій позиції
    std::fstream io(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!io) {
        std::cerr << "Помилка: не вдалося відкрити файл транзакцій для запису: " << filePath << std::endl;
        return false;
    }

    io.seekp(pos);
    if (!io.good()) {
        std::cerr << "Помилка: seekp не вдався." << std::endl;
        io.close();
        return false;
    }

    this->saveToFile(io); // перезаписуємо поточний об'єкт
    io.flush();
    io.close();

    return true;
}

// ---------- AuditLog::updateInFile ----------
bool AuditLog::updateInFile() {
	string filePath = process.getAuditLogDBPath();
    std::ifstream in(filePath, std::ios::binary);
    if (!in) {
        std::cerr << "Помилка: не вдалося відкрити файл аудиту для читання: " << filePath << std::endl;
        return false;
    }

    AuditLog temp;
    std::streampos pos;
    bool found = false;

    while (true) {
        pos = in.tellg();
        temp.loadFromFile(in);
        if (!in.good()) break;

        if (temp.id == this->id) {
            found = true;
            break;
        }
    }

    in.close();

    if (!found) {
        std::cerr << "Попередження: запис аудиту з ID = " << id << " не знайдено у файлі: " << filePath << std::endl;
        return false;
    }

    std::fstream io(filePath, std::ios::in | std::ios::out | std::ios::binary);
    if (!io) {
        std::cerr << "Помилка: не вдалося відкрити файл аудиту для запису: " << filePath << std::endl;
        return false;
    }

    io.seekp(pos);
    if (!io.good()) {
        std::cerr << "Помилка: seekp не вдався." << std::endl;
        io.close();
        return false;
    }

    this->saveToFile(io);
    io.flush();
    io.close();

    return true;
}




// ---------- Functions to create audit log database ----------
void create_db_audit_log() {
	string filePath = process.getAuditLogDBPath();
	std::ofstream ofs(filePath, std::ios::binary | std::ios::trunc);
	if (!ofs) {
		std::cerr << "Помилка: не вдалося створити файл аудиту: " << filePath << std::endl;
		return;
	}
	ofs.close();
	std::cout << "Файл аудиту успішно створено: " << filePath << std::endl;
}
// ---------- Functions to create transaction log database ----------
void create_db_transaction_log() {
    string filePath = process.getTransactionLogDBPath();
    std::ofstream ofs(filePath, std::ios::binary | std::ios::trunc);
    if (!ofs) {
        std::cerr << "Помилка: не вдалося створити файл транзакцій: " << filePath << std::endl;
        return;
    }
    ofs.close();
    std::cout << "Файл транзакцій успішно створено: " << filePath << std::endl;
};

// Function: TRAN_addTransaction
// Description: Створює новий запис TransactionLog і зберігає його у файл
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
) {
    TransactionLog transaction(
        id,
        transaction_id,
        op,
        st,
        otherCur,
        blocked,
        fromIban,
        fromCard,
        toIban,
        toCard,
        otherBank,
        userID,
        amount,
        allowedBy,
        comment
    );

    std::ofstream fout(process.getTransactionLogDBPath(), std::ios::binary | std::ios::app);
    if (!fout) {
        std::cerr << "Не вдалося відкрити файл транзакцій для запису." << std::endl;
        return;
    }

    transaction.saveToFile(fout);
    fout.close();
}

// Function: AUD_addAudit
// Description: Створює новий запис AuditLog і зберігає його у файл
void AUD_addAudit(int id, int userID, int sessionID, action act, const char* info, bool success = true, const char* errorMessage = "")
{
    AuditLog audit(id, userID, sessionID, act, info, success, errorMessage);

    std::ofstream fout(process.getAuditLogDBPath(), std::ios::binary | std::ios::app);
    if (!fout) {
        std::cerr << "Не вдалося відкрити файл аудиту для запису." << std::endl;
        return;
    }

    audit.saveToFile(fout);
    fout.close();
}

int getLastTransactionID() {
	string filePath = process.getTransactionLogDBPath();
	std::ifstream in(filePath, std::ios::binary);
	if (!in) {
		std::cerr << "Помилка: не вдалося відкрити файл транзакцій для читання: " << filePath << std::endl;
		return -1;
	}

	TransactionLog temp;
	int lastID = -1;

	while (true) {
		temp.loadFromFile(in);
		if (!in.good()) break;

		lastID = temp.getID();
	}

	in.close();
	return lastID;
};

int getLastAuditID() {
	string filePath = process.getAuditLogDBPath();
	std::ifstream in(filePath, std::ios::binary);
	if (!in) {
		std::cerr << "Помилка: не вдалося відкрити файл аудиту для читання: " << filePath << std::endl;
		return -1;
	}

	AuditLog temp;
	int lastID = -1;

	while (true) {
		temp.loadFromFile(in);
		if (!in.good()) break;

		lastID = temp.getID();
	}

	in.close();
	return lastID;
};

vector<TransactionLog> getTransactionsLogs(int transaction_id) {   // O(n)
    vector<TransactionLog> result;
    string filePath = process.getAuditLogDBPath();
    std::ifstream from(filePath, std::ios::binary);
    if (!from) {
        std::cerr << "Помилка: не вдалося відкрити файл аудиту для читання: " << filePath << std::endl;
        return result;
    }

    while (from.peek() != EOF) { // поки не кінець файлу
        TransactionLog temp;
        temp.loadFromFile(from);
        if (!from) break; // якщо читання не вдалося
        if (temp.getTransactionID() == transaction_id) {
            result.push_back(temp);
        }
    }

    from.close();
    return result;
}
vector<AuditLog> getAuditLogs(int transaction_PM_id) { // O(n)
	vector<AuditLog> result;
	string filePath = process.getAuditLogDBPath();
	std::ifstream from(filePath, std::ios::binary);
	if (!from) {
		std::cerr << "Помилка: не вдалося відкрити файл аудиту для читання: " << filePath << std::endl;
		return result;
	}

    while (from.peek() != EOF) { // поки не кінець файлу
        AuditLog temp;
        temp.loadFromFile(from);
        if (!from) break; // якщо читання не вдалося
        if (temp.getSessionID() == transaction_PM_id) {
            result.push_back(temp);
        };
    };

	from.close();
	return result;
};
vector <AuditLog> getUserAuditLogs(int user_id) {
	vector<AuditLog> result;
	string filePath = process.getAuditLogDBPath();
	std::ifstream from(filePath, std::ios::binary);
	if (!from) {
		std::cerr << "Помилка: не вдалося відкрити файл аудиту для читання: " << filePath << std::endl;
		return result;
	}

	while (from.peek() != EOF) { // поки не кінець файлу
		AuditLog temp;
		temp.loadFromFile(from);
		if (!from) break; // якщо читання не вдалося
		if (temp.getUserID() == user_id) {
			result.push_back(temp);
		}
	}

	from.close();
	return result;
};

void save_audit_log(AuditLog& log) {
	std::ofstream fout(process.getAuditLogDBPath(), std::ios::binary | std::ios::app);
	if (!fout) {
		std::cerr << "Не вдалося відкрити файл аудиту для запису." << std::endl;
		return;
	}
	log.saveToFile(fout);
	fout.close();
};

void save_transaction_log(TransactionLog& log) {
	std::ofstream fout(process.getTransactionLogDBPath(), std::ios::binary | std::ios::app);
	if (!fout) {
		std::cerr << "Не вдалося відкрити файл транзакцій для запису." << std::endl;
		return;
	}
	log.saveToFile(fout);
	fout.close();
};


void printTransactions(char msg[5][1024], int page) {
    ifstream fin(process.getTransactionLogDBPath(), ios::binary);
    if (!fin) { cerr << "Не вдалося відкрити файл для читання." << endl; for (int i = 0; i < 5; i++) msg[i][0] = '\0'; return; }

    TransactionLog log;
    int startIndex = (page - 1) * 25;
    int currentIndex = 0;
    int msgIndex = 0;
    string buffer;

    while (!fin.eof()) {
        log.loadFromFile(fin);
        if (currentIndex < startIndex) { currentIndex++; continue; }

		stringstream ss;  // id | transaction_id | PAN from | PAN to | Status | by User | Amount 
        ss << "id: " << log.getID()
            << " | transaction_id: " << log.getTransactionID()
            << " | PAN from: " << log.getFromCardNumber()
            << " | PAN to: " << log.getToCardNumber()
            << " | Status: " << transactionStatusToString.at(log.getStatus())
			<< " | by User: " << log.getUserID()
			<< " | Amount: " << log.getCurrency() << "\n";
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