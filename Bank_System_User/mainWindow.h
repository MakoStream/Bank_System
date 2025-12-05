#pragma once
#include <QDebug>
#include <QtWidgets>
#include <QDialog>
#include <QString>
#include <QtConcurrent>

#include "ui_window.h"
//#include "user_info.h"
#include "ResponseManager.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QDialog
{
    Q_OBJECT

private slots:
    void onLoginButtonClicked();
	void onRegisterButtonClicked();
	void onBackToLoginButtonClicked();
	void onRegisterButtonCompletedClicked();
	void onLogoutButtonClicked();
	void onTransactionRequestButtonClicked();

    void showPAN_Transaction();
    void showIBAN_Transaction();

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    QWidget* cloneWidget(const QWidget* original);

    void addUserCardsBoxItem(string cardPan, int acc_id);
    void clearUserCardsBox();

    void AddTransactionBoxItem(const std::string& pan_to, const string& pan_from, int amount, const std::string& currency_type);
    void AddAccountBoxItem(const std::string& accountPan, const std::string& currency_type, float balance);

	void fillTransactionBox();
	void fillAccountBox();
    

public slots:
    

    void setSessionId(int sid);
    void setUserId(int uid);

	void hideLoginWindow();
	void showLoginWindow();

    void hideRegWindow();
	void showRegWindow();

	void hideMainMenuWindow();
	void showMainMenuWindow();

    void hideTransactionWindow();
    void showTransactionWindow();

    void ClientUserDataUpdate();

    void setMessage(const std::string& message);

private:
    QVBoxLayout* layout = nullptr;
	QHBoxLayout* hLayout = nullptr;
    Ui::MainWindow* ui;
};


class Worker : public QObject {
    Q_OBJECT
public:
    void runCommand() {
		cout << "Worker thread running command..." << endl;
        response_manager.get_response(*handleP);
    }
};

// оголошуємо зовнішній об’єкт
extern MainWindow w;
extern Worker* worker;