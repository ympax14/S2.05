#include "EnerSense/EnerSenseWindow.hpp"
#include "Login/LoginDialog.hpp"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    LoginDialog login;

    if (login.exec() == QDialog::Accepted && login.success) {
        EnerSenseWindow w;
        w.show();

        return QCoreApplication::exec();
    } else {
        return EXIT_SUCCESS;
    }
}