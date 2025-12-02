#define _CRT_SECURE_NO_WARNINGS

#include "login.h"
#include "ui_login.h"
#include <iostream>
//#include <string>

#include <QThread>

using namespace std;

LoginWindow::LoginWindow(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);

    // Підключаємо сигнал кнопки до слота
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginWindow::onLoginButtonClicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}




void LoginWindow::onLoginButtonClicked()
{
    QString qlogin = ui->login_line_edit->text();
    QString qpassword = ui->password_line_edit->text();

    std::string login = qlogin.toUtf8().constData();
    std::string password = qpassword.toUtf8().constData();

    // тут робиш що треба: відправляєш на сервер, перевіряєш, і т.д.
    qDebug() << "Login: " << qlogin << "Password: " << qpassword;

	string cmdText = "login " + login + " " + password;
	strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
	handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';


    // викликати команду у фоні
    QMetaObject::invokeMethod(worker, [=]() {
        worker->runCommand();
        }, Qt::QueuedConnection);

    return;
}

void LoginWindow::setSessionId(int sid) {
    ui->session_id_value->setText(QString::number(sid)); // або QLabel, залежно від твого UI
}

void LoginWindow::setUserId(int uid) {
    ui->user_id_value->setText(QString::number(uid)); // або QLabel, залежно від твого UI
}