#ifndef INVOICE_HPP
#define INVOICE_HPP

#include <QString>
#include <QDate>

class Invoice {
public:
    QString clientName;
    QDate billingDate;

    // Les données du compteur
    int initialIndex;
    int currentIndex;

    // Les tarifs figés au moment de la facture
    double subscriptionPriceApplied;
    double kwhPriceApplied;

    Invoice(const QString& _clientName) : clientName(_clientName), billingDate(QDate::currentDate()), initialIndex(0), currentIndex(0), subscriptionPriceApplied(0.0), kwhPriceApplied(0.0) {}

    // Calculs de base
    inline int getConsumption() const {
        return (currentIndex >= initialIndex) ? (currentIndex - initialIndex) : 0;
    }

    inline double getTotal() const {
        return subscriptionPriceApplied + (getConsumption() * kwhPriceApplied);
    }
};

#endif // INVOICE_HPP