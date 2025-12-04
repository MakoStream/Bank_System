#define _CRT_SECURE_NO_WARNINGS

#include "mainWindow.h"
#include "ui_login.h"
#include <iostream>
//#include <string>

#include <QThread>

using namespace std;

MainWindow::MainWindow(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Підключаємо сигнал кнопки до слота
    connect(ui->LoginButton, &QPushButton::clicked, this, &MainWindow::onLoginButtonClicked);
	connect(ui->RegisterButton, &QPushButton::clicked, this, &MainWindow::onRegisterButtonClicked);
	connect(ui->BackToLoginButton, &QPushButton::clicked, this, &MainWindow::onBackToLoginButtonClicked);
    connect(ui->RegisterButtonCompl, & QPushButton::clicked, this, &MainWindow::onRegisterButtonCompletedClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::hideRegWindow() {
	ui->RegWidget->hide();
}
void MainWindow::showRegWindow() {
	ui->RegWidget->show();
}
void MainWindow::hideMainMenuWindow() {
	ui->MainMenuWidget->hide();
}
void MainWindow::showMainMenuWindow() {
	ui->MainMenuWidget->show();
}

void MainWindow::hideLoginWindow() {
	ui->loginWidget->hide();
}
void MainWindow::showLoginWindow() {
	ui->loginWidget->show();
}



void MainWindow::onLoginButtonClicked()
{
    QString qlogin = ui->login_line_edit->text();
    QString qpassword = ui->password_line_edit->text();

    std::string login = qlogin.toUtf8().constData();
    std::string password = qpassword.toUtf8().constData();

    // тут робиш що треба: відправляєш на сервер, перевіряєш, і т.д.
    //qDebug() << "Login: " << qlogin << "Password: " << qpassword;

    string cmdText = "login " + login + " " + password;
    strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
    handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';


    // викликати команду у фоні
    QMetaObject::invokeMethod(worker, [=]() {
        worker->runCommand();
        }, Qt::QueuedConnection);

    return;
};

void MainWindow::onRegisterButtonCompletedClicked() {
    QString qlogin = ui->LoginEdit->text();
    QString qpassword = ui->PasswordEdit->text();
    QString qName = ui->NameEdit->text();
    QString qSurname = ui->SurnameEdit->text();

    string login = qlogin.toUtf8().constData();
    string password = qpassword.toUtf8().constData();
    string Name = qName.toUtf8().constData();
    string Surname = qSurname.toUtf8().constData();


    string cmdText = "reg_user " + login + " " + password + " " + Name + " " + Surname + " " + "0000000000";
    strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
    handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';


    // викликати команду у фоні
    QMetaObject::invokeMethod(worker, [=]() {
        worker->runCommand();
        }, Qt::QueuedConnection);

    return;
};

void MainWindow::onRegisterButtonClicked() {
	hideLoginWindow();
	showRegWindow();
};

void MainWindow::onBackToLoginButtonClicked() {
	hideRegWindow();
	showLoginWindow();
};

void MainWindow::setSessionId(int sid) {
    ui->session_id_value->setText(QString::number(sid)); // або QLabel, залежно від твого UI
}

void MainWindow::setUserId(int uid) {
    ui->user_id_value->setText(QString::number(uid)); // або QLabel, залежно від твого UI
}

void MainWindow::setMessage(const std::string& message) {
    // конвертуємо std::string у QString

    QString qmsg = QString::fromUtf8(message.c_str(), static_cast<int>(message.size()));
    ui->msg_box->setText(qmsg);
}