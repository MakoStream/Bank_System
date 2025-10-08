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

//TBkk12340101XXXXXXXXXXXX
//400000XXXXXXXXXCc

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
    int bank_id = 0;
    char* bank_id_str = new char[17]; // 16 ���� + '\0'
    memset(bank_id_str, 0, 17);

    // ������� ��������
    std::ifstream in("bank_id.bin", std::ios::binary);
    if (in.is_open()) {
        in.read(reinterpret_cast<char*>(&bank_id), sizeof(bank_id));
        in.close();
    }
    else {
        // ���� ����� ���� � ��������� � 0
        std::ofstream newFile("bank_id.bin", std::ios::binary);
        bank_id = 0;
        newFile.write(reinterpret_cast<const char*>(&bank_id), sizeof(bank_id));
        newFile.close();
    }

    // ������������ �� �������� �����
    bank_id++;
    std::ofstream out("bank_id.bin", std::ios::binary | std::ios::trunc);
    if (!out) {
        delete[] bank_id_str;
        throw std::runtime_error("�� ������� ������� bank_id.bin ��� ������");
    }
    out.write(reinterpret_cast<const char*>(&bank_id), sizeof(bank_id));
    out.close();

    // ������� 15-������ ������: "400000" + 9 ���� body
    char withoutCheck[16]; // 15 ���� + '\0'
    snprintf(withoutCheck, sizeof(withoutCheck), "400000%09d", bank_id); // ������� 9 ���� � ��������� ������

    // ���������� Luhn-���������� �����
    int check = computeLuhnCheckDigit(withoutCheck);

    // ������� ���������� 16-������� �����
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

    // ������� IBAN � ������ TB00xxxxxxxxxxxxxxx
    snprintf(IBAN_str, 36, "TB001234%014d", IBAN);

    return IBAN_str;
}

char* generate_CVC() {
	int CVC = rand() % 1000; // ��������� ����� �� 0 �� 999
	char* CVC_str = new char[4]; // 3 ����� + '\0'
	snprintf(CVC_str, 4, "%03d", CVC); // ������������ � ��������� ������
	return CVC_str;
}

void ACC_addAccount(int userID, balanceType balance_type, cardType type, short accountType) {
    std::ofstream fout("accounts.dat", ios::binary | ios::app);
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


