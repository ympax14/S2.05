#ifndef INVOICE_HPP
#define INVOICE_HPP

#include <QString>
#include <QDate>
#include <algorithm>

class Invoice {
public:
    QString const clientName, contract;
    QDate billingDate;

    int powerKVA;
    bool isHPHC;

    // Index
    int initialIndexBase, currentIndexBase;
    int initialIndexHP, currentIndexHP;
    int initialIndexHC, currentIndexHC;

    // Tarifs figés (Hors Taxe)
    double subscriptionPriceAppliedHT;
    double baseKwhPriceAppliedHT;
    double hpKwhPriceAppliedHT;
    double hcKwhPriceAppliedHT;

    // Taxes figées
    double ctaApplied;
    double acciseApplied;

    Invoice(const QString& _clientName, const QString& _contract, int _kva, bool _hphc) :
        clientName(_clientName), contract(_contract), billingDate(QDate::currentDate()),
        powerKVA(_kva), isHPHC(_hphc),
        initialIndexBase(0), currentIndexBase(0), initialIndexHP(0), currentIndexHP(0), initialIndexHC(0), currentIndexHC(0),
        subscriptionPriceAppliedHT(0.0), baseKwhPriceAppliedHT(0.0), hpKwhPriceAppliedHT(0.0), hcKwhPriceAppliedHT(0.0),
        ctaApplied(0.0), acciseApplied(0.0) {}

    int getConsumptionBase() const { return std::max(0, currentIndexBase - initialIndexBase); }
    int getConsumptionHP() const { return std::max(0, currentIndexHP - initialIndexHP); }
    int getConsumptionHC() const { return std::max(0, currentIndexHC - initialIndexHC); }

    int getTotalConsumption() const {
        if (isHPHC) return getConsumptionHP() + getConsumptionHC();
        return getConsumptionBase();
    }

    double getTotalTTC() const {
        // TVA 5.5% sur Abonnement + CTA
        double fixedCostTTC = (subscriptionPriceAppliedHT + ctaApplied) * 1.055;

        // Calcul du coût de la molécule HT selon l'option choisie
        double energyCostHT = 0.0;
        if (isHPHC) {
            energyCostHT = (getConsumptionHP() * hpKwhPriceAppliedHT) + (getConsumptionHC() * hcKwhPriceAppliedHT);
        } else {
            energyCostHT = getConsumptionBase() * baseKwhPriceAppliedHT;
        }

        // TVA 20% sur la molécule + Accise
        double variableCostTTC = (energyCostHT + (getTotalConsumption() * acciseApplied)) * 1.20;

        return fixedCostTTC + variableCostTTC;
    }
};

#endif // INVOICE_HPP