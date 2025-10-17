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

// ���������� ���������� ����� Luhn ��� ����� ���� (��� ���������� �����)
int computeLuhnCheckDigit(const char* digits_without_check) {
    int sum = 0;
    int n = (int)strlen(digits_without_check);
    // ������ ������ �����
    for (int i = 0; i < n; ++i) {
        int d = digits_without_check[n - 1 - i] - '0';
        if (i % 2 == 0) { // ��������� ����� ����� ����� ��� ��������� ���������� �����
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

// ��������� 16-�������� ������: "400000" + 9-digit body + Luhn check
char* get_BankID() {
    int bank_id = process.incrementCardPAN();

    char withoutCheck[16]; // 15 ���� + '\0'
    snprintf(withoutCheck, sizeof(withoutCheck), "400000%09d", bank_id);

    int check = computeLuhnCheckDigit(withoutCheck);

    // �������� ����� ��� 16 ���� + '\0'
    char* bank_id_str = (char*)malloc(17);
    if (!bank_id_str) return nullptr;

    snprintf(bank_id_str, 17, "%s%d", withoutCheck, check);

    return bank_id_str; // ���'��� ����� ���� ����� ��������� free() ���� ������������
}


char* get_IBAN() {
    int IBAN = process.incrementCardIBAN();

    // �������� ����� ��� "TB00" + 25 ���� + '\0' = 30 �������
    char* IBAN_str = (char*)malloc(30);
    if (!IBAN_str) return nullptr;

    // ������� IBAN
    snprintf(IBAN_str, 30, "TB00%025d", IBAN);

    return IBAN_str; // ���'��� ����� ���� �������� ���� ������������
}


char* generate_CVC() {
	int CVC = rand() % 1000; // ��������� ����� �� 0 �� 999
	char* CVC_str = new char[4]; // 3 ����� + '\0'
	snprintf(CVC_str, 4, "%03d", CVC); // ������������ � ��������� ������
	return CVC_str;
}

void ACC_addAccount(int userID, balanceType balance_type, cardType type, short accountType) {
    std::ofstream fout(process.getAccountDBPath(), ios::binary | ios::app);
    if (!fout) {
        std::cerr << "�� ������� ������� ���� ��� ������." << std::endl;
        return;
    }


    // ��������� ��������� IBAN �� ������ ������
    char* IBAN = get_IBAN();
    char* cardNumber = get_BankID();


    // ��������� ���������� PIN, CVV �� ���� ��������� ������ 䳿
    char PIN[5] = "----";
    char CVV[4] = "---";

    char expirationDate[8];                 // ������ MM/YY

    // �������� �������� �����������
    if (!isUserExist_byId(userID)) {
        std::cerr << "���������� � ID " << userID << " �� ����." << std::endl;
        fout.close();
        return;
    }

    // ��������� ������ �������
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


    // ����� ������ ������� � ����
	newAccount.save(fout);
	fout.close();
	delete[] IBAN;
	delete[] cardNumber;
}

void printAllAccounts(char msg[5][1024], int page) {
    ifstream fin(process.getAccountDBPath(), ios::binary);
    if (!fin) {
        cerr << "�� ������� ������� ���� ��� �������." << endl;
        for (int i = 0; i < 5; i++) msg[i][0] = '\0';
        return;
    }

    account acc;
    int startIndex = (page - 1) * 25; // ���������� �������� 25 ������
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

        // ���� ������������ 5 ������ ��� ������� > 900 �������
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
		std::cerr << "�� ������� �������� ����." << std::endl;
		return;
	}
	std::cout << "�������� ���� accounts.dat �������� ������." << std::endl;
	fout.close();

    // ��������� ������� ������� �����
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

// �������� ��������� ������� �� ������� ������
bool isAccountExist_byCardNumber(const char* cardNumber) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) {
        std::cerr << "�� ������� ������� ���� ��� �������." << std::endl;
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

// �������� ��������� ������� �� IBAN
bool isAccountExist_byIBAN(const char* IBAN) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) {
        std::cerr << "�� ������� ������� ���� ��� �������." << std::endl;
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

// �������� ������� �� ������� ������
account getAccount_byCardNumber(const char* cardNumber) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) {
        std::cerr << "�� ������� ������� ���� ��� �������." << std::endl;
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

// �������� ������� �� IBAN
account getAccount_byIBAN(const char* IBAN) {
    std::ifstream fin(process.getAccountDBPath(), std::ios::binary);
    if (!fin) {
        std::cerr << "�� ������� ������� ���� ��� �������." << std::endl;
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
    // 1. ������� �� ������ � �����
    std::ifstream inFile("accounts.dat", std::ios::binary);
    if (!inFile) {
        std::cerr << "�������: �� ������� ������� ���� accounts.dat ��� �������." << std::endl;
        return;
    }

    std::vector<account> all;
    account temp;

    // �������� ���������� ����� load
    while (true) {
        temp = account(); // ������� ����� �����������
        temp.load(inFile);
        if (!inFile) break; // ���� ���������� �� ������� � �����
        all.push_back(temp);
    }
    inFile.close();

    // 2. ��������� �������� �����
    bool found = false;
    for (auto& acc : all) {
        if (strcmp(acc.getIBAN(), this->getIBAN()) == 0) { // ������ �� IBAN
            acc = *this; // ��������� ���� ��'���
            found = true;
            break;
        }
    }

    if (!found) {
        std::cerr << "������������: ������� �� ����� IBAN �� ��������." << std::endl;
        return;
    }

    // 3. ������������ ����
    std::ofstream outFile(process.getAccountDBPath(), std::ios::binary | std::ios::trunc);
    if (!outFile) {
        std::cerr << "�������: �� ������� ������� ���� accounts.dat ��� ������." << std::endl;
        return;
    }

    for (auto& acc : all)
        acc.save(outFile);

    outFile.close();
}



