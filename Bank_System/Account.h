/**
 * @file Account.h
 * @brief Bank account class and functions for account management.
 * @note Required headers: <iostream>, <windows.h>, <vector>, <fstream>, <sstream>, "Logger.h", <csignal>, <unordered_map>, <stdexcept>, <cstdio>, "User.h"
 */

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
extern std::unordered_map <balanceType, string> balanceMapToString;
extern std::unordered_map<std::string, balanceType> balanceMap;
extern std::unordered_map<std::string, cardStatus> statusMap;
extern std::unordered_map<cardStatus, std::string> statusMapReverse;

class account; // forward declaration
account getAccount_byIBAN(const char* IBAN); // Пошук рахунку за IBAN
account getAccount_byCardNumber(const char* cardNumber); // Пошук рахунку за номером картки

/**
 * @class account
 * @brief Represents a bank account with IBAN, PAN, PIN, CVV, balance, type, status, and owner ID.
 * @note Required headers: <iostream>, <fstream>, <sstream>, "User.h", <cstring>
 */
class account {  // Дані банківської картки
	int id;
	int userID;                      // ID власника картки
	char IBAN[37];
	char cardNumber[17];
	char PIN[4];
	char CVV[3];
	char expirationDate[8];          // MM/YY
	balanceType balance_type = UAH;
	double balance = 0.0;
	cardType type = DEFAULT;
	short AccountType = 0;             // План рахунків https://www.buhoblik.org.ua/uchet/organizacziya-buxgalterskogo-ucheta/388-plan-raxunkiv.html
	cardStatus status = NONVERIFED;
public:
	/**
	 * @brief Default constructor for empty account or loading from file.
	 * @note Requires <cstring>
	 */
	account() {} // порожній конструктор для load

	/**
     * @brief Full constructor for creating an account
     * @param _id Unique account ID
     * @param _userID Owner's user ID
     * @param _IBAN IBAN string
     * @param _cardNumber Card number string (PAN)
     * @param _PIN PIN code
     * @param _CVV CVV code
     * @param _expirationDate Card expiration date
     * @param _AccountType Accounting plan type
     * @param _balance_type Currency type
     * @param _type Card type
     * @param _status Card status
     * @note Requires <cstring>
     */
	account(const int _id, const int _userID, const char* _IBAN, const char* _cardNumber, const char* _PIN,
		const char* _CVV, const char* _expirationDate,short _AccountType, balanceType _balance_type = UAH,
		cardType _type = DEFAULT, cardStatus _status = NONVERIFED)
		: id(_id), userID(_userID), balance_type(_balance_type), type(_type), AccountType(_AccountType), status(_status)
	{
		strncpy(IBAN, _IBAN, sizeof(IBAN) - 1); IBAN[sizeof(IBAN) - 1] = '\0';
		strncpy(cardNumber, _cardNumber, sizeof(cardNumber) - 1); cardNumber[sizeof(cardNumber) - 1] = '\0';
		strncpy(PIN, _PIN, sizeof(PIN) - 1); PIN[sizeof(PIN) - 1] = '\0';
		strncpy(CVV, _CVV, sizeof(CVV) - 1); CVV[sizeof(CVV) - 1] = '\0';
		strncpy(expirationDate, _expirationDate, sizeof(expirationDate) - 1); expirationDate[sizeof(expirationDate) - 1] = '\0';
	}

    /** @name Getters */
    ///@{
    int getId() { return id; }                               ///< Returns account ID
    User getUser() { return getUser_byId(userID); }          ///< Returns user object
    int getUserID() { return userID; }                       ///< Returns owner ID
    char* getIBAN() { return IBAN; }                         ///< Returns IBAN
    char* getPAN() { return cardNumber; }                    ///< Returns card number
    char* getPIN() { return PIN; }                            ///< Returns PIN
    char* getCVV() { return CVV; }                           ///< Returns CVV
    char* getExpirationDate() { return expirationDate; }     ///< Returns expiration date
    balanceType getBalanceType() { return balance_type; }    ///< Returns currency type
    double getBalance() { return balance; }                  ///< Returns balance
    cardType getCardType() { return type; }                  ///< Returns card type
    short getAccountType() { return AccountType; }          ///< Returns account plan type
    cardStatus getCardStatus() { return status; }           ///< Returns card status
    ///@}

    /** @name Status and access */
    ///@{
    void verify() { if (status == NONVERIFED) status = AVAILABLE; } ///< Set status from NONVERIFED to AVAILABLE
    void unban() { status = AVAILABLE; }                            ///< Set status to AVAILABLE
    void ban(std::string reason) { status = BLOCKED; }              ///< Set status to BLOCKED
    ///@}

    /** @name PIN/CVV operations */
    ///@{
    void changePIN(const char* newPIN) { strncpy(PIN, newPIN, sizeof(PIN) - 1); PIN[sizeof(PIN) - 1] = '\0'; } ///< Change PIN
    void setPIN(const char* newPIN) { strncpy(PIN, newPIN, sizeof(PIN) - 1); PIN[sizeof(PIN) - 1] = '\0'; }    ///< Set new PIN
    void setCVV(const char* newCVV) { strncpy(CVV, newCVV, sizeof(CVV) - 1); CVV[sizeof(CVV) - 1] = '\0'; }   ///< Set new CVV
    bool checkPIN(const char* inputPIN) { return strncmp(PIN, inputPIN, sizeof(PIN)) == 0; }                  ///< Check input PIN
    bool checkCVV(const char* inputCVV) { return strncmp(CVV, inputCVV, sizeof(CVV)) == 0; }                  ///< Check input CVV
    ///@}

    /** @name Balance operations */
    ///@{
    void setBalance(double amount) { balance += amount; } ///< Add or remove funds
    ///@}

    /** @name File operations */
    ///@{
    void save(std::ofstream& ofs) {
        ofs.write((char*)&id, sizeof(id));
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
    } ///< Write account to file

    void load(std::ifstream& ifs) {
        ifs.read((char*)&id, sizeof(id));
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
    } ///< Read account from file

    void updateInFile(); ///< Update account record in file
    ///@}
};

extern account emptyAccount;

// Функції для роботи з банківськими рахунками
void ACC_createDB(); // Створення пустого бінарного файлу accounts.dat

/**
 * @brief Adds a new account and saves it to the database
 * @param userID Owner user ID
 * @param balance_type Currency type
 * @param type Card type
 * @param accountType Account plan type
 * @see DB_create_accounts()
 */
void ACC_addAccount(int userID, balanceType balance_type, cardType type, short accountType); // Додавання нового рахунку

/**
 * @brief Find account by card number.
 * @param cardNumber Card number (PAN)
 * @return account object, or emptyAccount if not found
 * @note Requires account class and <fstream>
 */
account ACC_getAccountByCardNumber(const char* cardNumber); // Пошук рахунку за номером картки


/**
 * @brief Find account by IBAN.
 * @param IBAN IBAN of the account
 * @return account object, or emptyAccount if not found
 * @note Requires account class and <fstream>
 */
account ACC_getAccountByIBAN(const char* IBAN); // Пошук рахунку за IBAN

/**
 * @brief Transfer funds between accounts
 * @note Requires account class
 */
void transferFunds(account& fromAcc, account& toAcc, double amount); // Переказ коштів між рахунками



/**
 * @brief Print accounts into paged messages
 * @note Requires account class, <fstream>, <sstream>
 */
void printAllAccounts(char msg[5][1024], int page);

/**
 * @brief Create empty accounts file
 * @note Requires ACC_addAccount()
 */
void DB_create_accounts();

/**
 * @brief Check if account exists by IBAN
 * @note Requires account class, <fstream>
 */
bool isAccountExist_byIBAN(const char* IBAN);

/**
 * @brief Check if account exists by card number
 * @note Requires account class, <fstream>
 */
bool isAccountExist_byCardNumber(const char* cardNumber);

/**
 * @brief Get account by ID
 * @note Requires account class, <fstream>
 */
account getAccountById(int id);

/**
 * @brief Check if account exists by ID
 * @note Requires account class, <fstream>
 */
bool isAccountExistById(int id);

/**
 * @brief Get last account in database
 * @note Requires account class, <fstream>
 * @see ACC_addAccount()
 */
account getLastAccount();

/**
 * @brief Get all accounts of a user
 * @note Requires account class, <fstream>
 */
vector<account> getUserAccounts(int user_id);

/**
 * @brief Debug: set account balance
 * @note Requires account class
 */
void setAccountBalance(account& acc, double newBalance);


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