#pragma once
#include <QDebug>
#include <QtWidgets>
#include <QDialog>
#include <QtConcurrent>

#include "user_info.h"
#include "ResponseManager.h"



QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QDialog
{
    Q_OBJECT

private slots:
    void onLoginButtonClicked();

public:
    explicit LoginWindow(QWidget* parent = nullptr);
    ~LoginWindow();

public slots:
    void setSessionId(int sid);
    void setUserId(int uid);

private:
    Ui::LoginWindow* ui;
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
extern LoginWindow w;
extern Worker* worker;