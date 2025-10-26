#include "Account.h"
#include "mainProcess.h"

// Objects: balanceMap, cardMap, statusMap, statusMapReverse
// Description: These maps provide conversions between string identifiers and corresponding enum values for balances, card types, and card statuses.
// Requirements: <string>, <unordered_map>, enums balanceType, cardType, cardStatus
// Required for: account management functions, printing, validation
unordered_map <string, balanceType> balanceMap = { {"UAH", UAH}, {"DLR", DLR}, {"EUR", EUR} };
unordered_map<string, cardType> cardMap = { {"DEFAULT", DEFAULT}, {"DEPOSITE", DEPOSITE}, {"CREDIT", CREDIT} };
unordered_map<string, cardStatus> statusMap = { {"AVAILABLE", AVAILABLE}, {"BLOCKED", BLOCKED}, {"NONVERIFED", NONVERIFED} };
unordered_map<cardStatus, string> statusMapReverse = { {AVAILABLE, "AVAILABLE"}, {BLOCKED, "BLOCKED"}, {NONVERIFED, "NONVERIFED"} };

// Empty account object
// Requirements: account class
// Required for: returning default account if not found
account emptyAccount;

// Function: computeLuhnCheckDigit
// Description: Calculates the Luhn check digit for a numeric string (excluding the check digit itself).
// Requirements: <cstring>
// Required for: get_BankID()
int computeLuhnCheckDigit(const char* digits_without_check) {
    int sum = 0;
    int n = (int)strlen(digits_without_check);
    for (int i = 0; i < n; ++i) {
        int d = digits_without_check[n - 1 - i] - '0';
        if (i % 2 == 0) {
            int dd = d * 2;
            if (dd > 9) dd -= 9;
            sum += dd;
        }
        else {
            sum += d;
        }
    }
    return (10 - (sum % 10)) % 10;
}

// Function: get_BankID
// Description: Generates a 16-digit bank card number using "400000" + 9-digit body + Luhn check digit.
// Requirements: <cstdlib>, <cstdio>, computeLuhnCheckDigit(), process.incrementCardPAN()
// Required for: ACC_addAccount()
char* get_BankID() {
    int bank_id = process.incrementCardPAN();
    char withoutCheck[16];
    snprintf(withoutCheck, sizeof(withoutCheck), "400000%09d", bank_id);
    int check = computeLuhnCheckDigit(withoutCheck);
    char* bank_id_str = (char*)malloc(17);
    if (!bank_id_str) return nullptr;
    snprintf(bank_id_str, 17, "%s%d", withoutCheck, check);
    return bank_id_str;
}

// Function: get_IBAN
// Description: Generates a pseudo IBAN string in the format "TB00" + 25-digit number.
// Requirements: <cstdlib>, <cstdio>, process.incrementCardIBAN()
// Required for: ACC_addAccount()
char* get_IBAN() {
    int IBAN = process.incrementCardIBAN();
    char* IBAN_str = (char*)malloc(30);
    if (!IBAN_str) return nullptr;
    snprintf(IBAN_str, 30, "TB00%025d", IBAN);
    return IBAN_str;
}

// Function: generate_CVC
// Description: Generates a random 3-digit CVC code for a card.
// Requirements: <cstdlib>, <cstdio>, rand()
// Required for: ACC_addAccount()
char* generate_CVC() {
    int CVC = rand() % 1000;
    char* CVC_str = new char[4];
    snprintf(CVC_str, 4, "%03d", CVC);
    return CVC_str;
}

// Function: getLastAccount
// Description: Reads the last account from the account database file.
// Requirements: <fstream>, account class, process.getAccountDBPath()
// Required for: ACC_addAccount()
account getLastAccount() {
    ifstream fin(process.getAccountDBPath(), ios::binary | ios::in);
    account empty{};
    if (!fin.is_open()) { cerr << "Не вдалося відкрити файл!" << endl; return empty; }
    fin.seekg(0, ios::end);
    streamoff fileSize = fin.tellg();
    if (fileSize < static_cast<streamoff>(sizeof(account)) || fileSize % sizeof(account) != 0) {
        cerr << "Файл порожній або пошкоджений!" << endl;
        return empty;
    }
    fin.seekg(-static_cast<streamoff>(sizeof(account)), ios::end);
    account last{};
    fin.read(reinterpret_cast<char*>(&last), sizeof(account));
    if (!fin) { cerr << "Помилка читання останнього запису!" << endl; return empty; }
    return last;
}

// Function: getAccountById
// Description: Finds an account by its ID.
// Requirements: <fstream>, account class, process.getAccountDBPath()
// Required for: 
account getAccountById(int id) {
    ifstream fin(process.getAccountDBPath(), ios::binary);
    account acc;
    while (fin.read(reinterpret_cast<char*>(&acc), sizeof(account))) {
        if (acc.getId() == id) {
            fin.close();
            return acc;
        }
    }
    fin.close();
    return emptyAccount;
}

// Function: isAccountExisytById
// Description: Checks if an account exists by ID.
// Requirements: <fstream>, account class, process.getAccountDBPath()
// Required for: 
bool isAccountExisytById(int id) {
    ifstream fin(process.getAccountDBPath(), ios::binary);
    account acc;
    while (fin.read(reinterpret_cast<char*>(&acc), sizeof(account))) {
        if (acc.getId() == id) {
            fin.close();
            return true;
        }
    }
    fin.close();
    return false;
}

// Function: ACC_addAccount
// Description: Creates a new account and saves it to the database file.
// Requirements: get_IBAN(), get_BankID(), generate_CVC(), getLastAccount(), isUserExist_byId(), account class, <fstream>
// Required for: DB_create_accounts(), class NewAccountCommand
void ACC_addAccount(int userID, balanceType balance_type, cardType type, short accountType) {
    std::ofstream fout(process.getAccountDBPath(), ios::binary | ios::app);
    if (!fout) { std::cerr << "Не вдалося відкрити файл для запису." << std::endl; return; }

    char* IBAN = get_IBAN();
    char* cardNumber = get_BankID();

    char PIN[5] = "----";
    char CVV[4] = "---";
    char expirationDate[8];

    if (!isUserExist_byId(userID)) {
        std::cerr << "Користувач з ID " << userID << " не існує." << std::endl;
        fout.close();
        return;
    }

    account newAccount(
        getLastAccount().getId() + 1,
        userID,
        IBAN,
        cardNumber,
        PIN,
        CVV,
        expirationDate,
        accountType,
        balance_type,
        type,
        NONVERIFED
    );

    newAccount.save(fout);
    fout.close();
    delete[] IBAN;
    delete[] cardNumber;
}

// Function: printAllAccounts
// Description: Reads accounts from the database and formats them into pages of messages.
// Requirements: <fstream>, <sstream>, account class, statusMapReverse, process.getAccountDBPath()
// Required for: class AccountListCommand
void printAllAccounts(char msg[5][1024], int page) {
    ifstream fin(process.getAccountDBPath(), ios::binary);
    if (!fin) { cerr << "Не вдалося відкрити файл для читання." << endl; for (int i = 0; i < 5; i++) msg[i][0] = '\0'; return; }

    account acc;
    int startIndex = (page - 1) * 25;
    int currentIndex = 0;
    int msgIndex = 0;
    string buffer;

    while (!fin.eof()) {
        acc.load(fin);
        if (currentIndex < startIndex) { currentIndex++; continue; }

        stringstream ss;
        ss << "id: " << acc.getId()
            << " | IBAN: " << acc.getIBAN()
            << " | PAN: " << acc.getPAN()
            << " | Status: " << statusMapReverse[acc.getCardStatus()]
            << " | Owner ID: " << acc.getUserID() << "\n";
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

// Function: DB_create_accounts
// Description: Creates the initial accounts database and populates it with default bank accounts.
// Requirements: ACC_addAccount(), account class, <fstream>
// Required for: class CreateDBCommand
void DB_create_accounts() {
    std::ofstream fout(process.getAccountDBPath(), ios::binary | ios::trunc);
    if (!fout) { std::cerr << "Не вдалося створити файл." << std::endl; return; }
    std::cout << "Бінарний файл accounts.dat створено успішно." << std::endl;
    fout.close();

    ACC_addAccount(0, UAH, DEFAULT, 2011);
    ACC_addAccount(0, DLR, DEFAULT, 2012);
    ACC_addAccount(0, EUR, DEFAULT, 2013);
    ACC_addAccount(0, UAH, DEFAULT, 3011);
    ACC_addAccount(0, DLR, DEFAULT, 3012);
    ACC_addAccount(0, EUR, DEFAULT, 3013);
    ACC_addAccount(0, UAH, DEFAULT, 7011);
    ACC_addAccount(0, DLR, DEFAULT, 7012);
    ACC_addAccount(0, EUR, DEFAULT, 7013);
    ACC_addAccount(0, UAH, DEFAULT, 7041);
    ACC_addAccount(0, DLR, DEFAULT, 7042);
    ACC_addAccount(0, EUR, DEFAULT, 7043);
    ACC_addAccount(0, UAH, DEFAULT, 8011);
    ACC_addAccount(0, DLR, DEFAULT, 8012);
    ACC_addAccount(0, EUR, DEFAULT, 8013);
}

// Function: isAccountExist_byCardNumber
// Description: Checks if an account exists by card number.
// Requirements: <fstream>, account class, process.getAccountDBPath()
// Required for: class AccountInfoCommand
bool isAccountExist_byCardNumber(const char* cardNumber) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) { std::cerr << "Не вдалося відкрити файл для читання." << std::endl; return false; }
    account acc;
    while (true) {
        acc.load(fin);
        if (fin.eof()) break;
        if (strcmp(acc.getPAN(), cardNumber) == 0) { fin.close(); return true; }
    }
    fin.close();
    return false;
}

// Function: isAccountExist_byIBAN
// Description: Checks if an account exists by IBAN.
// Requirements: <fstream>, account class, process.getAccountDBPath()
// Required for: class AccountBanCommand, class AccountVerifyCommand, class AccountInfoCommand
bool isAccountExist_byIBAN(const char* IBAN) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) { std::cerr << "Не вдалося відкрити файл для читання." << std::endl; return false; }
    account acc;
    while (true) {
        acc.load(fin);
        if (fin.eof()) break;
        if (strcmp(acc.getIBAN(), IBAN) == 0) { fin.close(); return true; }
    }
    fin.close();
    return false;
}

// Function: getAccount_byCardNumber
// Description: Retrieves an account object by card number.
// Requirements: <fstream>, account class, process.getAccountDBPath()
// Required for: class AccountInfoCommand
account getAccount_byCardNumber(const char* cardNumber) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) { std::cerr << "Не вдалося відкрити файл для читання." << std::endl; return emptyAccount; }
    account acc;
    while (true) {
        acc.load(fin);
        if (fin.eof()) break;
        if (strcmp(acc.getPAN(), cardNumber) == 0) { fin.close(); return acc; }
    }
    fin.close();
    return emptyAccount;
}

// Function: getAccount_byIBAN
// Description: Retrieves an account object by IBAN.
// Requirements: <fstream>, account class, process.getAccountDBPath()
// Required for: account.transfer(), class AccountBanCommand, class AccountInfoCommand, class AccountVerifyCommand
account getAccount_byIBAN(const char* IBAN) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) { std::cerr << "Не вдалося відкрити файл для читання." << std::endl; return emptyAccount; }
    account acc;
    while (true) {
        acc.load(fin);
        if (fin.eof()) break;
        if (strcmp(acc.getIBAN(), IBAN) == 0) { fin.close(); return acc; }
    }
    fin.close();
    return emptyAccount;
}

// Method: account::updateInFile
// Description: Updates the current account object in the accounts database file.
// Requirements: <fstream>, <vector>, account class, process.getAccountDBPath()
// Required for: class account
void account::updateInFile() {
    std::ifstream inFile(process.getAccountDBPath(), std::ios::binary);
    if (!inFile) { std::cerr << "Помилка: не вдалося відкрити файл accounts.dat для читання." << std::endl; return; }

    std::vector<account> all;
    account temp;

    while (true) {
        temp = account();
        temp.load(inFile);
        if (!inFile) break;
        all.push_back(temp);
    }
    inFile.close();

    bool found = false;
    for (auto& acc : all) {
        if (strcmp(acc.getIBAN(), this->getIBAN()) == 0) {
            acc = *this;
            found = true;
            break;
        }
    }

    if (!found) { std::cerr << "Попередження: рахунок із таким IBAN не знайдено." << std::endl; return; }

    std::ofstream outFile(process.getAccountDBPath(), std::ios::binary | std::ios::trunc);
    if (!outFile) { std::cerr << "Помилка: не вдалося відкрити файл accounts.dat для запису." << std::endl; return; }
    for (auto& acc : all) acc.save(outFile);
    outFile.close();
}
