#ifndef OFFER_HPP
#define OFFER_HPP

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>
#include <map>

// Tarif pour une puissance donnée (ex: 6 kVA, 9 kVA)
struct PowerTierTariff {
    double subscriptionPriceHT; // Abonnement HT
    double baseKwhPriceHT; // Prix kWh Base HT
    double hpKwhPriceHT; // Prix kWh Heures Pleines HT
    double hcKwhPriceHT; // Prix kWh Heures Creuses HT
};

// Taxes applicables
struct Taxes {
    double cta; // Contribution Tarifaire d'Acheminement (mensuel)
    double accise; // Ex-CSPE ou TICGN (par kWh)
};

// Tarif complet à une date T
struct ContractTariff {
    Taxes taxes;
    std::map<int, PowerTierTariff> powerTiers; // Clé = kVA (ex: 3, 6, 9)
};

class EnergyOffer {
public:
    QString providerName;
    QString offerName;
    QString energyType;
    std::map<QDate, ContractTariff> tariffHistory;

    EnergyOffer(const QString& provider, const QString& name, const QString& type) : providerName(provider), offerName(name), energyType(type) {}

    void addTariffVersion(const QDate& effectiveDate, const ContractTariff& tariff) {
        this->tariffHistory[effectiveDate] = tariff;
    }

    void removeTariffVersion(const QDate& effectiveDate) {
        this->tariffHistory.erase(effectiveDate);
    }

    ContractTariff getTariffAt(const QDate& date) const {
        if (this->tariffHistory.empty()) return {};
        auto it = this->tariffHistory.lower_bound(date);
        if (it != this->tariffHistory.begin() && (it == this->tariffHistory.end() || it->first > date))
            --it;
        return it->second;
    }

    ContractTariff getCurrentTariff() const {
        if (this->tariffHistory.empty()) return {};
        return this->tariffHistory.rbegin()->second;
    }

    QJsonObject toJson() const {
        QJsonObject json;
        json["providerName"] = this->providerName;
        json["offerName"] = this->offerName;
        json["energyType"] = this->energyType;

        QJsonArray tariffsArray;
        for (const auto& pair : this->tariffHistory) {
            QJsonObject tJson;
            tJson["date"] = pair.first.toString(Qt::ISODate);

            QJsonObject taxesJson;
            taxesJson["cta"] = pair.second.taxes.cta;
            taxesJson["accise"] = pair.second.taxes.accise;
            tJson["taxes"] = taxesJson;

            QJsonArray tiersArray;
            for (const auto& tier : pair.second.powerTiers) {
                QJsonObject tierJson;
                tierJson["kva"] = tier.first;
                tierJson["subHT"] = tier.second.subscriptionPriceHT;
                tierJson["baseHT"] = tier.second.baseKwhPriceHT;
                tierJson["hpHT"] = tier.second.hpKwhPriceHT;
                tierJson["hcHT"] = tier.second.hcKwhPriceHT;
                tiersArray.append(tierJson);
            }
            tJson["tiers"] = tiersArray;
            tariffsArray.append(tJson);
        }

        json["tariffs"] = tariffsArray;
        return json;
    }

    static EnergyOffer* fromJson(const QJsonObject& json) {
        EnergyOffer* offer = new EnergyOffer(json["providerName"].toString(), json["offerName"].toString(), json["energyType"].toString());
        QJsonArray tariffsArray = json["tariffs"].toArray();

        for (int i = 0; i < tariffsArray.size(); ++i) {
            QJsonObject tJson = tariffsArray[i].toObject();
            QDate date = QDate::fromString(tJson["date"].toString(), Qt::ISODate);

            ContractTariff tariff;
            tariff.taxes.cta = tJson["taxes"].toObject()["cta"].toDouble();
            tariff.taxes.accise = tJson["taxes"].toObject()["accise"].toDouble();

            QJsonArray tiersArray = tJson["tiers"].toArray();
            for (int j = 0; j < tiersArray.size(); ++j) {
                QJsonObject tierJson = tiersArray[j].toObject();
                PowerTierTariff pt;
                pt.subscriptionPriceHT = tierJson["subHT"].toDouble();
                pt.baseKwhPriceHT = tierJson["baseHT"].toDouble();
                pt.hpKwhPriceHT = tierJson["hpHT"].toDouble();
                pt.hcKwhPriceHT = tierJson["hcHT"].toDouble();
                tariff.powerTiers[tierJson["kva"].toInt()] = pt;
            }
            offer->addTariffVersion(date, tariff);
        }
        return offer;
    }
};

#endif // OFFER_HPP