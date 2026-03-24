#ifndef OFFER_HPP
#define OFFER_HPP

#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>
#include <map>

struct ContractTariff {
    double subscriptionPrice;
    double kwhPrice;
};

class EnergyOffer {
public:
    QString providerName;
    QString offerName;
    QString energyType;
    std::map<QDate, ContractTariff> tariffHistory;

    EnergyOffer(const QString& provider, const QString& name, const QString& type) : providerName(provider), offerName(name), energyType(type) {}

    void addTariffVersion(const QDate& effectiveDate, double subPrice, double kwh) {
        this->tariffHistory[effectiveDate] = {subPrice, kwh};
    }

    void removeTariffVersion(const QDate& effectiveDate) {
        this->tariffHistory.erase(effectiveDate);
    }

    ContractTariff getTariffAt(const QDate& date) const {
        if (this->tariffHistory.empty()) return {0.0, 0.0};

        std::map<QDate, ContractTariff>::const_iterator it = this->tariffHistory.lower_bound(date);

        if (it != this->tariffHistory.begin() && (it == this->tariffHistory.end() || it->first > date))
            --it;

        return it->second;
    }

    ContractTariff getCurrentTariff() const {
        if (this->tariffHistory.empty()) return {0.0, 0.0};

        return this->tariffHistory.rbegin()->second;
    }

    QJsonObject toJson() const {
        QJsonObject json;
        json["providerName"] = this->providerName;
        json["offerName"] = this->offerName;
        json["energyType"] = this->energyType;

        QJsonArray tariffsArray;
        for (const std::pair<const QDate, ContractTariff>& pair : this->tariffHistory) {
            QJsonObject tJson;
            tJson["date"] = pair.first.toString(Qt::ISODate);
            tJson["subscriptionPrice"] = pair.second.subscriptionPrice;
            tJson["kwhPrice"] = pair.second.kwhPrice;
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
            offer->addTariffVersion(date, tJson["subscriptionPrice"].toDouble(), tJson["kwhPrice"].toDouble());
        }

        return offer;
    }
};

#endif // OFFER_HPP