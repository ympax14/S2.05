#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>

class LoginDialog : public QDialog {
    Q_OBJECT

private:
    static const int WINDOW_WIDTH = 400, WINDOW_HEIGHT = 250;
    // Layout principal
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QFont titleFont;

    // Layout formulaire
    QFormLayout *formLayout;
    QLineEdit *emailInput;
    QLineEdit *passwordInput;

    QLabel *errorLabel;

    // Layout boutons
    QHBoxLayout *buttonLayout;
    QPushButton *loginBtn;

public:
    LoginDialog();

    bool success;  // Pour savoir si la connexion est réussite
private slots:
    void tryLogin();
};

#endif // LOGINDIALOG_H
