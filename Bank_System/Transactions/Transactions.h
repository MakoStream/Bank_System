#pragma once

#include "../mainProcess.h"

#include <fstream>
#include <iostream>

#include <string>
#include "../LogEye.h"
#include "../Account.h"
#include "../User.h"
#include <mutex>

extern std::mutex transactionFileMutex;

enum status_enum {
	ALLOWED, DECLINED, WAITING, CANCELLED, PROCESSED
};

class Transaction {
private:
	int id;

	// accounts
	int from_account_id;
	int to_account_id;

	int initieted_by_user_id;		 // who initiated the transaction

	status_enum status=WAITING;
	int status_reason=0;         // reason for status (id will be realised later)

	char request_timestamp[20];  // when request was made
	char process_timestamp[20];	 // when request was processed

	float amount=0.0f;

	bool initiated_by_owner = 0; // true if initiated by account owner
	char PIN[5];				 // if initiated by owner
	char CVV[4];				 // if initiated by non owner

	int comment_id=0;	         // comment associated with transaction !!! NEED create a comment binfile
	int allowed_by_user_id;		 // who allowed the transaction (-1 if allowed by server)



	// constructors
	Transaction(int _id, account _from_account, account _to_account, User _initieted_by_user, float _amount, const char* _PIN_CVV, int _comment_id) {
		//basic data initialization
		id = _id;
		from_account_id = _from_account.getId();
		to_account_id = _to_account.getId();
		initieted_by_user_id = _initieted_by_user.getId();
		amount = _amount;
		comment_id = _comment_id;
		// if owner
		if (initieted_by_user_id == _from_account.getUserID()) {
			initiated_by_owner = 1;
		};
		// request timestamp
		strncpy(request_timestamp, getTimestamp().c_str(), sizeof(request_timestamp));
		request_timestamp[sizeof(request_timestamp) - 1] = '\0';


		// PIN or CVV
		if (initiated_by_owner) {
			strncpy(PIN, _PIN_CVV, sizeof(PIN));
			PIN[sizeof(PIN) - 1] = '\0';
		}
		else {
			strncpy(CVV, _PIN_CVV, sizeof(CVV));
			CVV[sizeof(CVV) - 1] = '\0';
		};

		return;
	};
public:

	Transaction() {}; // default constructor

	// Getters
	int getId() const { return id; };
	status_enum getStatus() const { return status; };
	float getAmount() const { return amount; };
	int getFromAccountId() const { return from_account_id; };
	int getToAccountId() const { return to_account_id; };
	int getInitietedByUserId() const { return initieted_by_user_id; }
	int getAllowedByUserId() const { return allowed_by_user_id; }

	char* getPIN() { return PIN; };
	char* getCVV() { return CVV; };

	// ======================
	static const char* statusEnumToString(status_enum st) {
		switch (st) {
		case ALLOWED:   return "ALLOWED";
		case DECLINED:  return "DECLINED";
		case WAITING:   return "WAITING";
		case CANCELLED: return "CANCELLED";
		case PROCESSED: return "PROCESSED";
		}
		return "UNKNOWN";
	}
	// Setters and processors

	void cancelTransaction(int _status_reason, User _allowed_by_user) {
		status = CANCELLED;
		status_reason = _status_reason;
		allowed_by_user_id = _allowed_by_user.getId();

		// process timestamp
		strncpy(process_timestamp, getTimestamp().c_str(), sizeof(process_timestamp));
		process_timestamp[sizeof(process_timestamp) - 1] = '\0';

		return;
	};

	void processTransaction(status_enum _status, int _status_reason, User _allowed_by_user) {
		cout << "Prosecced" << endl;
		status = _status;
		status_reason = _status_reason;
		allowed_by_user_id = _allowed_by_user.getId();

		// process timestamp
		strncpy(process_timestamp, getTimestamp().c_str(), sizeof(process_timestamp));
		process_timestamp[sizeof(process_timestamp) - 1] = '\0';

		// finalize transaction (money operations if success)

		account from_account   = account::getAccountById(from_account_id);
		account to_account     = account::getAccountById(to_account_id);

		// if allowed
		if (status == ALLOWED) {
			from_account.setBalance(-amount);
			to_account.setBalance(amount);

			from_account.updateInFile();
			to_account.updateInFile();
		};

		//===================================================
		return;
	};

	void declineTransaction(int _status_reason, User _allowed_by_user) {
		status = DECLINED;
		status_reason = _status_reason;
		allowed_by_user_id = _allowed_by_user.getId();

		// process timestamp
		strncpy(process_timestamp, getTimestamp().c_str(), sizeof(process_timestamp));
		process_timestamp[sizeof(process_timestamp) - 1] = '\0';

		return;
	};

	static void transactionRequest(account _from_account, account _to_account, User _initieted_by_user, float _amount, const char* _PIN_CVV, string comment = "");
	static void printAllTransactions(char msg[5][1024], int page);
	static void createDB();

	static Transaction getTransactionById(int transaction_id);
	static vector<Transaction> getTransactionsInitBy(User user);
	static vector<Transaction> getTransactionWithAccount(account acc);
	static void startTransactionThread();
	static void stopTransactionThread();

	// save to files
	void save(std::ofstream& ofs) {
		ofs.write((char*)&id, sizeof(id));
		ofs.write((char*)&from_account_id, sizeof(from_account_id));
		ofs.write((char*)&to_account_id, sizeof(to_account_id));
		ofs.write((char*)&initieted_by_user_id, sizeof(initieted_by_user_id));
		ofs.write((char*)&status, sizeof(status));
		ofs.write((char*)&status_reason, sizeof(status_reason));
		ofs.write((char*)&request_timestamp, sizeof(request_timestamp));
		ofs.write((char*)&process_timestamp, sizeof(process_timestamp));
		ofs.write((char*)&amount, sizeof(amount));
		ofs.write((char*)&initiated_by_owner, sizeof(initiated_by_owner));
		ofs.write((char*)&PIN, sizeof(PIN));
		ofs.write((char*)&CVV, sizeof(CVV));
		ofs.write((char*)&comment_id, sizeof(comment_id));
		ofs.write((char*)&allowed_by_user_id, sizeof(allowed_by_user_id));

		return;
	};

	void read(std::ifstream& ifs) {
		ifs.read((char*)&id, sizeof(id));
		ifs.read((char*)&from_account_id, sizeof(from_account_id));
		ifs.read((char*)&to_account_id, sizeof(to_account_id));
		ifs.read((char*)&initieted_by_user_id, sizeof(initieted_by_user_id));
		ifs.read((char*)&status, sizeof(status));
		ifs.read((char*)&status_reason, sizeof(status_reason));
		ifs.read((char*)&request_timestamp, sizeof(request_timestamp));
		ifs.read((char*)&process_timestamp, sizeof(process_timestamp));
		ifs.read((char*)&amount, sizeof(amount));
		ifs.read((char*)&initiated_by_owner, sizeof(initiated_by_owner));
		ifs.read((char*)&PIN, sizeof(PIN));
		ifs.read((char*)&CVV, sizeof(CVV));
		ifs.read((char*)&comment_id, sizeof(comment_id));
		ifs.read((char*)&allowed_by_user_id, sizeof(allowed_by_user_id));

		return;
	};

	void updateInFile();
};


extern std::thread workerThread;