#include "NewTariffDialog.hpp"

NewTariffDialog::NewTariffDialog(QWidget *parent) :
    QDialog(parent),

    layout(new QFormLayout(this)),

    dateEdit(new QDateEdit(QDate::currentDate(), this)),
    subPriceBox(new QDoubleSpinBox(this)),
    kwhPriceBox(new QDoubleSpinBox(this)),

    btnBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    this->setWindowTitle("Ajouter une évolution de tarif");

    this->dateEdit->setCalendarPopup(true);

    this->subPriceBox->setDecimals(2);
    this->subPriceBox->setMinimum(0.0);
    this->subPriceBox->setMaximum(9999.99);
    this->subPriceBox->setSuffix(" €");

    this->kwhPriceBox->setDecimals(2);
    this->kwhPriceBox->setMinimum(0.0);
    this->kwhPriceBox->setMaximum(999.99);
    this->kwhPriceBox->setSuffix(" €");

    this->layout->addRow("Applicable à partir du :", this->dateEdit);
    this->layout->addRow("Nouvel Abonnement :", this->subPriceBox);
    this->layout->addRow("Nouveau kWh :", this->kwhPriceBox);

    this->layout->addWidget(this->btnBox);

    this->connect(this->btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    this->connect(this->btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
