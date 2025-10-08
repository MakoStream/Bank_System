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
#include "User.h"

using namespace std;

enum cardStatus { AVAILABLE, BLOCKED, NONVERIFED};
enum cardType { DEPOSITE, DEFAULT, CREDIT };
enum balanceType { UAH, DLR, EUR };

extern std::unordered_map<std::string, cardType> cardMap;
extern std::unordered_map<std::string, balanceType> balanceMap;


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
	cardStatus status = NONVERIFED;
public:
	account() {} // порожній конструктор для load
	account(const int _userID, const char* _IBAN, const char* _cardNumber, const char* _CVV, const char* _expirationDate,
		balanceType _balance_type = UAH, double _balance = 0.0, cardType _type = DEFAULT, cardStatus _status = NONVERIFED)
		: userID(_userID), balance_type(_balance_type), balance(_balance), type(_type), status(_status)
	{
		strncpy(IBAN, _IBAN, sizeof(IBAN) - 1);
		strncpy(cardNumber, _cardNumber, sizeof(cardNumber) - 1);
		strncpy(CVV, _CVV, sizeof(CVV) - 1);
		strncpy(expirationDate, _expirationDate, sizeof(expirationDate) - 1);
	};

	// Базові функції для повернення даних
	User getUser() { return getUser_byId(userID); };
	char* getIBAN() {return IBAN;}
	char* getCardNumber() {return cardNumber;};
	//char* getPIN() { return PIN; };
	char* getCVV() { return CVV; };
	char* getExpirationDate() { return expirationDate; };
	balanceType getBalanceType() { return balance_type; };
	double getBalance() { return balance; };
	cardType getCardType() { return type; };
	cardStatus getCardStatus() {return status;};



	

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
		ifs.read((char*)&status, sizeof(status));
	};
};

// Функції для роботи з банківськими рахунками
void ACC_createDB(); // Створення пустого бінарного файлу accounts.dat
void ACC_addAccount(account acc); // Додавання нового рахунку в accounts.dat
account ACC_getAccountByCardNumber(const char* cardNumber); // Пошук рахунку за номером картки
account ACC_getAccountByIBAN(const char* IBAN); // Пошук рахунку за IBAN
void transferFunds(account& fromAcc, account& toAcc, double amount); // Переказ коштів між рахунками