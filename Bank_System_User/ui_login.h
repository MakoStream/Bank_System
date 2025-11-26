/********************************************************************************
** Form generated from reading UI file 'login.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGIN_H
#define UI_LOGIN_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCommandLinkButton>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWindow
{
public:
    QLabel *session_id_label;
    QLabel *session_id_value;
    QLabel *user_id_value;
    QLabel *user_id_label;
    QLabel *logo_main;
    QLabel *label;
    QLabel *login_label;
    QLineEdit *login_line_edit;
    QLineEdit *password_line_edit;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *label_login;
    QLabel *label_unput_password;
    QCommandLinkButton *ForgedPasswodButton;
    QLabel *msg_box;
    QPushButton *pushButton;
    QCommandLinkButton *RegisterButton;

    void setupUi(QDialog *LoginWindow)
    {
        if (LoginWindow->objectName().isEmpty())
            LoginWindow->setObjectName("LoginWindow");
        LoginWindow->setEnabled(true);
        LoginWindow->resize(618, 315);
        LoginWindow->setMinimumSize(QSize(618, 315));
        LoginWindow->setMaximumSize(QSize(618, 315));
        QFont font;
        font.setPointSize(8);
        LoginWindow->setFont(font);
        LoginWindow->setContextMenuPolicy(Qt::ContextMenuPolicy::NoContextMenu);
        LoginWindow->setAcceptDrops(false);
        QIcon icon;
        icon.addFile(QString::fromUtf8("C:/Users/Admin/OneDrive/Pictures/GlassCubeRed1.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        LoginWindow->setWindowIcon(icon);
        LoginWindow->setWindowOpacity(2.000000000000000);
        session_id_label = new QLabel(LoginWindow);
        session_id_label->setObjectName("session_id_label");
        session_id_label->setGeometry(QRect(0, 300, 26, 16));
        session_id_value = new QLabel(LoginWindow);
        session_id_value->setObjectName("session_id_value");
        session_id_value->setGeometry(QRect(30, 300, 42, 16));
        user_id_value = new QLabel(LoginWindow);
        user_id_value->setObjectName("user_id_value");
        user_id_value->setGeometry(QRect(30, 285, 42, 16));
        user_id_label = new QLabel(LoginWindow);
        user_id_label->setObjectName("user_id_label");
        user_id_label->setGeometry(QRect(0, 285, 27, 16));
        logo_main = new QLabel(LoginWindow);
        logo_main->setObjectName("logo_main");
        logo_main->setGeometry(QRect(-120, -50, 561, 321));
        logo_main->setPixmap(QPixmap(QString::fromUtf8("C:/Users/Admin/OneDrive/Pictures/GlassCubeRed.png")));
        logo_main->setScaledContents(true);
        logo_main->setWordWrap(false);
        label = new QLabel(LoginWindow);
        label->setObjectName("label");
        label->setEnabled(true);
        label->setGeometry(QRect(352, 6, 231, 41));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("MS Reference Sans Serif")});
        font1.setPointSize(12);
        font1.setBold(true);
        font1.setStrikeOut(false);
        font1.setKerning(true);
        font1.setStyleStrategy(QFont::PreferDefault);
        label->setFont(font1);
        label->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        label->setAcceptDrops(false);
        label->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        label->setScaledContents(false);
        label->setWordWrap(false);
        login_label = new QLabel(LoginWindow);
        login_label->setObjectName("login_label");
        login_label->setEnabled(true);
        login_label->setGeometry(QRect(352, 29, 231, 41));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("MS Reference Sans Serif")});
        font2.setPointSize(10);
        font2.setBold(true);
        font2.setStrikeOut(false);
        font2.setKerning(true);
        font2.setStyleStrategy(QFont::PreferDefault);
        login_label->setFont(font2);
        login_label->setFocusPolicy(Qt::FocusPolicy::NoFocus);
        login_label->setAcceptDrops(false);
        login_label->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        login_label->setScaledContents(false);
        login_label->setWordWrap(false);
        login_line_edit = new QLineEdit(LoginWindow);
        login_line_edit->setObjectName("login_line_edit");
        login_line_edit->setGeometry(QRect(430, 140, 161, 21));
        login_line_edit->setMaxLength(34);
        password_line_edit = new QLineEdit(LoginWindow);
        password_line_edit->setObjectName("password_line_edit");
        password_line_edit->setGeometry(QRect(430, 169, 161, 21));
        password_line_edit->setMaxLength(34);
        verticalLayoutWidget = new QWidget(LoginWindow);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(0, 0, 361, 321));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        label_login = new QLabel(LoginWindow);
        label_login->setObjectName("label_login");
        label_login->setGeometry(QRect(370, 140, 55, 16));
        label_unput_password = new QLabel(LoginWindow);
        label_unput_password->setObjectName("label_unput_password");
        label_unput_password->setGeometry(QRect(370, 170, 61, 16));
        ForgedPasswodButton = new QCommandLinkButton(LoginWindow);
        ForgedPasswodButton->setObjectName("ForgedPasswodButton");
        ForgedPasswodButton->setGeometry(QRect(420, 189, 151, 31));
        QFont font3;
        font3.setFamilies({QString::fromUtf8("Segoe UI")});
        font3.setPointSize(8);
        ForgedPasswodButton->setFont(font3);
        ForgedPasswodButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        ForgedPasswodButton->setIconSize(QSize(0, 0));
        msg_box = new QLabel(LoginWindow);
        msg_box->setObjectName("msg_box");
        msg_box->setGeometry(QRect(370, 70, 231, 61));
        msg_box->setFont(font3);
        msg_box->setScaledContents(false);
        msg_box->setWordWrap(true);
        msg_box->setMargin(0);
        msg_box->setIndent(1);
        pushButton = new QPushButton(LoginWindow);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(440, 270, 93, 28));
        RegisterButton = new QCommandLinkButton(LoginWindow);
        RegisterButton->setObjectName("RegisterButton");
        RegisterButton->setGeometry(QRect(440, 220, 111, 31));
        RegisterButton->setFont(font3);
        RegisterButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        RegisterButton->setIconSize(QSize(0, 0));

        retranslateUi(LoginWindow);

        QMetaObject::connectSlotsByName(LoginWindow);
    } // setupUi

    void retranslateUi(QDialog *LoginWindow)
    {
        LoginWindow->setWindowTitle(QCoreApplication::translate("LoginWindow", "MakoStream's Bank", nullptr));
        session_id_label->setText(QCoreApplication::translate("LoginWindow", "s-id:", nullptr));
        session_id_value->setText(QCoreApplication::translate("LoginWindow", "000000", nullptr));
        user_id_value->setText(QCoreApplication::translate("LoginWindow", "000000", nullptr));
        user_id_label->setText(QCoreApplication::translate("LoginWindow", "u-id:", nullptr));
        logo_main->setText(QString());
        label->setText(QCoreApplication::translate("LoginWindow", "Some project", nullptr));
        login_label->setText(QCoreApplication::translate("LoginWindow", "login", nullptr));
        login_line_edit->setInputMask(QString());
        login_line_edit->setText(QString());
        label_login->setText(QCoreApplication::translate("LoginWindow", "login", nullptr));
        label_unput_password->setText(QCoreApplication::translate("LoginWindow", "password", nullptr));
        ForgedPasswodButton->setText(QCoreApplication::translate("LoginWindow", "forgot password?", nullptr));
        msg_box->setText(QCoreApplication::translate("LoginWindow", "Message_Text", nullptr));
        pushButton->setText(QCoreApplication::translate("LoginWindow", "Login", nullptr));
        RegisterButton->setText(QCoreApplication::translate("LoginWindow", "registration", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWindow: public Ui_LoginWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGIN_H
