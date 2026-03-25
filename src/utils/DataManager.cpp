#include "DataManager.hpp"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QDebug>

DataManager::DataManager() : currentUser(nullptr) {
    loadData();
}

DataManager::~DataManager() {
    for (User* u : this->users) delete u;
}

void DataManager::loadData() {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Création de la base de données exhaustive par défaut.";

        User* adminUser = new User("admin", "admin", "Administrateur Système", true, nullptr);
        this->users.push_back(adminUser);

        // Helper pour générer facilement les paliers kVA
        auto addTiers = [](ContractTariff& t, double baseAbo, double multAbo, double baseKwh, double hpKwh, double hcKwh) {
            const std::vector<int> kvas = {3, 6, 9, 12, 15, 18, 36};
            for (int k : kvas) {
                double sub = baseAbo + (multAbo * (k - 3) / 3); // L'abo monte avec la puissance
                t.powerTiers[k] = {sub, baseKwh, hpKwh, hcKwh};
            }
        };

        // ==========================================
        // OFFRES ÉLECTRICITÉ
        // ==========================================

        // 1. EDF - Tarif Bleu (Tarif Réglementé)
        EnergyOffer* edfElec = new EnergyOffer("EDF", "Tarif Bleu (TRV)", "Électricité");
        ContractTariff edfTariff;
        edfTariff.taxes.cta = 1.63;
        edfTariff.taxes.accise = 0.021;
        // Abo commence à ~9.60 pour 3kVA, monte de ~3€ tous les 3kVA
        addTiers(edfTariff, 9.60, 3.10, 0.2060, 0.2260, 0.1710);
        edfElec->addTariffVersion(QDate(2024, 2, 1), edfTariff);
        this->offers.push_back(edfElec);

        // 2. TotalEnergies - Heures Eco (Compétitif sur le Base)
        EnergyOffer* totalElec = new EnergyOffer("TotalEnergies", "Heures Eco Elec", "Électricité");
        ContractTariff totalTariff;
        totalTariff.taxes.cta = 1.63; totalTariff.taxes.accise = 0.021;
        addTiers(totalTariff, 9.50, 3.00, 0.1920, 0.2150, 0.1620);
        totalElec->addTariffVersion(QDate(2024, 3, 1), totalTariff);
        this->offers.push_back(totalElec);

        // 3. Engie - Elec Référence (Abo cher, kWh très bas)
        EnergyOffer* engieElec = new EnergyOffer("Engie", "Elec Référence 1 an", "Électricité");
        ContractTariff engieTariff;
        engieTariff.taxes.cta = 1.63; engieTariff.taxes.accise = 0.021;
        addTiers(engieTariff, 12.50, 3.50, 0.1810, 0.1980, 0.1450);
        engieElec->addTariffVersion(QDate(2024, 1, 1), engieTariff);
        this->offers.push_back(engieElec);

        // 4. Eni - Web Elec (Gros prix de base,)
        EnergyOffer* eniElec = new EnergyOffer("Eni", "Web Elec", "Électricité");
        ContractTariff eniTariff;
        eniTariff.taxes.cta = 1.63; eniTariff.taxes.accise = 0.021;
        addTiers(eniTariff, 10.00, 3.20, 0.2100, 0.2300, 0.1800);
        eniElec->addTariffVersion(QDate(2024, 4, 1), eniTariff);
        this->offers.push_back(eniElec);

        // ==========================================
        // OFFRES GAZ
        // Note: Le gaz ne se mesure pas en kVA, mais on assigne les mêmes prix à tous les "kVA" pour simplifier la structure de données.
        // ==========================================

        // 1. EDF - Avantage Gaz
        EnergyOffer* edfGaz = new EnergyOffer("EDF", "Avantage Gaz", "Gaz");
        ContractTariff edfGazTariff;
        edfGazTariff.taxes.cta = 2.10; edfGazTariff.taxes.accise = 0.01637;
        addTiers(edfGazTariff, 20.50, 0.0, 0.0910, 0.0, 0.0); // Pas de HP/HC en gaz
        edfGaz->addTariffVersion(QDate(2024, 1, 1), edfGazTariff);
        this->offers.push_back(edfGaz);

        // 2. Engie - Gaz Tranquillité
        EnergyOffer* engieGaz = new EnergyOffer("Engie", "Gaz Tranquillité 1 an", "Gaz");
        ContractTariff engieGazTariff;
        engieGazTariff.taxes.cta = 2.10; engieGazTariff.taxes.accise = 0.01637;
        addTiers(engieGazTariff, 22.00, 0.0, 0.0860, 0.0, 0.0);
        engieGaz->addTariffVersion(QDate(2024, 2, 1), engieGazTariff);
        this->offers.push_back(engieGaz);

        // 3. TotalEnergies - Spéciale Gaz
        EnergyOffer* totalGaz = new EnergyOffer("TotalEnergies", "Spéciale Gaz", "Gaz");
        ContractTariff totalGazTariff;
        totalGazTariff.taxes.cta = 2.10; totalGazTariff.taxes.accise = 0.01637;
        addTiers(totalGazTariff, 19.50, 0.0, 0.0890, 0.0, 0.0);
        totalGaz->addTariffVersion(QDate(2024, 3, 1), totalGazTariff);
        this->offers.push_back(totalGaz);

        // ==========================================
        // COMPTE CLIENT PAR DÉFAUT
        // ==========================================
        Contract* defaultContract = new Contract(edfElec, 9, true); // Client en 9 kVA, Heures Creuses !
        User* defaultUser = new User("jeandupont@gmail.com", "1234", "DUPONT Jean", false, defaultContract);
        this->users.push_back(defaultUser);

        this->saveData();
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc(QJsonDocument::fromJson(data));
    QJsonObject root = doc.object();

    QJsonArray offersArray = root["offers"].toArray();
    for (int i = 0; i < offersArray.size(); ++i) {
        this->offers.push_back(EnergyOffer::fromJson(offersArray[i].toObject()));
    }

    QJsonArray usersArray = root["users"].toArray();
    for (int i = 0; i < usersArray.size(); ++i) {
        QJsonObject userJson = usersArray[i].toObject();
        Contract* contract = nullptr;

        if (userJson.contains("contract")) {
            QJsonObject contractJson = userJson["contract"].toObject();
            EnergyOffer* linkedContract = this->getOfferByName(contractJson["providerName"].toString(), contractJson["offerName"].toString());

            int kva = contractJson.contains("powerKVA") ? contractJson["powerKVA"].toInt() : 6;
            bool hphc = contractJson.contains("isHPHC") ? contractJson["isHPHC"].toBool() : false;

            if (linkedContract) contract = new Contract(linkedContract, kva, hphc);
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