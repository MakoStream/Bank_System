#define _CRT_SECURE_NO_WARNINGS

#include "mainWindow.h"
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
	connect(ui->LogoutButton, &QPushButton::clicked, this, &MainWindow::onLogoutButtonClicked);

    connect(ui->TransactionPAN, &QPushButton::clicked, this, &MainWindow::showTransactionWindow);
    connect(ui->BackFromTransactionWindow, &QPushButton::clicked, this, &MainWindow::hideTransactionWindow);

	connect(ui->TransactionRequestButton, &QPushButton::clicked, this, &MainWindow::onTransactionRequestButtonClicked);
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

void MainWindow::showTransactionWindow() {
    ui->TransactionWidget->show();
};

void MainWindow::hideTransactionWindow() {
    ui->TransactionWidget->hide();
};



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

void MainWindow::onLogoutButtonClicked() {
    string cmdText = "logout";
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
    ui->session_id_value->setText(QString::number(sid)); 
    ui->session_id_value_3->setText(QString::number(sid));
}

void MainWindow::setUserId(int uid) {
    ui->user_id_value->setText(QString::number(uid));
    ui->user_id_value_3->setText(QString::number(uid));
}

void MainWindow::setMessage(const std::string& message) {
    // конвертуємо std::string у QString

    QString qmsg = QString::fromUtf8(message.c_str(), static_cast<int>(message.size()));
    ui->msg_box->setText(qmsg);
}


void MainWindow::clearUserCardsBox() {
    ui->UserCardsBox->clear();
};

void MainWindow::addUserCardsBoxItem(string cardPan, int acc_id) {
    cout << cardPan << endl;
    QString itemText = QString::fromUtf8(cardPan.c_str(), static_cast<int>(cardPan.size()));
    ui->UserCardsBox->addItem(itemText, acc_id);
};

void MainWindow::onTransactionRequestButtonClicked() {
    QString from = ui->UserCardsBox->currentText();
	string from_card_pan = from.toUtf8().constData();

	QString to = ui->PAN_to->text();
	string to_card_pan = to.toUtf8().constData();

    QString qPIN = ui->PIN_Edit->text();
	string PIN = qPIN.toUtf8().constData();

    int accId = ui->UserCardsBox->currentData().toInt();

	float amount = ui->AmountSpin->value();

	string cmdText = "request_PAN " + from_card_pan + " " + to_card_pan + " " + to_string(amount) + " 000 " + PIN;
	strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
	handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';

	// викликати команду у фоні
	QMetaObject::invokeMethod(worker, [=]() {
		worker->runCommand();
		}, Qt::QueuedConnection);
};