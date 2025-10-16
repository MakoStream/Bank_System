#include "Account.h"


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
    int bank_id = 0;
    char* bank_id_str = new char[17]; // 16 цифр + '\0'
    memset(bank_id_str, 0, 17);

    // Зчитуємо лічильник
    std::ifstream in("bank_id.bin", std::ios::binary);
    if (in.is_open()) {
        in.read(reinterpret_cast<char*>(&bank_id), sizeof(bank_id));
        in.close();
    }
    else {
        // Якщо файла немає — створюємо з 0
        std::ofstream newFile("bank_id.bin", std::ios::binary);
        bank_id = 0;
        newFile.write(reinterpret_cast<const char*>(&bank_id), sizeof(bank_id));
        newFile.close();
    }

    // Інкрементуємо та записуємо назад
    bank_id++;
    std::ofstream out("bank_id.bin", std::ios::binary | std::ios::trunc);
    if (!out) {
        delete[] bank_id_str;
        throw std::runtime_error("Не вдалося відкрити bank_id.bin для запису");
    }
    out.write(reinterpret_cast<const char*>(&bank_id), sizeof(bank_id));
    out.close();

    // Формуємо 15-значну основу: "400000" + 9 цифр body
    char withoutCheck[16]; // 15 цифр + '\0'
    snprintf(withoutCheck, sizeof(withoutCheck), "400000%09d", bank_id); // гарантує 9 цифр з провідними нулями

    // Обчислюємо Luhn-контрольну цифру
    int check = computeLuhnCheckDigit(withoutCheck);

    // Формуємо остаточний 16-значний рядок
    snprintf(bank_id_str, 17, "%s%d", withoutCheck, check);

    return bank_id_str;
}

char* get_IBAN() {
    int IBAN = 0;
    char* IBAN_str = new char[36];
    memset(IBAN_str, 0, 36);

    std::ifstream in("IBAN.bin", std::ios::binary);
    if (in.is_open()) {
        in.read(reinterpret_cast<char*>(&IBAN), sizeof(IBAN));
        in.close();
    }
    else {
        std::ofstream newFile("IBAN.bin", std::ios::binary);
        IBAN = 0;
        newFile.write(reinterpret_cast<const char*>(&IBAN), sizeof(IBAN));
        newFile.close();
    }

    IBAN++;

    std::ofstream out("IBAN.bin", std::ios::binary | std::ios::trunc);
    out.write(reinterpret_cast<const char*>(&IBAN), sizeof(IBAN));
    out.close();

    // Формуємо IBAN у форматі TB00xxxxxxxxxxxxxxx
    snprintf(IBAN_str, 36, "TB001234%014d", IBAN);

    return IBAN_str;
}

char* generate_CVC() {
	int CVC = rand() % 1000; // випадкове число від 0 до 999
	char* CVC_str = new char[4]; // 3 цифри + '\0'
	snprintf(CVC_str, 4, "%03d", CVC); // форматування з провідними нулями
	return CVC_str;
}

void ACC_addAccount(int userID, balanceType balance_type, cardType type, short accountType) {
    std::ofstream fout("accounts.dat", ios::binary | ios::app);
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
}

void printAllAccounts(char msg[5][1024], int page) {
    ifstream fin("accounts.dat", ios::binary);
    if (!fin) {
        cerr << "Не вдалося відкрити файл для читання." << endl;
        for (int i = 0; i < 5; i++) msg[i][0] = '\0';
        return;
    }

    account acc;
    int startIndex = (page - 1) * 25; // пропускаємо попередні 25 записів
    int currentIndex = 0;
    int msgIndex = 0;
    string buffer;

    while (true) {
        acc.load(fin);
        if (fin.eof()) break;

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

        // Якщо накопичилось 5 записів або довжина > 900 символів
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
	std::ofstream fout("accounts.dat", ios::binary | ios::trunc);
	if (!fout) {
		std::cerr << "Не вдалося створити файл." << std::endl;
		return;
	}
	std::cout << "Бінарний файл accounts.dat створено успішно." << std::endl;
	fout.close();
}


