#define _CRT_SECURE_NO_WARNINGS
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>
#include "UserClientTest.h";
#include <boost/bind.hpp>
using namespace boost::unit_test;

// std::
using namespace std;
#define byte unsigned char

handleInfo* g_handle = nullptr;


/*
    “ести:

        ¬заЇмод≥€ з базовими функц≥€ми:
        1. split(const string& s)
        2. isStringDigit(const string& input)
        3. isPhone(const string& s)
        4. getTimestamp()

        ¬заЇмод≥€ з командами: 
        1. ќтримати номер сес≥њ
            // get_SID
        2. Ћог≥н хибний // login <login> <password>
        3. —творити базу данних юзер 
            // create_db user
        4. «ареЇструвати юзера 
            // reg_user <login> <password> <name> <surname> <phone>
            //example: reg_user user1 pass1 John Doe 1234567890
        5. ’ибний лог≥н
            // login <login> <password>
            4.1 ѕо лог≥ну
            4.2 ѕо паролю
        6. ”сп≥шний лог≥н
            // login <login> <password>
        7. ’то €
            //me
        8. «ареЇструвати ще одного юзера 
            // reg_user <login> <password> <name> <surname> <phone>
            //example: reg_user user1 pass1 John Doe 1234567890
        9. —писок юзер≥в
            //user_list
        10. ≤нфо про юзера
            9.1 user_info 1
            9.2 user_info <login>
        12. —творити базу даних рахунк≥в
            // create_db accounts
        12. „екнути список рахунк≥в
            // account_list
        13. —творити рахунок дл€ юзера з айд≥ = 1
            // new_account 1 UAH DEFULT 312
        14. ≤нфо про рахунок                     // команда в≥дсутн€ !!!
            13.(3) // account_info <id/PAN/IBAN>
        15. ¬ериф≥кувати рахунок // TB000000000000000000000000015
            // account_verify <IBAN>
        16. «абанити ≥ розбанити рахунок
            16.1 // account_ban <IBAN>
            16.2 // account_unban <IBAN>
        17. –азлог≥нитись
            // logout

    */
//=================
//   Basic Func
//=================

void test_splitString() {
    vector<string> result1 = split("one two three");
    BOOST_CHECK_EQUAL(result1.size(), 3);
    BOOST_CHECK_EQUAL(result1[0], "one");
    BOOST_CHECK_EQUAL(result1[1], "two");
    BOOST_CHECK_EQUAL(result1[2], "three");

    vector<string> result2 = split("");
    BOOST_CHECK_EQUAL(result2.size(), 0);

    vector<string> result3 = split("  a   b c ");
    BOOST_CHECK_EQUAL(result3.size(), 3);
    BOOST_CHECK_EQUAL(result3[0], "a");
    BOOST_CHECK_EQUAL(result3[1], "b");
    BOOST_CHECK_EQUAL(result3[2], "c");
};
void test_isStringDigit() {
    BOOST_CHECK(isStringDigit("123"));
    BOOST_CHECK(isStringDigit("0"));
    BOOST_CHECK(isStringDigit("12.34"));
    BOOST_CHECK(!isStringDigit("12..34"));
    BOOST_CHECK(!isStringDigit("12a34"));
    BOOST_CHECK(!isStringDigit(""));
    BOOST_CHECK(!isStringDigit("."));
};
void test_isPhone() {
    BOOST_CHECK(isPhone("0123456789"));
    BOOST_CHECK(!isPhone("123456789"));    // менше 10 цифр
    BOOST_CHECK(!isPhone("01234567890"));  // б≥льше 10 цифр
    BOOST_CHECK(!isPhone("01234abc89"));   // м≥стить букви
    BOOST_CHECK(!isPhone(""));             // порожн≥й р€док
};
void test_Timestamp() {
    string ts = getTimestamp();
    // ѕерев≥рка формату YYYY-MM-DD_HH-MM-SS
    BOOST_CHECK_EQUAL(ts.size(), 19);
    BOOST_CHECK(isdigit(ts[0]));
    BOOST_CHECK_EQUAL(ts[4], '-');
    BOOST_CHECK_EQUAL(ts[7], '-');
    BOOST_CHECK_EQUAL(ts[10], '_');
    BOOST_CHECK_EQUAL(ts[13], '-');
    BOOST_CHECK_EQUAL(ts[16], '-');
};
//=================
//  User Command
//=================
void test_getSID(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    if (g_handle == nullptr) {
        cout << "Error 1: g_handle is nullptr!" << endl;
        return;
    }
    strncpy(handle.sessionData.cmd, "getSID", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.sessionId != -1);
};
void test_login1(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "login MakoStream tralalelo999", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю // User with this login does not exist!
    BOOST_CHECK(handle.sessionData.hash[0] == 0);
    BOOST_CHECK(strcmp(handle.sessionData.msg[0], "User with this login does not exist!") == 0);
};
void test_create_db_users(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "create_db users", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(strcmp(handle.sessionData.msg[0], "User database created!")==0);
};
void test_reg_user1(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "reg_user user1 pass1 John Doe 1234567890", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.msg[0][0] != '\0');
};
void test_login2(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "login user1 pass228", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.hash[0] != 1);
    BOOST_CHECK(strcmp(handle.sessionData.msg[0], "Incorrect password!") == 0);

    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "login user2 pass1", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.hash[0] != 1);
    BOOST_CHECK(strcmp(handle.sessionData.msg[0], "User with this login does not exist!") == 0);
};
void test_login3(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    BOOST_REQUIRE(handle.sessionData.cmd != nullptr);
    strncpy(handle.sessionData.cmd, "login user1 pass1", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.hash[0] == 1);
    BOOST_CHECK(strcmp(handle.sessionData.msg[0], "user1") == 0);
    BOOST_CHECK(strcmp(handle.sessionData.msg[1], "John") == 0);
    BOOST_CHECK(strcmp(handle.sessionData.msg[2], "Doe") == 0);
};
void test_me(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    if (g_handle == nullptr) {
        cout << "Error: g_handle is nullptr!" << endl;
        return;
    }
    BOOST_REQUIRE(handle.sessionData.cmd != nullptr);
    strncpy(handle.sessionData.cmd, "me ", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.hash[0] == 1);
    BOOST_CHECK(handle.sessionData.hash[1] == 0);
    BOOST_CHECK(handle.sessionData.hash[2] == 1234567890);
    BOOST_CHECK(strcmp(handle.sessionData.msg[1], "user1 John Doe") == 0);
};
void test_reg_user2(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    memset(handle.sessionData.cmd, 0, sizeof(handle.sessionData.cmd));
    strncpy(handle.sessionData.cmd, "reg_user MakoStream trololo822 Makar Drobot 1234567891", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.msg[0][0] != '\0');
};
void test_user_list(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "user_list", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.msg[0][0] != '\0');
};
void test_user_info(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "user_info 1", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.hash[0] == 1);
    BOOST_CHECK(handle.sessionData.hash[1] == 1);
    BOOST_CHECK(handle.sessionData.hash[2] == 1234567891);
    BOOST_CHECK(strcmp(handle.sessionData.msg[1], "MakoStream Makar Drobot") == 0);

    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "user_info MakoStream", sizeof(handle.sessionData.cmd) - 1);
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.hash[0] == 1);
    BOOST_CHECK(handle.sessionData.hash[1] == 1);
    BOOST_CHECK(handle.sessionData.hash[2] == 1234567891);
    BOOST_CHECK(strcmp(handle.sessionData.msg[1], "MakoStream Makar Drobot") == 0);
};
void test_create_db_accounts(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "create_db accounts", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(strcmp(handle.sessionData.msg[0], "Accounts database created!") == 0);
};
void test_new_account(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "new_account 1 UAH DEFAULT 312", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю // –ахунок усп≥шно створено
    BOOST_CHECK(handle.sessionData.hash[0] == 1);
    BOOST_CHECK(strcmp(handle.sessionData.msg[0], "–ахунок усп≥шно створено") == 0);
};
void test_account_list(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "account_list", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.msg[0][0] != '\0');
};
void test_account_info(handleInfo& handle) { // поверне хибу
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "account_info 1", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.msg[1][0] != '\0');
};
void test_account_verify(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "account_verify TB000000000000000000000000015", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю 
    BOOST_CHECK(strcmp(handle.sessionData.msg[0], "Account verified successfully") == 0);
};
void test_account_ban_unban(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "account_ban TB000000000000000000000000015", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю 
    BOOST_CHECK(strcmp(handle.sessionData.msg[0], "Account banned!") == 0);

    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "account_unban TB000000000000000000000000015", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю 
    BOOST_CHECK(strcmp(handle.sessionData.msg[0], "Account unbanned!") == 0);
};
void test_logout(handleInfo& handle) {
    // –едагую handle - прописую в ньому команду
    strncpy(handle.sessionData.cmd, "logout", sizeof(handle.sessionData.cmd) - 1);
    handle.sessionData.cmd[sizeof(handle.sessionData.cmd) - 1] = '\0';
    // в≥дправл€ю на "сервер"
    response_manager.get_response(handle);
    // ѕерев≥р€ю
    BOOST_CHECK(handle.sessionData.userId ==-1);
};



bool
init_function()
{
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_splitString(); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_isStringDigit(); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_isPhone(); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_Timestamp(); })
    );

    //=================================================

    //=================================================
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_getSID(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_login1(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_create_db_users(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_reg_user1(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_login2(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_login3(*g_handle); })
    );
    
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_me(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_reg_user2(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_user_list(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_user_info(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_create_db_accounts(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_new_account(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_account_list(*g_handle); })
    );
    /*framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_account_info(*g_handle); })
    );*/
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_account_verify(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_account_ban_unban(*g_handle); })
    );
    framework::master_test_suite().add(
        BOOST_TEST_CASE([]() { test_logout(*g_handle); })
    );



    return true;
}

void runUnitTest(handleInfo& handle) {
    g_handle = &handle; // передаЇмо handle у глобальну зм≥нну
    // —творюЇмо власн≥ argc/argv
    int argc = 1;
    char* argv[] = { (char*)"unit_test" };
    ::boost::unit_test::unit_test_main(&init_function, argc, argv);
};