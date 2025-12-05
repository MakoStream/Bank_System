#include "Transactions.h"
#include <fstream>
#include <iostream>
#include <thread>
#include <atomic>

std::mutex transactionFileMutex;

using namespace std;

std::thread workerThread;
bool stopFlag = false;

void Transaction::transactionRequest(account _from_account, account _to_account, User _initieted_by_user, float _amount, const char* _PIN_CVV, string comment) {
	Transaction new_transaction(process.incrementTransactionID(), _from_account, _to_account, _initieted_by_user, _amount, _PIN_CVV, 0); // comment db will be created later

    std::ofstream ofs(process.getTransactionLogDBPath(), std::ios::binary | std::ios::app); // <-- app для додавання
    if (!ofs) {
        std::cerr << "Помилка відкриття файлу для запису транзакції" << std::endl;
        return;
    }

    new_transaction.save(ofs); // записуємо у кінець файлу

	return;
};

void Transaction::updateInFile() {

    std::string path = process.getTransactionLogDBPath();
    std::string tmpPath = path + ".tmp";

    std::ifstream in(path, std::ios::binary);
    if (!in) {
        std::cerr << "Помилка: не вдалося відкрити файл transactions.dat для читання." << std::endl;
        return;
    }

    std::ofstream out(tmpPath, std::ios::binary | std::ios::trunc);
    if (!out) {
        std::cerr << "Помилка: не вдалося створити тимчасовий файл transactions.tmp." << std::endl;
        return;
    }

    bool found = false;
    Transaction temp;

    while (true) {
        temp = Transaction();
        temp.read(in);
        if (!in) break;

        if (temp.getId() == this->getId()) {
            this->save(out);   // записуємо оновлену транзакцію
            found = true;
        }
        else {
            temp.save(out);    // копіюємо як є
        }
    }

    in.close();
    out.close();

    if (!found) {
        std::cerr << "Попередження: транзакцію з таким ID не знайдено." << std::endl;
        std::remove(tmpPath.c_str());
        return;
    }

    // замінюємо старий файл новим
    std::remove(path.c_str());
    std::rename(tmpPath.c_str(), path.c_str());
};


void Transaction::printAllTransactions(char msg[5][1024], int page) {
    ifstream fin(process.getTransactionLogDBPath(), ios::binary);
    if (!fin) {
        cerr << "Не вдалося відкрити transactions.dat для читання." << endl;
        for (int i = 0; i < 5; i++) msg[i][0] = '\0';
        return;
    }

    Transaction tr;
    int startIndex = (page - 1) * 25; // 25 записів на сторінку
    int currentIndex = 0;
    int msgIndex = 0;

    string buffer;

    while (true) {
        tr = Transaction();
        tr.read(fin);
        if (!fin) break;

        if (currentIndex < startIndex) {
            currentIndex++;
            continue;
        }

        stringstream ss;
        ss << "ID: " << tr.getId()
            << " | FROM: " << tr.getFromAccountId()
            << " | TO: " << tr.getToAccountId()
            << " | INITIATED BY: " << tr.getInitietedByUserId()
            << " | STATUS: " << statusEnumToString(tr.getStatus())
            << " | ALLOWED BY: " << tr.getAllowedByUserId()
            << "\n";

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

    for (; msgIndex < 5; msgIndex++)
        msg[msgIndex][0] = '\0';

    fin.close();
}

void transactionWorker() {
	string filename = process.getTransactionLogDBPath();
    while (!stopFlag) {
        std::ifstream fin(filename, std::ios::binary);
        if (!fin) {
            std::cerr << "Не вдалося відкрити файл transactions.dat" << std::endl;
            return;
        }
        Transaction trx;
        std::lock_guard<std::mutex> lock(transactionFileMutex);
        while (!fin.eof()) {
            trx.read(fin);
            if (fin.eof()) break;
            if (trx.getStatus() == ALLOWED or trx.getStatus() == PROCESSED) { continue; };

            // Тут обробляємо транзакцію
			account from_acc = account::getAccountById(trx.getFromAccountId());
			account to_acc = account::getAccountById(trx.getToAccountId());

			User init_user = User::getUser_byId(trx.getInitietedByUserId());

			if (from_acc.getBalance() >= trx.getAmount()) {

				// Можна виконати. Перевіряємо, чи є власником карти ініціатор
                if (from_acc.getUserID() == init_user.getId() && to_acc.getUserID() == init_user.getId()) {
                    // Операція між власними картками ініціатора
                    trx.processTransaction(ALLOWED, 0, emptyUser); // 0 - дефолт поки
                    fin.close();
                    trx.updateInFile();
                    continue;
                }
				if (from_acc.getUserID() == init_user.getId() && from_acc.checkPIN(trx.getPIN())) {
					// Власник карти - ініціатор
					trx.processTransaction(ALLOWED, 0, emptyUser); // 0 - дефолт поки
                    fin.close();
                    trx.updateInFile();
                    continue;
				}
                if (from_acc.getUserID() != init_user.getId() && from_acc.checkCVV(trx.getCVV())) {
					// Не власник карти, але CVV вірний (магазин і т.д ) ||| White list in future
					trx.processTransaction(ALLOWED, 0, emptyUser); // 0 - дефолт поки
                    fin.close();
                    trx.updateInFile();
                    continue;
                }
			}
			else {
				// Відхиляємо транзакцію
				trx.declineTransaction(1, emptyUser); // 1 - недостатньо коштів
				fin.close();
				trx.updateInFile();
				continue;
			}


            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        fin.close();
    };

    
    std::cout << "Потік транзакцій завершено" << std::endl;
}

// Функція запуску потоку
void Transaction::startTransactionThread() {
    stopFlag = false;
    if (workerThread.joinable())
        workerThread.join(); // зачекаємо, якщо попередній ще працює
    workerThread = std::thread(transactionWorker);
}

// Функція зупинки потоку
void Transaction::stopTransactionThread() {
    stopFlag = true;
    if (workerThread.joinable())
        workerThread.join();
}

void Transaction::createDB() {
	std::ofstream fout(process.getTransactionLogDBPath(), std::ios::binary | std::ios::trunc);
	fout.close();
};

Transaction Transaction::getTransactionById(int transaction_id) {
	ifstream fin(process.getTransactionLogDBPath(), ios::binary);
	Transaction tr;
	while (true) {
		tr = Transaction();
		tr.read(fin);
		if (!fin) break;
		if (tr.getId() == transaction_id) {
			fin.close();
			return tr;
		}
	}
	fin.close();
	return Transaction(); // повертаємо порожню транзакцію, якщо не знайдено
}

vector<Transaction> Transaction::getTransactionsInitBy(User user) {
	vector<Transaction> transactions;
    int user_id = user.getId();
    ifstream fin(process.getTransactionLogDBPath(), ios::binary);
    Transaction tr;
    while (true) {
        tr = Transaction();
        tr.read(fin);
        if (!fin)break;
        if (tr.getInitietedByUserId() == user_id) {
			transactions.push_back(tr);
        };
    }
	fin.close();
	return transactions;
};

vector<Transaction> Transaction::getTransactionWithAccount(account acc) {
    vector<Transaction> transactions;
    int account_id = acc.getId();
    ifstream fin(process.getTransactionLogDBPath(), ios::binary);
    Transaction tr;
    while (true) {
        tr = Transaction();
        tr.read(fin);
        if (!fin)break;
        if (tr.getFromAccountId() == account_id || tr.getToAccountId()==account_id) {
            transactions.push_back(tr);
        };
    }
    fin.close();
    return transactions;
};

vector<int> Transaction::getUserTransactions(int user_id) {
	vector<int> transaction_ids;
	ifstream fin(process.getTransactionLogDBPath(), ios::binary);
	Transaction tr;
	while (true) {
		tr = Transaction();
		tr.read(fin);
		if (!fin)break;
		account from_acc = account::getAccountById(tr.getFromAccountId());
		account to_acc = account::getAccountById(tr.getToAccountId());
		if (from_acc.getUserID() == user_id || to_acc.getUserID() == user_id) {
			transaction_ids.push_back(tr.getId());
		};
	}
	fin.close();
	return transaction_ids;
}; 