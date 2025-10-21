#define _CRT_SECURE_NO_WARNINGS
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>
#include "UserClientTest.h";
// std::
using namespace std;


HANDLE pipe = INVALID_HANDLE_VALUE;
sessionConstruct session{};
DWORD read = 0, written = 0;

handleInfo handle(pipe, session, read, written);

handleInfo* g_handle = nullptr;

/*
    Тести:

        Взаємодія з базовими функціями:
        1. split(const string& s)
        2. isStringDigit(const string& input)
        3. isPhone(const string& s)
        4. getTimestamp()

        Взаємодія з командами: 
        1. Отримати номер сесії
            // get_SID
        2. Логін хибний // login <login> <password>
        3. Створити базу данних юзер 
            // create_db user
        4. Зареєструвати юзера 
            // reg_user <login> <password> <name> <surname> <phone>
            //example: reg_user user1 pass1 John Doe 1234567890
        5. Хибний логій
            // login <login> <password>
            4.1 По логіну
            4.2 По паролю
        6. Успішний логін
            // login <login> <password>
        7. Хто я
            //me
        8. Зареєструвати ще одного юзера 
            // reg_user <login> <password> <name> <surname> <phone>
            //example: reg_user user1 pass1 John Doe 1234567890
        9. Список юзерів
            //user_list
        10. Інфо про юзера
            9.1 user_info 1
            9.2 user_info <login>
        12. Створити базу даних рахунків
            // create_db accounts
        12. Чекнути список рахунків
            // account_list
        13. Створити рахунок для юзера з айді = 1
            // new_account 1 UAH DEFULT 312
        14. Інфо про рахунок                     // команда відсутня !!!
            13.(3) // account_info <id/PAN/IBAN>
        15. Верифікувати рахунок
            // account_verify <IBAN>
        16. Забанити і розбанити рахунок
            16.1 // account_ban <IBAN>
            16.2 // account_unban <IBAN>
        17. Разлогінитись
            // logout

    */


struct HandleFixture {
    HANDLE pipe = INVALID_HANDLE_VALUE;
    sessionConstruct session{};
    DWORD bytesRead = 0;
    DWORD bytesWritten = 0;

    handleInfo handle;

    HandleFixture()
        : handle(pipe, session, bytesRead, bytesWritten) // ініціалізація посилань
    {
        g_handle = &handle;
    }

    ~HandleFixture() {
        g_handle = nullptr;
    }

    void sendCommand(const string& cmd) {
        strncpy(handle.sessionData.cmd, cmd.c_str(), sizeof(handle.sessionData.cmd) - 1);
        handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
        response_manager.get_response(handle);
    }
};


// ======================
// Basic functions tests
// ======================
BOOST_FIXTURE_TEST_CASE(test_splitString, HandleFixture) {
    vector<string> expected = { "hello", "world", "test" };
    vector<string> result = split("hello world test");
    BOOST_CHECK_EQUAL_COLLECTIONS(result.begin(), result.end(), expected.begin(), expected.end());

    vector<string> empty = split("");
    BOOST_CHECK(empty.empty());
}

BOOST_FIXTURE_TEST_CASE(test_isStringDigit, HandleFixture) {
    BOOST_CHECK(isStringDigit("12345"));
    BOOST_CHECK(isStringDigit("12.45"));
    BOOST_CHECK(!isStringDigit("12..45"));
    BOOST_CHECK(!isStringDigit("12a45"));
    BOOST_CHECK(!isStringDigit(""));
}

BOOST_FIXTURE_TEST_CASE(test_isPhone, HandleFixture) {
    BOOST_CHECK(isPhone("0971234567"));
    BOOST_CHECK(!isPhone("097123456"));
    BOOST_CHECK(!isPhone("09712345678"));
    BOOST_CHECK(!isPhone("09712A4567"));
    BOOST_CHECK(!isPhone(""));
}

BOOST_FIXTURE_TEST_CASE(test_getTimestamp, HandleFixture) {
    string ts = getTimestamp();
    BOOST_CHECK(!ts.empty());
    BOOST_CHECK(ts.find('_') != string::npos);
    BOOST_CHECK(ts.find(':') == string::npos);
}

// ======================
// User commands tests
// ======================
BOOST_FIXTURE_TEST_CASE(test_command_get_SID, HandleFixture) {
    sendCommand("get_SID");
    BOOST_CHECK(handle.sessionData.sessionId != -1);
}

BOOST_FIXTURE_TEST_CASE(test_command_login1, HandleFixture) {
    sendCommand("login MakoStream 1234567frw");
    BOOST_CHECK(handle.sessionData.hash[0] != 1);
    BOOST_CHECK(handle.sessionData.msg[0][0] != '\0');
}

BOOST_FIXTURE_TEST_CASE(test_command_create_db_user, HandleFixture) {
    sendCommand("create_db users");
    BOOST_CHECK(handle.sessionData.hash[0] == 1);
}

BOOST_FIXTURE_TEST_CASE(test_command_reg_user1, HandleFixture) {
    sendCommand("reg_user user1 12345678 John Doe 1234567890");
    BOOST_CHECK(handle.sessionData.msg[0][0] != '\0');
}

BOOST_FIXTURE_TEST_CASE(test_command_login2, HandleFixture) {
    sendCommand("login user2 12345678");
    BOOST_CHECK(strncmp(handle.sessionData.msg[0], "User with this login does not exist!", 100) == 0);
    BOOST_CHECK(handle.sessionData.hash[0] != 1);

    sendCommand("login user1 12345679");
    BOOST_CHECK(strncmp(handle.sessionData.msg[0], "Incorrect password!", 100) == 0);
    BOOST_CHECK(handle.sessionData.hash[0] != 1);
}

BOOST_FIXTURE_TEST_CASE(test_command_login3, HandleFixture) {
    sendCommand("login user1 12345678");
    BOOST_CHECK(strncmp(handle.sessionData.msg[0], "user1", 100) == 0);
    BOOST_CHECK(strncmp(handle.sessionData.msg[1], "John", 100) == 0);
    BOOST_CHECK(strncmp(handle.sessionData.msg[2], "Doe", 100) == 0);
}

BOOST_FIXTURE_TEST_CASE(test_command_me, HandleFixture) {
    sendCommand("me");
    BOOST_CHECK(handle.sessionData.hash[0] == 1);
    BOOST_CHECK(handle.sessionData.hash[1] == handle.sessionData.userId);
}

BOOST_FIXTURE_TEST_CASE(test_command_reg_user2, HandleFixture) {
    sendCommand("reg_user MakoStream 1234567frw Makar Drobot 1234567891");
    BOOST_CHECK(handle.sessionData.msg[0][0] != '\0');
}

BOOST_FIXTURE_TEST_CASE(test_command_user_list, HandleFixture) {
    sendCommand("user_list");
    BOOST_CHECK(handle.sessionData.msg[0][0] != '\0');
}

BOOST_FIXTURE_TEST_CASE(test_command_user_info, HandleFixture) {
    sendCommand("user_info 1");
    BOOST_CHECK(handle.sessionData.hash[0] == 1);
    BOOST_CHECK(handle.sessionData.hash[1] == 1);
    BOOST_CHECK(strncmp(handle.sessionData.msg[1], "MakoStrteam Makar Drobot", 100) == 0);

    sendCommand("user_info MakoStream");
    BOOST_CHECK(handle.sessionData.hash[0] == 1);
    BOOST_CHECK(handle.sessionData.hash[1] == 1);
    BOOST_CHECK(strncmp(handle.sessionData.msg[1], "MakoStrteam Makar Drobot", 100) == 0);
}

BOOST_FIXTURE_TEST_CASE(test_command_create_db_accounts, HandleFixture) {
    sendCommand("create_db accounts");
    BOOST_CHECK(handle.sessionData.hash[0] == 1);
}

BOOST_FIXTURE_TEST_CASE(test_command_account_list, HandleFixture) {
    sendCommand("account_list");
    BOOST_CHECK(handle.sessionData.msg[0][0] != '\0');
}

BOOST_FIXTURE_TEST_CASE(test_command_new_account, HandleFixture) {
    sendCommand("new_account 1 UAH DEFAULT 312");
    BOOST_CHECK(handle.sessionData.msg[0][0] != '\0');
}

BOOST_FIXTURE_TEST_CASE(test_command_account_info, HandleFixture) {
    BOOST_CHECK(false); // ще не готово
}

BOOST_FIXTURE_TEST_CASE(test_command_account_verify, HandleFixture) {
    sendCommand("account_verify TB0000000000000000000000016");
    BOOST_CHECK(strncmp(handle.sessionData.msg[0], "Account verified successfully", 100) == 0);
}

BOOST_FIXTURE_TEST_CASE(test_command_account_ban_unban, HandleFixture) {
    sendCommand("account_ban TB0000000000000000000000016");
    BOOST_CHECK(strncmp(handle.sessionData.msg[0], "Account banned!", 100) == 0);

    sendCommand("account_unban TB0000000000000000000000016");
    BOOST_CHECK(strncmp(handle.sessionData.msg[0], "Account unbanned!", 100) == 0);
}

BOOST_FIXTURE_TEST_CASE(test_command_logout, HandleFixture) {
    sendCommand("logout");
    BOOST_CHECK(handle.sessionData.userId == -1);
}


bool init_unit_test() {
    return true; // тести вже зареєстровані через BOOST_AUTO_TEST_CASE
}


void runUnitTest(handleInfo& handle) {
    g_handle = &handle;

    string timestamp = getTimestamp();
    string filename = "unit_test_log_" + timestamp + ".txt";
    ofstream logFile(filename);

    boost::unit_test::unit_test_log.set_stream(logFile);
    boost::unit_test::unit_test_log.set_threshold_level(boost::unit_test::log_messages);

    // Запуск тестів
    boost::unit_test::unit_test_main(&init_unit_test, 0, nullptr);

    // Очистка консолі
    system("cls");

    cout << "Результати тестів збережено у файл: " << filename << endl;
}