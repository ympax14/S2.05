#ifndef OFFERDIALOG_H
#define OFFERDIALOG_H

#include "../../Models/Offer.hpp"
#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QDateEdit>
#include <QTableWidget>
#include <QDialog>

class OfferDialog : public QDialog {
public:
    QVBoxLayout * const mainLayout;
    QFormLayout * const topLayout;

    QLineEdit * const providerEdit, * const nameEdit;
    QComboBox * const typeCombo;
    QDateEdit * const dateEdit;

    QDoubleSpinBox * const ctaBox;
    QDoubleSpinBox * const acciseBox;
    QTableWidget * const tiersTable;

    QDialogButtonBox * const btnBox;

    OfferDialog(QWidget* parent = nullptr);

    std::map<int, PowerTierTariff> getTiers() const;
    void setTiers(const std::map<int, PowerTierTariff>& tiers);
};

#endif // OFFERDIALOG_H