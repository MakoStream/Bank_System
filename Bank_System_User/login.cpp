#include "login.h"
#include "ui_login.h"

LoginWindow::LoginWindow(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}
