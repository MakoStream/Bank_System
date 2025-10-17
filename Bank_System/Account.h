#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "Logger.h"
#include <csignal>
#include <unordered_map>
#include <stdexcept>
#include <cstdio>
#include "User.h"


using namespace std;

enum cardStatus { AVAILABLE, BLOCKED, NONVERIFED};
enum cardType { DEPOSITE, DEFAULT, CREDIT };
enum balanceType { UAH, DLR, EUR };

extern std::unordered_map<std::string, cardType> cardMap;
extern std::unordered_map<std::string, balanceType> balanceMap;
extern std::unordered_map<std::string, cardStatus> statusMap;
extern std::unordered_map<cardStatus, std::string> statusMapReverse;

class account; // forward declaration
account getAccount_byIBAN(const char* IBAN); // Пошук рахунку за IBAN
account getAccount_byCardNumber(const char* cardNumber); // Пошук рахунку за номером картки

// Class: Account
// Description: Represents a bank account with card details, balance, and status
// Requirements: <string>, <fstream>, cardStatus, cardType, balanceType
// Required for: 
class account {  // Дані банківської картки
	int userID;                      // ID власника картки
	char IBAN[36];
	char cardNumber[16];
	char PIN[4];
	char CVV[3];
	char expirationDate[8];          // MM/YY
	balanceType balance_type = UAH;
	double balance = 0.0;
	cardType type = DEFAULT;
	short AccountType = 0;             // План рахунків https://www.buhoblik.org.ua/uchet/organizacziya-buxgalterskogo-ucheta/388-plan-raxunkiv.html
	cardStatus status = NONVERIFED;
public:
	account() {} // порожній конструктор для load
	account(const int _userID, const char* _IBAN, const char* _cardNumber, const char* _PIN,
		const char* _CVV, const char* _expirationDate,short _AccountType, balanceType _balance_type = UAH,
		cardType _type = DEFAULT, cardStatus _status = NONVERIFED)
		: userID(_userID), balance_type(_balance_type), type(_type), AccountType(_AccountType), status(_status)
	{
		strncpy(IBAN, _IBAN, sizeof(IBAN) - 1); IBAN[sizeof(IBAN) - 1] = '\0';
		strncpy(cardNumber, _cardNumber, sizeof(cardNumber) - 1); cardNumber[sizeof(cardNumber) - 1] = '\0';
		strncpy(PIN, _PIN, sizeof(PIN) - 1); PIN[sizeof(PIN) - 1] = '\0';
		strncpy(CVV, _CVV, sizeof(CVV) - 1); CVV[sizeof(CVV) - 1] = '\0';
		strncpy(expirationDate, _expirationDate, sizeof(expirationDate) - 1); expirationDate[sizeof(expirationDate) - 1] = '\0';
	}

	// Базові функції для повернення даних
	User getUser() { return getUser_byId(userID); };
	int getUserID() { return userID; };
	char* getIBAN() {return IBAN;}
	char* getPAN() {return cardNumber;};
	//char* getPIN() { return PIN; };
	//char* getCVV() { return CVV; };
	char* getExpirationDate() { return expirationDate; };
	balanceType getBalanceType() { return balance_type; };
	double getBalance() { return balance; };
	cardType getCardType() { return type; };
	short getAccountType() { return AccountType; };
	cardStatus getCardStatus() {return status;};

	// Доступ
	void verify() {
		if (status == NONVERIFED) status = AVAILABLE;
	}; 
	void unban() { status = AVAILABLE; };
	void ban(string reason) { 
		status = BLOCKED; 
	};
	void changePIN(const char* newPIN) { strncpy(PIN, newPIN, sizeof(PIN) - 1); PIN[sizeof(PIN) - 1] = '\0'; };

	bool checkPIN(const char* inputPIN) {
		return strncmp(PIN, inputPIN, sizeof(PIN)) == 0;
	}
	bool checkCVV(const char* inputCVV) {
		return strncmp(CVV, inputCVV, sizeof(CVV)) == 0;
	}
	
	// Операції з балансом
	void setBalance(double ammount) { balance += ammount; };

	void transfer(account to, double value) {
		double commission = 1.0;
		if (balance_type != to.getBalanceType()) {
			std::cerr << "Помилка: Рахунки мають різні валюти." << std::endl;
			return;
		};
		if (balance < value + commission) {
			std::cerr << "Помилка: Недостатньо коштів на рахунку відправника." << std::endl;
			return;
		};
		setBalance(-(value + commission));
		to.setBalance(value);
		account comission_bag = getAccount_byIBAN("TB000000000000000000000000010");
		comission_bag.setBalance(commission);
		comission_bag.updateInFile();
		this->updateInFile();
		to.updateInFile();
	};
	


	

	// Функції для запису та зчитування даних з бінарного файлу accounts.dat
	void save(std::ofstream& ofs) {
		ofs.write((char*)&userID, sizeof(userID));
		ofs.write((char*)&IBAN, sizeof(IBAN));
		ofs.write((char*)&cardNumber, sizeof(cardNumber));
		ofs.write((char*)&PIN, sizeof(PIN));
		ofs.write((char*)&CVV, sizeof(CVV));
		ofs.write((char*)&expirationDate, sizeof(expirationDate));
		ofs.write((char*)&balance_type, sizeof(balance_type));
		ofs.write((char*)&balance, sizeof(balance));
		ofs.write((char*)&type, sizeof(type));
		ofs.write((char*)&AccountType, sizeof(AccountType));
		ofs.write((char*)&status, sizeof(status));
	};
	void load(std::ifstream& ifs) {
		ifs.read((char*)&userID, sizeof(userID));
		ifs.read((char*)&IBAN, sizeof(IBAN));
		ifs.read((char*)&cardNumber, sizeof(cardNumber));
		ifs.read((char*)&PIN, sizeof(PIN));
		ifs.read((char*)&CVV, sizeof(CVV));
		ifs.read((char*)&expirationDate, sizeof(expirationDate));
		ifs.read((char*)&balance_type, sizeof(balance_type));
		ifs.read((char*)&balance, sizeof(balance));
		ifs.read((char*)&type, sizeof(type));
		ifs.read((char*)&AccountType, sizeof(AccountType));
		ifs.read((char*)&status, sizeof(status));
	};
	void updateInFile();
};

extern account emptyAccount;

// Функції для роботи з банківськими рахунками
void ACC_createDB(); // Створення пустого бінарного файлу accounts.dat
void ACC_addAccount(int userID, balanceType balance_type, cardType type, short accountType); // Додавання нового рахунку
account ACC_getAccountByCardNumber(const char* cardNumber); // Пошук рахунку за номером картки
account ACC_getAccountByIBAN(const char* IBAN); // Пошук рахунку за IBAN
void transferFunds(account& fromAcc, account& toAcc, double amount); // Переказ коштів між рахунками

int generateIBAN(); // Генерація унікального IBAN
int generateCardNumber(); // Генерація унікального номера картки
void printAllAccounts(char msg[5][1024], int page); // Виведення інформації про всі рахунки
void DB_create_accounts(); // Створення пустого бінарного файлу accounts.dat
bool isAccountExist_byIBAN(const char* IBAN); // Перевірка існування рахунку за IBAN
bool isAccountExist_byCardNumber(const char* cardNumber); // Перевірка існування рахунку за номером картки
//account getAccount_byIBAN(const char* IBAN); // Пошук рахунку за IBAN
//account getAccount_byCardNumber(const char* cardNumber); // Пошук рахунку за номером картки

//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================

// reg_user login password name surname phone //example: reg_user StreamCompanyBank 9723424887 Mako Stream 0000000000
// new_account owner curence_type cardtype account_type //example: new_account 0 USD DEPOSITE 316
// Типи рахунків:

//Поточні рахунки клієнтів : рахунки 31 класу
	//313 — Рахунки в національній валюті
	//314 — Рахунки в іноземній валюті
//Депозитні рахунки клієнтів : рахунки 31 класу
	//315 — Депозитні рахунки в національній валюті
	//316 — Депозитні рахунки в іноземній валюті
//Кредитні рахунки клієнтів : рахунки 31 класу
	//317 — Кредитні рахунки в національній валюті
	//318 — Кредитні рахунки в іноземній валюті
// Депонована заробітня плата : рахунок 36 класу
	//3651 — Депонована заробітня плата


// План рахунків банку з валютними субрахунками

// 20 клас — Дебіторська заборгованість банку
// 201 — Дебіторська заборгованість за операціями з клієнтами
// 2011 — Дебіторська заборгованість клієнтів UAH
// 2012 — Дебіторська заборгованість клієнтів USD
// 2013 — Дебіторська заборгованість клієнтів EUR

// 30 клас — Блокування коштів на рахунках клієнтів
// 303 — Блокування коштів
// 3031 — Блокування коштів UAH
// 3032 — Блокування коштів USD
// 3033 — Блокування коштів EUR

// 70 клас — Доходи банку
// 701 — Процентні доходи від кредитних операцій
// 7011 — Процентні доходи від кредитів UAH
// 7012 — Процентні доходи від кредитів USD
// 7013 — Процентні доходи від кредитів EUR

// 704 — Доходи від комісійних операцій
// 7041 — Доходи від комісій UAH
// 7042 — Доходи від комісій USD
// 7043 — Доходи від комісій EUR

// 80 клас — Витрати банку
// 801 — Витрати на оплату депозитів
// 8011 — Витрати на депозити UAH
// 8012 — Витрати на депозити USD
// 8013 — Витрати на депозити EUR