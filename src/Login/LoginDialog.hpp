#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QSizePolicy>
#include <QTimer>

class LoginDialog : public QDialog {
    Q_OBJECT

private:
    static const int WINDOW_WIDTH = 400,
        WINDOW_HEIGHT = 250,
        ERROR_MESSAGE_TIMEOUT = 3000;
    // Layout principal
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;

    // Layout formulaire
    QFormLayout *formLayout;
    QLineEdit *emailInput;
    QLineEdit *passwordInput;

    QTimer* errorTimer;
    QLabel *errorLabel;

    // Layout boutons
    QHBoxLayout *buttonLayout;
    QPushButton *loginBtn, *quitBtn;

    void setupWindow();
    void setupMainLayout();
    void setupFormLayout();
    void setupErrors();
    void setupButtonsLayout();

    void connectButtons();

    void showError(const QString &message);
public:
    LoginDialog();

    bool success;  // Pour savoir si la connexion est réussite
private slots:
    void tryLogin();
};

#endif // LOGINDIALOG_H
