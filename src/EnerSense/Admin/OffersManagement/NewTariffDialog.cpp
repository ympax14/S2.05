#include "NewTariffDialog.hpp"
#include <QHeaderView>

NewTariffDialog::NewTariffDialog(QWidget *parent) :
    QDialog(parent),

    mainLayout(new QVBoxLayout(this)),
    topLayout(new QFormLayout()),
    dateEdit(new QDateEdit(QDate::currentDate(), this)),
    ctaBox(new QDoubleSpinBox(this)),
    acciseBox(new QDoubleSpinBox(this)),
    tiersTable(new QTableWidget(this)),
    btnBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    this->setWindowTitle("Ajouter une évolution de tarif");
    this->setMinimumSize(650, 400);

    this->dateEdit->setCalendarPopup(true);
    this->ctaBox->setDecimals(2); this->ctaBox->setMaximum(999.99); this->ctaBox->setSuffix(" €/mois");
    this->acciseBox->setDecimals(4); this->acciseBox->setMaximum(9.9999); this->acciseBox->setSuffix(" €/kWh");

    this->topLayout->addRow("Applicable à partir du :", this->dateEdit);
    this->topLayout->addRow("CTA Mensuelle :", this->ctaBox);
    this->topLayout->addRow("Accise/CSPE :", this->acciseBox);
    this->mainLayout->addLayout(this->topLayout);

    // Configuration de la grille kVA
    const std::vector<int> kvaList = {3, 6, 9, 12, 15, 18, 36};
    this->tiersTable->setRowCount(kvaList.size());
    this->tiersTable->setColumnCount(4);
    this->tiersTable->setHorizontalHeaderLabels({"Abo HT (€)", "Base HT (€)", "HP HT (€)", "HC HT (€)"});
    this->tiersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (size_t i = 0; i < kvaList.size(); ++i) {
        this->tiersTable->setVerticalHeaderItem(i, new QTableWidgetItem(QString::number(kvaList[i]) + " kVA"));

        for (int col = 0; col < 4; ++col) {
            QDoubleSpinBox* box = new QDoubleSpinBox(this);
            box->setDecimals(4);
            box->setMaximum(9999.99);
            box->setButtonSymbols(QAbstractSpinBox::NoButtons);
            this->tiersTable->setCellWidget(i, col, box);
        }
    }
    this->mainLayout->addWidget(this->tiersTable);
    this->mainLayout->addWidget(this->btnBox);

    QObject::connect(this->btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(this->btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

std::map<int, PowerTierTariff> NewTariffDialog::getTiers() const {
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