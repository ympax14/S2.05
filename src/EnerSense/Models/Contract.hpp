#ifndef CONTRACT_HPP
#define CONTRACT_HPP

#include <map>

#include <QString>
#include <QDate>
#include <QJsonArray>
#include <QJsonObject>

// Représente une version des tarifs du contrat à une date donnée
struct ContractTariff {
    double subscriptionPrice; // Prix de l'abonnement (Coût fixe)
    double kwhPrice; // Prix du kWh (Coût variable)
};

class Contract {
public:
    QString providerName;
    QString energyType; // "Électricité", mais n'importe quel "type" peut être rentré

    // std::map trie naturellement par clé (ici QDate).
    // Cela nous permet de garder un historique des tarifs
    std::map<QDate, ContractTariff> tariffHistory;

    Contract(const QString& provider, const QString& type) : providerName(provider), energyType(type) {}

    // Ajouter une mise à jour du contrat
    void addTariffVersion(const QDate& effectiveDate, double subPrice, double kwh) {
        tariffHistory[effectiveDate] = {subPrice, kwh};
    }

    // Retirer une mise à jour du contrat
    void removeTariffVersion(const QDate& effectiveDate) {
        tariffHistory.erase(effectiveDate);
    }

    // Récupérer le tarif applicable à une date précise
    ContractTariff getTariffAt(const QDate& date) const {
        if (tariffHistory.empty()) return {0.0, 0.0};

        // Cherche le tarif le plus récent par rapport à la date donnée
        auto it = tariffHistory.lower_bound(date);

        // Si ContractTariff n'existe pas à cette date donnée, on prend la version juste avant
        if (it != tariffHistory.begin() && (it == tariffHistory.end() || it->first > date))
            --it;

        return it->second;
    }

    // SÉRIALISATION JSON
    QJsonObject toJson() const {
        QJsonObject json;
        json["providerName"] = providerName;
        json["energyType"] = energyType;

        QJsonArray tariffsArray;
        for (const auto& pair : tariffHistory) {
            QJsonObject tJson;
            tJson["date"] = pair.first.toString(Qt::ISODate); // Sauvegarde la date au format AAAA-MM-JJ
            tJson["subscriptionPrice"] = pair.second.subscriptionPrice;
            tJson["kwhPrice"] = pair.second.kwhPrice;
            tariffsArray.append(tJson);
        }
        json["tariffs"] = tariffsArray;
        return json;
    }

    static Contract* fromJson(const QJsonObject& json) {
        Contract* c = new Contract(json["providerName"].toString(), json["energyType"].toString());
        QJsonArray tariffsArray = json["tariffs"].toArray();
        for (int i = 0; i < tariffsArray.size(); ++i) {
            QJsonObject tJson = tariffsArray[i].toObject();
            QDate date = QDate::fromString(tJson["date"].toString(), Qt::ISODate);
            c->addTariffVersion(date, tJson["subscriptionPrice"].toDouble(), tJson["kwhPrice"].toDouble());
        }
        return c;
    }
};

#endif // CONTRACT_HPP
