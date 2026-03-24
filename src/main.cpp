#include "EnerSense/Admin/EnerSenseAdminWindow.hpp"
#include "EnerSense/Client/EnerSenseClientWindow.hpp"
#include "Login/LoginDialog.hpp"
#include "utils/DataManager.hpp"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    LoginDialog login;

    if (login.exec() == QDialog::Accepted && login.success) {
        User* currentUser = DataManager::getInstance().getCurrentUser();

        if (!currentUser)
            throw std::runtime_error("Erreur lors de la récupération du compte utilisateur");

        QMainWindow * window = nullptr;

        if (currentUser->isAdmin)
            window = new EnerSenseAdminWindow(nullptr, &app);
        else
            window = new EnerSenseClientWindow(nullptr, &app);

        window->show();
        int returnCode = QCoreApplication::exec();

        delete window;

        return returnCode;
    } else return EXIT_SUCCESS;
}