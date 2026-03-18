#include "EnerSense/EnerSenseWindow.hpp"
#include "Login/LoginDialog.hpp"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    LoginDialog login;

    if (login.exec() == QDialog::Accepted && login.success) {
        EnerSenseWindow w(nullptr, &app);
        w.show();

        return QCoreApplication::exec();
    } else return EXIT_SUCCESS;
}