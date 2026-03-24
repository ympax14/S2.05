#include "DataManager.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

DataManager::DataManager() : currentUser(nullptr) {
    loadData();
}

DataManager::~DataManager() {
    for (User* u : this->users)
        delete u;
}

void DataManager::loadData() {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Création d'un utilisateur par défaut.";

        // Compte administrateur par défaut
        User* adminUser = new User("admin", "admin", "Administrateur Système", true, nullptr);
        this->users.push_back(adminUser);

        // Offres par défaut
        // EDF - Électricité (Tarif Réglementé de Base)
        // Stratégie : Le standard du marché. Abonnement moyen, prix moyen.
        EnergyOffer* edfElec = new EnergyOffer("EDF", "Tarif Bleu", "Électricité");
        edfElec->addTariffVersion(QDate(2023, 2, 1), 14.30, 0.2062);
        edfElec->addTariffVersion(QDate(2024, 2, 1), 15.12, 0.2516);
        this->offers.push_back(edfElec);

        // EDF - Gaz (Avantage Gaz)
        // Note : Le prix du gaz est toujours plus bas que l'électricité, mais l'abonnement est plus cher.
        EnergyOffer* edfGaz = new EnergyOffer("EDF", "Avantage Gaz", "Gaz");
        edfGaz->addTariffVersion(QDate(2023, 1, 1), 20.50, 0.1120);
        edfGaz->addTariffVersion(QDate(2024, 7, 1), 21.10, 0.0980); // Le gaz a un peu baissé récemment
        this->offers.push_back(edfGaz);

        // TotalEnergies - Électricité (Offre Verte Fixe)
        // Stratégie : Abonnement CHER, mais kWh MOINS CHER.
        // -> Gagne le comparateur si le client simule une grosse consommation (ex: > 8000 kWh).
        EnergyOffer* totalElec = new EnergyOffer("TotalEnergies", "Offre Verte Fixe", "Électricité");
        totalElec->addTariffVersion(QDate(2023, 3, 1), 16.00, 0.1950);
        totalElec->addTariffVersion(QDate(2024, 3, 1), 17.50, 0.2310);
        this->offers.push_back(totalElec);

        // TotalEnergies - Gaz (Offre Essentielle)
        // Stratégie : Abonnement PAS CHER, mais kWh PLUS CHER.
        // -> Gagne le comparateur si le client simule une petite consommation (ex: < 2000 kWh).
        EnergyOffer* totalGaz = new EnergyOffer("TotalEnergies", "Offre Essentielle", "Gaz");
        totalGaz->addTariffVersion(QDate(2023, 1, 1), 18.00, 0.1180);
        totalGaz->addTariffVersion(QDate(2024, 1, 1), 18.20, 0.1050);
        this->offers.push_back(totalGaz);

        // 5. Engie - Électricité (Elec Référence)
        // Stratégie : Abonnement très faible, kWh très cher (parfait pour les studios)
        EnergyOffer* engieElec = new EnergyOffer("Engie", "Elec Référence 1 an", "Électricité");
        engieElec->addTariffVersion(QDate(2024, 1, 1), 12.00, 0.2650);
        this->offers.push_back(engieElec);

        // Compte client par défaut
        Contract* defaultContract = new Contract(edfElec); // On lie le contrat à l'offre
        User* defaultUser = new User("jeandupont@gmail.com", "1234", "DUPONT Jean", false, defaultContract);
        this->users.push_back(defaultUser);

        this->saveData();
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject root = doc.object();

    // Charger les offres
    QJsonArray offersArray = root["offers"].toArray();
    for (int i = 0; i < offersArray.size(); ++i) {
        this->offers.push_back(EnergyOffer::fromJson(offersArray[i].toObject()));
    }

    // Charger les utilisateurs
    QJsonArray usersArray = root["users"].toArray();
    for (int i = 0; i < usersArray.size(); ++i) {
        QJsonObject userJson = usersArray[i].toObject();
        Contract* contract = nullptr;

        if (userJson.contains("contract")) {
            QJsonObject contractJson = userJson["contract"].toObject();

            // On cherche l'offre correspondante
            EnergyOffer* linkedContract = this->getOfferByName(contractJson["providerName"].toString(), contractJson["offerName"].toString());
            if (linkedContract) contract = new Contract(linkedContract);
        }

        this->users.push_back(new User(userJson["id"].toString(), userJson["password"].toString(), userJson["fullName"].toString(), userJson["isAdmin"].toBool(), contract));
    }
}

void DataManager::saveData() const {
    QJsonObject root;
    QJsonArray usersArray;
    for (User* u : users) usersArray.append(u->toJson());
    root["users"] = usersArray;

    QJsonArray offersArray;
    for (EnergyOffer* o : offers) offersArray.append(o->toJson());
    root["offers"] = offersArray;

    QJsonDocument doc(root);
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) file.write(doc.toJson());
}

bool DataManager::authenticate(const QString& id, const QString& password) {
    for (User* u : this->users) {
        if (u->id == id && u->password == password) {
            this->currentUser = u;
            return true;
        }
    }

    return false;
}

EnergyOffer* DataManager::getOfferByName(const QString& provider, const QString& name) {
    for (EnergyOffer* o : offers) {
        if (o->providerName == provider && o->offerName == name) return o;
    }

    return nullptr;
}

void DataManager::addOffer(EnergyOffer* offer) {
    this->offers.push_back(offer);
    this->saveData();
}

void DataManager::addUser(User* user) {
    this->users.push_back(user);
    this->saveData();
}