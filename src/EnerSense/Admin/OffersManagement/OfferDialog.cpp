#include "OfferDialog.hpp"

OfferDialog::OfferDialog(QWidget* parent) :
    QDialog(parent),

    layout(new QFormLayout(this)),

    providerEdit(new QLineEdit(this)),
    nameEdit(new QLineEdit(this)),
    typeCombo(new QComboBox(this)),

    dateEdit(new QDateEdit(QDate::currentDate(), this)),

    subPriceBox(new QDoubleSpinBox(this)),
    kwhPriceBox(new QDoubleSpinBox(this)),

    btnBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    this->setWindowTitle("Détails de l'Offre");

    this->typeCombo->addItems({"Électricité", "Gaz"});

    this->dateEdit->setCalendarPopup(true);

    this->subPriceBox->setDecimals(2);
    this->subPriceBox->setMinimum(0.00);
    this->subPriceBox->setMaximum(9999.99);
    this->subPriceBox->setSuffix(" €");

    this->kwhPriceBox->setDecimals(2);
    this->kwhPriceBox->setMinimum(0.00);
    this->kwhPriceBox->setMaximum(999.99);
    this->kwhPriceBox->setSuffix(" €");

    this->layout->addRow("Fournisseur :", this->providerEdit);
    this->layout->addRow("Nom de l'offre :", this->nameEdit);
    this->layout->addRow("Type d'énergie :", this->typeCombo);
    this->layout->addRow("Date de lancement :", this->dateEdit);
    this->layout->addRow("Abonnement mensuel :", this->subPriceBox);
    this->layout->addRow("Prix du kWh :", this->kwhPriceBox);

    this->layout->addWidget(btnBox);

    QObject::connect(this->btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(this->btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
