#include "OfferDialog.hpp"
#include <QHeaderView>

OfferDialog::OfferDialog(QWidget* parent) :
    QDialog(parent),

    mainLayout(new QVBoxLayout(this)),
    topLayout(new QFormLayout()),
    providerEdit(new QLineEdit(this)),
    nameEdit(new QLineEdit(this)),
    typeCombo(new QComboBox(this)),
    dateEdit(new QDateEdit(QDate::currentDate(), this)),
    ctaBox(new QDoubleSpinBox(this)),
    acciseBox(new QDoubleSpinBox(this)),
    tiersTable(new QTableWidget(this)),
    btnBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    this->setWindowTitle("Détails de l'Offre");
    this->setMinimumSize(650, 500);

    this->typeCombo->addItems({"Électricité", "Gaz"});
    this->dateEdit->setCalendarPopup(true);
    this->ctaBox->setDecimals(2); this->ctaBox->setMaximum(999.99); this->ctaBox->setSuffix(" €/mois");
    this->acciseBox->setDecimals(4); this->acciseBox->setMaximum(9.9999); this->acciseBox->setSuffix(" €/kWh");

    this->topLayout->addRow("Fournisseur :", this->providerEdit);
    this->topLayout->addRow("Nom de l'offre :", this->nameEdit);
    this->topLayout->addRow("Type d'énergie :", this->typeCombo);
    this->topLayout->addRow("Date de lancement :", this->dateEdit);
    this->topLayout->addRow("CTA Mensuelle :", this->ctaBox);
    this->topLayout->addRow("Accise/CSPE :", this->acciseBox);
    this->mainLayout->addLayout(this->topLayout);

    const std::vector<int> kvaList = {3, 6, 9, 12, 15, 18, 36};
    this->tiersTable->setRowCount(kvaList.size());
    this->tiersTable->setColumnCount(4);
    this->tiersTable->setHorizontalHeaderLabels({"Abo HT (€)", "Base HT (€)", "HP HT (€)", "HC HT (€)"});
    this->tiersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (size_t i = 0; i < kvaList.size(); ++i) {
        this->tiersTable->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(kvaList[i]) + " kVA"));

        for (int col = 0; col < 4; ++col) {
            QDoubleSpinBox* box = new QDoubleSpinBox(this);
            box->setDecimals(4); box->setMaximum(9999.99); box->setButtonSymbols(QAbstractSpinBox::NoButtons);

            this->tiersTable->setCellWidget(i, col, box);
        }
    }

    this->mainLayout->addWidget(this->tiersTable);
    this->mainLayout->addWidget(btnBox);

    QObject::connect(this->btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(this->btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

std::map<int, PowerTierTariff> OfferDialog::getTiers() const {
    std::map<int, PowerTierTariff> tiers;
    const std::vector<int> kvaList = {3, 6, 9, 12, 15, 18, 36};

    for (size_t i = 0; i < kvaList.size(); ++i) {
        PowerTierTariff pt;
        pt.subscriptionPriceHT = static_cast<QDoubleSpinBox*>(this->tiersTable->cellWidget(i, 0))->value();
        pt.baseKwhPriceHT = static_cast<QDoubleSpinBox*>(this->tiersTable->cellWidget(i, 1))->value();
        pt.hpKwhPriceHT = static_cast<QDoubleSpinBox*>(this->tiersTable->cellWidget(i, 2))->value();
        pt.hcKwhPriceHT = static_cast<QDoubleSpinBox*>(this->tiersTable->cellWidget(i, 3))->value();
        tiers[kvaList[i]] = pt;
    }

    return tiers;
}

void OfferDialog::setTiers(const std::map<int, PowerTierTariff>& tiers) {
    const std::vector<int> kvaList = {3, 6, 9, 12, 15, 18, 36};

    for (size_t i = 0; i < kvaList.size(); ++i) {
        if (tiers.count(kvaList[i])) {
            PowerTierTariff pt = tiers.at(kvaList[i]);

            static_cast<QDoubleSpinBox*>(this->tiersTable->cellWidget(i, 0))->setValue(pt.subscriptionPriceHT);
            static_cast<QDoubleSpinBox*>(this->tiersTable->cellWidget(i, 1))->setValue(pt.baseKwhPriceHT);
            static_cast<QDoubleSpinBox*>(this->tiersTable->cellWidget(i, 2))->setValue(pt.hpKwhPriceHT);
            static_cast<QDoubleSpinBox*>(this->tiersTable->cellWidget(i, 3))->setValue(pt.hcKwhPriceHT);
        }
    }
}