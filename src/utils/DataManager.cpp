#include "DataManager.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

DataManager::DataManager() : currentUser(nullptr) {
    loadData();
}

DataManager::~DataManager() {
    for (User* u : users) delete u;
}

void DataManager::loadData() {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Aucun fichier de données trouvé. Création d'un utilisateur par défaut.";

        // Création d'un contrat de base avec un historique
        Contract* defContract = new Contract("EDF", "Électricité");
        defContract->addTariffVersion(QDate(2023, 1, 1), 14.50, 0.2062); // Ancien prix
        defContract->addTariffVersion(QDate(2024, 2, 1), 15.12, 0.2516); // Nouveau prix

        // Utilisateur par défaut
        User* defaultUser = new User("jeandupont@gmail.com", "1234", "Jean Dupont", defContract);
        users.push_back(defaultUser);
        saveData(); // On crée le fichier initial
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonArray usersArray = doc.array();

    for (int i = 0; i < usersArray.size(); ++i) {
        users.push_back(User::fromJson(usersArray[i].toObject()));
    }
}

void DataManager::saveData() const {
    QJsonArray usersArray;
    for (User* u : users) {
        usersArray.append(u->toJson());
    }

    QJsonDocument doc(usersArray);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
    }
}

bool DataManager::authenticate(const QString& id, const QString& password) {
    for (User* u : users) {
        if (u->id == id && u->password == password) {
            currentUser = u;
            return true;
        }
    }
    return false;
}

void DataManager::addUser(User* user) {
    users.push_back(user);
    saveData();
}