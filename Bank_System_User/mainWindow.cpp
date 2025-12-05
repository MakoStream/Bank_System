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
    connect(ui->updateButton, &QPushButton::clicked, this, &MainWindow::ClientUserDataUpdate);

    connect(ui->TransactionPAN, &QPushButton::clicked, this, &MainWindow::showPAN_Transaction);
	connect(ui->TransactionIBAN, &QPushButton::clicked, this, &MainWindow::showIBAN_Transaction);
    connect(ui->BackFromTransactionWindow, &QPushButton::clicked, this, &MainWindow::hideTransactionWindow);

	connect(ui->TransactionRequestButton, &QPushButton::clicked, this, &MainWindow::onTransactionRequestButtonClicked);


    //  ===================================================
    // Робими віджет скролл транзакцій
    ui->TransactionTemplateWidget->hide();
    ui->historyScrollArea->setWidgetResizable(true);
    layout = new QVBoxLayout(ui->HistoryScrollAreaWidgetContents);

    layout->setContentsMargins(0, 0, 0, 0); // відступи по краях
    layout->setSpacing(5);

	// Робимо віджет скролл карток користувача
	ui->Card_Template->hide();
    ui->CardsScroll->setWidgetResizable(true);
	hLayout = new QHBoxLayout(ui->CardsScrollAreaWidgetContent);

	hLayout->setContentsMargins(0, 0, 0, 0); // відступи по краях
	hLayout->setSpacing(1);

    //=========================================
    hideRegWindow();
    //hideMainMenuWindow();
    hideTransactionWindow();

    
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

void MainWindow::showPAN_Transaction() {
    ui->TransactionRequestButton->setProperty("isIBAN", false);
	ui->TransactionFrameName->setText("PAN Transaction");
    showTransactionWindow();
};

void MainWindow::showIBAN_Transaction() {
    ui->TransactionRequestButton->setProperty("isIBAN", true);
    ui->TransactionFrameName->setText("IBAN Transaction");
    showTransactionWindow();
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

    response_manager.get_response(*handleP);

    if (handleP->sessionData.hash[0] == 1) {
        ClientUserDataUpdate();
    };

    return;
};

void MainWindow::onLogoutButtonClicked() {
    string cmdText = "logout";
    strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
    handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';

    response_manager.get_response(*handleP);
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


    response_manager.get_response(*handleP);
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

    bool isIban = ui->TransactionRequestButton->property("isIBAN").toBool();

	string cmdText;
	if (isIban) {
        cmdText = "request_IBAN " + from_card_pan + " " + to_card_pan + " " + to_string(amount) + " " + PIN;
    }
	else {
        cmdText = "request_PAN " + from_card_pan + " " + to_card_pan + " " + to_string(amount) + " 000 " + PIN;
	}
	strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
	handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';

	response_manager.get_response(*handleP);

	return;
};

QWidget* MainWindow::cloneWidget(const QWidget* original) {
    QWidget* copy = new QWidget(original->parentWidget());
    copy->setObjectName(original->objectName());
    copy->setGeometry(original->geometry());
    copy->setStyleSheet(original->styleSheet());
    copy->setMinimumSize(original->minimumSize());
    copy->setMaximumSize(original->maximumSize());
    copy->setSizePolicy(original->sizePolicy());
    copy->setLayout(nullptr);

    // ----- КЛОНУЄМО ЛЕЙЛАУТ -----
    if (original->layout()) {
        QLayout* origLayout = original->layout();

        if (auto* v = qobject_cast<QVBoxLayout*>(origLayout)) {
            auto* l = new QVBoxLayout(copy);
            l->setSpacing(v->spacing());
            l->setContentsMargins(v->contentsMargins());
            copy->setLayout(l);

        }
        else if (auto* h = qobject_cast<QHBoxLayout*>(origLayout)) {
            auto* l = new QHBoxLayout(copy);
            l->setSpacing(h->spacing());
            l->setContentsMargins(h->contentsMargins());
            copy->setLayout(l);

        }
        else if (auto* g = qobject_cast<QGridLayout*>(origLayout)) {
            auto* l = new QGridLayout(copy);
            l->setSpacing(g->spacing());
            l->setContentsMargins(g->contentsMargins());
            copy->setLayout(l);
        }
    }

    // ----- КЛОНУЄМО ВСІ CHILD WIDGETS -----
    for (QObject* child : original->children()) {
        if (!child->isWidgetType())
            continue;

        QWidget* childWidget = static_cast<QWidget*>(child);
        QWidget* childCopy = nullptr;

        // --- КОПІЮВАННЯ QLABEL ---
        if (auto* lbl = qobject_cast<QLabel*>(childWidget)) {
            QLabel* lblCopy = new QLabel(copy);
            lblCopy->setObjectName(lbl->objectName());
            lblCopy->setGeometry(lbl->geometry());
            lblCopy->setText(lbl->text());
            lblCopy->setAlignment(lbl->alignment());
            lblCopy->setStyleSheet(lbl->styleSheet());
            if (!lbl->pixmap(Qt::ReturnByValue).isNull())
                lblCopy->setPixmap(lbl->pixmap(Qt::ReturnByValue));
            childCopy = lblCopy;
        }

        // --- ІНШІ WIDGETS КОПІЮЄМО РЕКУРСІЄЮ ---
        else {
            childCopy = cloneWidget(childWidget);
            childCopy->setParent(copy);
        }

        // Додаємо у layout
        if (copy->layout())
            copy->layout()->addWidget(childCopy);
    }

    return copy;
}

// REMEBER_THIS: LastItem_HorizontalPos


void MainWindow::AddTransactionBoxItem(const std::string& pan_to, const string& pan_from, int amount, const std::string& currency_type) {
	QString panToStr = QString::fromUtf8(pan_to.c_str(), static_cast<int>(pan_to.size()));
    QString panFromStr = QString::fromUtf8(pan_from.c_str(), static_cast<int>(pan_from.size()));
	QString amountStr = QString::number(amount);
	QString currencyStr = QString::fromUtf8(currency_type.c_str(), static_cast<int>(currency_type.size()));

	QWidget* newWidget = cloneWidget(ui->TransactionTemplateWidget);

	QLabel* panToLabel = newWidget->findChild<QLabel*>("History_PAN_to");
	if (panToLabel) {
		panToLabel->setText(panToStr);
	}
    QLabel* panFromLabel = newWidget->findChild<QLabel*>("History_PAN_from");
    if (panFromLabel) {
        panFromLabel->setText(panFromStr);
    }

	QLabel* amountLabel = newWidget->findChild<QLabel*>("History_ammount");
	if (amountLabel) {
		amountLabel->setText(amountStr);
	}

	QLabel* currencyLabel = newWidget->findChild<QLabel*>("History_value_type");
	if (currencyLabel) {
		currencyLabel->setText(currencyStr);
	}

	layout->addWidget(newWidget);
	newWidget->show();
};

void MainWindow::fillTransactionBox() {
    // Очищаємо скролл
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    string cmdText = "get_user_transactions";
    strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
    handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';
    response_manager.get_response(*handleP);

    //===============================
    if (handleP->sessionData.hash[0] == 1) {
        vector<string> transactions = split(handleP->sessionData.msg[0]);
        cout << "===" << handleP->sessionData.msg[0] << endl;
        for (string& a : transactions) {
            cmdText = "transaction_info " + a;

			strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
			handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';
            response_manager.get_response(*handleP);
        };
    }
    else {
		setMessage("No transactions found.");
        return;
    };
};

void MainWindow::AddAccountBoxItem(const std::string& accountPan, const std::string& currency_type, float balance) { // like AddTransactionBoxItem
    // Конвертація в QString
    QString accountPanStr = QString::fromUtf8(accountPan.c_str(), static_cast<int>(accountPan.size()));
    QString currencyStr = QString::fromUtf8(currency_type.c_str(), static_cast<int>(currency_type.size()));
    QString balanceStr = QString::number(balance);

    // Клонування шаблону
    QWidget* newWidget = cloneWidget(ui->Card_Template);

    // Заповнення даних
    QLabel* accountPanLabel = newWidget->findChild<QLabel*>("Card_PAN");
    if (accountPanLabel) accountPanLabel->setText(accountPanStr);

    QLabel* balanceLabel = newWidget->findChild<QLabel*>("Card_Value");
    if (balanceLabel) balanceLabel->setText(balanceStr);

    QLabel* currencyLabel = newWidget->findChild<QLabel*>("Card_Value_Type");
    if (currencyLabel) currencyLabel->setText(currencyStr);

    // Додати в горизонтальний layout скролл-зони
    hLayout->addWidget(newWidget);

    // Показати новий widget
    newWidget->show();
};

void MainWindow::fillAccountBox() {
    // Очищаємо скролл
    QLayoutItem* item;
    while ((item = hLayout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }

    // потім (за неохідності)
    for (int a : currentUser.accounts_id) {
        string cmdText = "account_info_id " + to_string(a);
        strncpy(handleP->sessionData.cmd, cmdText.c_str(), sizeof(handleP->sessionData.cmd) - 1);
        handleP->sessionData.cmd[sizeof(handleP->sessionData.cmd) - 1] = '\0';

        response_manager.get_response(*handleP);

        vector<string> args = split(handleP->sessionData.msg[0]);
        w.AddAccountBoxItem(args[1], "UAH", stoi(args[3]));
    };
};

void MainWindow::ClientUserDataUpdate() {
	fillTransactionBox();
	fillAccountBox();
};

