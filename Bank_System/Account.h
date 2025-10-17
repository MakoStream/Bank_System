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
account getAccount_byIBAN(const char* IBAN); // ����� ������� �� IBAN
account getAccount_byCardNumber(const char* cardNumber); // ����� ������� �� ������� ������

// Class: Account
// Description: Represents a bank account with card details, balance, and status
// Requirements: <string>, <fstream>, cardStatus, cardType, balanceType
// Required for: 
class account {  // ��� ��������� ������
	int userID;                      // ID �������� ������
	char IBAN[36];
	char cardNumber[16];
	char PIN[4];
	char CVV[3];
	char expirationDate[8];          // MM/YY
	balanceType balance_type = UAH;
	double balance = 0.0;
	cardType type = DEFAULT;
	short AccountType = 0;             // ���� ������� https://www.buhoblik.org.ua/uchet/organizacziya-buxgalterskogo-ucheta/388-plan-raxunkiv.html
	cardStatus status = NONVERIFED;
public:
	account() {} // ������� ����������� ��� load
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

	// ����� ������� ��� ���������� �����
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

	// ������
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
	
	// �������� � ��������
	void setBalance(double ammount) { balance += ammount; };

	void transfer(account to, double value) {
		double commission = 1.0;
		if (balance_type != to.getBalanceType()) {
			std::cerr << "�������: ������� ����� ��� ������." << std::endl;
			return;
		};
		if (balance < value + commission) {
			std::cerr << "�������: ����������� ����� �� ������� ����������." << std::endl;
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
	


	

	// ������� ��� ������ �� ���������� ����� � �������� ����� accounts.dat
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

// ������� ��� ������ � ����������� ���������
void ACC_createDB(); // ��������� ������� �������� ����� accounts.dat
void ACC_addAccount(int userID, balanceType balance_type, cardType type, short accountType); // ��������� ������ �������
account ACC_getAccountByCardNumber(const char* cardNumber); // ����� ������� �� ������� ������
account ACC_getAccountByIBAN(const char* IBAN); // ����� ������� �� IBAN
void transferFunds(account& fromAcc, account& toAcc, double amount); // ������� ����� �� ���������

int generateIBAN(); // ��������� ���������� IBAN
int generateCardNumber(); // ��������� ���������� ������ ������
void printAllAccounts(char msg[5][1024], int page); // ��������� ���������� ��� �� �������
void DB_create_accounts(); // ��������� ������� �������� ����� accounts.dat
bool isAccountExist_byIBAN(const char* IBAN); // �������� ��������� ������� �� IBAN
bool isAccountExist_byCardNumber(const char* cardNumber); // �������� ��������� ������� �� ������� ������
//account getAccount_byIBAN(const char* IBAN); // ����� ������� �� IBAN
//account getAccount_byCardNumber(const char* cardNumber); // ����� ������� �� ������� ������

//===============================================================================================================================
//===============================================================================================================================
//===============================================================================================================================

// reg_user login password name surname phone //example: reg_user StreamCompanyBank 9723424887 Mako Stream 0000000000
// new_account owner curence_type cardtype account_type //example: new_account 0 USD DEPOSITE 316
// ���� �������:

//������ ������� �볺��� : ������� 31 �����
	//313 � ������� � ����������� �����
	//314 � ������� � �������� �����
//�������� ������� �볺��� : ������� 31 �����
	//315 � �������� ������� � ����������� �����
	//316 � �������� ������� � �������� �����
//������� ������� �볺��� : ������� 31 �����
	//317 � ������� ������� � ����������� �����
	//318 � ������� ������� � �������� �����
// ���������� �������� ����� : ������� 36 �����
	//3651 � ���������� �������� �����


// ���� ������� ����� � ��������� ������������

// 20 ���� � ���������� ������������� �����
// 201 � ���������� ������������� �� ���������� � �볺�����
// 2011 � ���������� ������������� �볺��� UAH
// 2012 � ���������� ������������� �볺��� USD
// 2013 � ���������� ������������� �볺��� EUR

// 30 ���� � ���������� ����� �� �������� �볺���
// 303 � ���������� �����
// 3031 � ���������� ����� UAH
// 3032 � ���������� ����� USD
// 3033 � ���������� ����� EUR

// 70 ���� � ������ �����
// 701 � �������� ������ �� ��������� ��������
// 7011 � �������� ������ �� ������� UAH
// 7012 � �������� ������ �� ������� USD
// 7013 � �������� ������ �� ������� EUR

// 704 � ������ �� �������� ��������
// 7041 � ������ �� ����� UAH
// 7042 � ������ �� ����� USD
// 7043 � ������ �� ����� EUR

// 80 ���� � ������� �����
// 801 � ������� �� ������ ��������
// 8011 � ������� �� �������� UAH
// 8012 � ������� �� �������� USD
// 8013 � ������� �� �������� EUR