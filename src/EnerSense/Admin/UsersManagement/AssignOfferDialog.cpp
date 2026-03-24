#include "AssignOfferDialog.hpp"

#include "../../../utils/DataManager.hpp"

AssignOfferDialog::AssignOfferDialog(QWidget* parent) :
    QDialog(parent),

    layout(new QVBoxLayout(this)),

    assignOfferLabel(new QLabel("Sélectionnez l'offre à assigner au client :")),
    offerCombo(new QComboBox(this)),

    btnBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    this->setWindowTitle("Assigner une offre");

    // Charger les offres
    for (EnergyOffer* offer : DataManager::getInstance().getOffers()) {
        this->offerCombo->addItem(offer->providerName + " - " + offer->offerName + " (" + offer->energyType + ")");
    }

    this->layout->addWidget(this->assignOfferLabel);
    this->layout->addWidget(this->offerCombo);
    this->layout->addWidget(this->btnBox);

    QObject::connect(this->btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(this->btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
