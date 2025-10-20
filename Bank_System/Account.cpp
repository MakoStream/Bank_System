#include "Account.h"
#include "mainProcess.h"


unordered_map <string, balanceType> balanceMap = {
    {"UAH", UAH},
    {"DLR", DLR},
    {"EUR", EUR}
};
unordered_map<string, cardType> cardMap = {
            {"DEFAULT", DEFAULT},
            {"DEPOSITE", DEPOSITE},
            {"CREDIT", CREDIT}
};

unordered_map<string, cardStatus> statusMap = {
            {"AVAILABLE", AVAILABLE},
            {"BLOCKED", BLOCKED},
            {"NONVERIFED", NONVERIFED}
};
unordered_map<cardStatus, string> statusMapReverse = {
    {AVAILABLE, "AVAILABLE"},
    {BLOCKED, "BLOCKED"},
    {NONVERIFED, "NONVERIFED"}
};

//TBkk12340101XXXXXXXXXXXX
//400000XXXXXXXXXCc
account emptyAccount;

// Обчислення контрольної цифри Luhn для рядка цифр (без контрольної цифри)
int computeLuhnCheckDigit(const char* digits_without_check) {
    int sum = 0;
    int n = (int)strlen(digits_without_check);
    // прохід справа наліво
    for (int i = 0; i < n; ++i) {
        int d = digits_without_check[n - 1 - i] - '0';
        if (i % 2 == 0) { // подвоюємо кожну другу цифру при обчисленні контрольної цифри
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

// Генерація 16-значного номера: "400000" + 9-digit body + Luhn check
char* get_BankID() {
    int bank_id = process.incrementCardPAN();

    char withoutCheck[16]; // 15 цифр + '\0'
    snprintf(withoutCheck, sizeof(withoutCheck), "400000%09d", bank_id);

    int check = computeLuhnCheckDigit(withoutCheck);

    // Виділяємо буфер для 16 цифр + '\0'
    char* bank_id_str = (char*)malloc(17);
    if (!bank_id_str) return nullptr;

    snprintf(bank_id_str, 17, "%s%d", withoutCheck, check);

    return bank_id_str; // Пам'ять треба буде вільно викликати free() після використання
}


char* get_IBAN() {
    int IBAN = process.incrementCardIBAN();

    // Виділяємо буфер для "TB00" + 25 цифр + '\0' = 30 символів
    char* IBAN_str = (char*)malloc(30);
    if (!IBAN_str) return nullptr;

    // Формуємо IBAN
    snprintf(IBAN_str, 30, "TB00%025d", IBAN);

    return IBAN_str; // Пам'ять треба буде звільнити після використання
}


char* generate_CVC() {
	int CVC = rand() % 1000; // випадкове число від 0 до 999
	char* CVC_str = new char[4]; // 3 цифри + '\0'
	snprintf(CVC_str, 4, "%03d", CVC); // форматування з провідними нулями
	return CVC_str;
}

account getLastAccount() {
    ifstream fin(process.getAccountDBPath(), ios::binary | ios::in);
    account empty{};

    if (!fin.is_open()) {
        cerr << "Не вдалося відкрити файл!" << endl;
        return empty;
    }

    fin.seekg(0, ios::end);
    streamoff fileSize = fin.tellg();

    // Перевірка на пустий файл або некратний розмір
    if (fileSize < static_cast<streamoff>(sizeof(account)) || fileSize % sizeof(account) != 0) {
        cerr << "Файл порожній або пошкоджений!" << endl;
        return empty;
    }

    fin.seekg(-static_cast<streamoff>(sizeof(account)), ios::end);

    account last{};
    fin.read(reinterpret_cast<char*>(&last), sizeof(account));

    if (!fin) {
        cerr << "Помилка читання останнього запису!" << endl;
        return empty;
    }

    return last;
}

void ACC_addAccount(int userID, balanceType balance_type, cardType type, short accountType) {
    std::ofstream fout(process.getAccountDBPath(), ios::binary | ios::app);
    if (!fout) {
        std::cerr << "Не вдалося відкрити файл для запису." << std::endl;
        return;
    }


    // Генерація унікальних IBAN та номера картки
    char* IBAN = get_IBAN();
    char* cardNumber = get_BankID();


    // Генерація випадкових PIN, CVV та дати закінчення терміну дії
    char PIN[5] = "----";
    char CVV[4] = "---";

    char expirationDate[8];                 // Формат MM/YY

    // Перевірка наявності користувача
    if (!isUserExist_byId(userID)) {
        std::cerr << "Користувач з ID " << userID << " не існує." << std::endl;
        fout.close();
        return;
    }

    // Створення нового рахунку
    account newAccount(
        0,
        userID,
        IBAN,
        cardNumber,
        PIN,
        CVV,
        expirationDate,
        accountType,       // AccountType
        balance_type,       // balance_type
        type,   // cardType
        NONVERIFED // cardStatus
    );


    // Запис нового рахунку у файл
    newAccount.save(fout);
    fout.close();
    delete[] IBAN;
    delete[] cardNumber;
};

void printAllAccounts(char msg[5][1024], int page) {
    ifstream fin(process.getAccountDBPath(), ios::binary);
    if (!fin) {
        cerr << "Не вдалося відкрити файл для читання." << endl;
        for (int i = 0; i < 5; i++) msg[i][0] = '\0';
        return;
    }

    account acc;
    int startIndex = (page - 1) * 25;
    int currentIndex = 0;
    int msgIndex = 0;
    string buffer;

    while (!fin.eof()) {
        acc.load(fin);

        if (currentIndex < startIndex) {
            currentIndex++;
            continue;
        }

        stringstream ss;
        ss << "IBAN: " << acc.getIBAN()
            << " | PAN: " << acc.getPAN()
            << " | Status: " << statusMapReverse[acc.getCardStatus()]
            << " | Owner ID: " << acc.getUserID()
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

    for (; msgIndex < 5; msgIndex++) msg[msgIndex][0] = '\0';

    fin.close();
}


void DB_create_accounts() {
	std::ofstream fout(process.getAccountDBPath(), ios::binary | ios::trunc);
	if (!fout) {
		std::cerr << "Не вдалося створити файл." << std::endl;
		return;
	}
	std::cout << "Бінарний файл accounts.dat створено успішно." << std::endl;
	fout.close();

    // створення базових рахунків банку
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

// Перевірка існування рахунку за номером картки
bool isAccountExist_byCardNumber(const char* cardNumber) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) {
        std::cerr << "Не вдалося відкрити файл для читання." << std::endl;
        return false;
    }

    account acc;
    while (true) {
        std::streampos pos = fin.tellg();
        acc.load(fin);
        if (fin.eof()) break;

        if (strcmp(acc.getPAN(), cardNumber) == 0) {
            fin.close();
            return true;
        }
    }

    fin.close();
    return false;
}

// Перевірка існування рахунку за IBAN
bool isAccountExist_byIBAN(const char* IBAN) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) {
        std::cerr << "Не вдалося відкрити файл для читання." << std::endl;
        return false;
    }

    account acc;
    while (true) {
        std::streampos pos = fin.tellg();
        acc.load(fin);
        if (fin.eof()) break;

        if (strcmp(acc.getIBAN(), IBAN) == 0) {
            fin.close();
            return true;
        }
    }

    fin.close();
    return false;
}

// Отримати рахунок за номером картки
account getAccount_byCardNumber(const char* cardNumber) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) {
        std::cerr << "Не вдалося відкрити файл для читання." << std::endl;
        return emptyAccount;
    }

    account acc;
    while (true) {
        std::streampos pos = fin.tellg();
        acc.load(fin);
        if (fin.eof()) break;

        if (strcmp(acc.getPAN(), cardNumber) == 0) {
            fin.close();
            return acc;
        }
    }

    fin.close();
    return emptyAccount;
}

// Отримати рахунок за IBAN
account getAccount_byIBAN(const char* IBAN) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) {
        std::cerr << "Не вдалося відкрити файл для читання." << std::endl;
        return emptyAccount;
    }

    account acc;
    while (true) {
        std::streampos pos = fin.tellg();
        acc.load(fin);
        if (fin.eof()) break;

        if (strcmp(acc.getIBAN(), IBAN) == 0) {
            fin.close();
            return acc;
        }
    }

    fin.close();
    return emptyAccount;
}

void account::updateInFile() {
    // 1. Зчитуємо всі записи з файлу
    std::ifstream inFile("accounts.dat", std::ios::binary);
    if (!inFile) {
        std::cerr << "Помилка: не вдалося відкрити файл accounts.dat для читання." << std::endl;
        return;
    }

    std::vector<account> all;
    account temp;

    // коректне зчитування через load
    while (true) {
        temp = account(); // очищаємо перед зчитуванням
        temp.load(inFile);
        if (!inFile) break; // якщо зчитування не вдалося — вихід
        all.push_back(temp);
    }
    inFile.close();

    // 2. Оновлюємо потрібний запис
    bool found = false;
    for (auto& acc : all) {
        if (strcmp(acc.getIBAN(), this->getIBAN()) == 0) { // строго по IBAN
            acc = *this; // оновлюємо весь об'єкт
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "Попередження: рахунок із таким IBAN не знайдено." << std::endl;
        return;
    }

    // 3. Перезаписуємо файл
    std::ofstream outFile(process.getAccountDBPath(), std::ios::binary | std::ios::trunc);
    if (!outFile) {
        std::cerr << "Помилка: не вдалося відкрити файл accounts.dat для запису." << std::endl;
        return;
    }

    for (auto& acc : all)
        acc.save(outFile);

    outFile.close();
}



