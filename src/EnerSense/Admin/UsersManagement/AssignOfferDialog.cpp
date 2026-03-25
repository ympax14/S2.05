#include "AssignOfferDialog.hpp"
#include "../../../utils/DataManager.hpp"

AssignOfferDialog::AssignOfferDialog(QWidget* parent) :
    QDialog(parent),

    layout(new QVBoxLayout(this)),
    assignOfferLabel(new QLabel("Sélectionnez l'offre à assigner au client :")),
    offerCombo(new QComboBox(this)),
    kvaCombo(new QComboBox(this)),
    hphcCheck(new QCheckBox("Option Heures Pleines / Heures Creuses", this)),
    btnBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    this->setWindowTitle("Assigner une offre");

    // Charger les offres
    for (EnergyOffer* offer : DataManager::getInstance().getOffers()) {
        this->offerCombo->addItem(offer->providerName + " - " + offer->offerName + " (" + offer->energyType + ")");
    }

    // Valeurs standards de compteurs en France
    this->kvaCombo->addItems({"3 kVA", "6 kVA", "9 kVA", "12 kVA", "15 kVA", "18 kVA", "36 kVA"});
    this->kvaCombo->setCurrentIndex(1); // 6 kVA par défaut

    this->layout->addWidget(this->assignOfferLabel);
    this->layout->addWidget(this->offerCombo);
    this->layout->addSpacing(10);
    this->layout->addWidget(new QLabel("Puissance souscrite :"));
    this->layout->addWidget(this->kvaCombo);
    this->layout->addWidget(this->hphcCheck);
    this->layout->addSpacing(10);
    this->layout->addWidget(this->btnBox);

    QObject::connect(this->btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(this->btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}