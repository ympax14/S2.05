#ifndef NEWTARIFFDIALOG_HPP
#define NEWTARIFFDIALOG_HPP

#include "../../Models/Offer.hpp"
#include <QDialog>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QTableWidget>

class NewTariffDialog : public QDialog {
public:
    QVBoxLayout * const mainLayout;
    QFormLayout * const topLayout;

    QDateEdit * const dateEdit;
    QDoubleSpinBox * const ctaBox;
    QDoubleSpinBox * const acciseBox;

    QTableWidget * const tiersTable; // Le tableau des grilles tarifaires
    QDialogButtonBox * const btnBox;

    NewTariffDialog(QWidget* parent = nullptr);
    std::map<int, PowerTierTariff> getTiers() const;
};

#endif // NEWTARIFFDIALOG_HPP