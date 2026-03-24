#include "OffersManagementPage.hpp"

#include "../../../utils/DataManager.hpp"
#include "ManageTariffsDialog.hpp"

#include "OfferDialog.hpp"

#include <QHeaderView>

OffersManagementPage::OffersManagementPage(QWidget* const &parent) :
    QWidget(parent),
    mainLayout(new QVBoxLayout(this)),
    offersTable(new QTableWidget(this)),
    btnLayout(new QHBoxLayout()),
    btnCreate(new QPushButton("Créer une offre", this)),
    btnManageTariffs(new QPushButton("Gérer l'historique des prix", this)), // NOUVEAU BOUTON
    btnDelete(new QPushButton("Supprimer l'offre", this))
{
    this->setupUI();

    QObject::connect(this->btnCreate, &QPushButton::clicked, this, &OffersManagementPage::createOffer);
    QObject::connect(this->btnManageTariffs, &QPushButton::clicked, this, &OffersManagementPage::manageTariffs);
    QObject::connect(this->btnDelete, &QPushButton::clicked, this, &OffersManagementPage::deleteOffer);

    this->refreshTable();
}

void OffersManagementPage::setupUI() {
    this->offersTable->setColumnCount(5);
    this->offersTable->setHorizontalHeaderLabels({"Fournisseur", "Nom de l'offre", "Énergie", "Abonnement Actuel", "Prix kWh Actuel"});
    this->offersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->offersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->offersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    this->offersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->btnDelete->setStyleSheet("color: red; font-weight: bold;");
    this->btnManageTariffs->setStyleSheet("background-color: #2980b9; color: white; font-weight: bold;");

    this->btnLayout->addWidget(this->btnCreate);
    this->btnLayout->addWidget(this->btnManageTariffs);
    this->btnLayout->addStretch();
    this->btnLayout->addWidget(this->btnDelete);

    this->mainLayout->addWidget(this->offersTable);
    this->mainLayout->addLayout(this->btnLayout);
}

void OffersManagementPage::refreshTable() {
    std::vector<EnergyOffer*>& offers = DataManager::getInstance().getOffers();
    this->offersTable->setRowCount(0);

    for (size_t i = 0; i < offers.size(); ++i) {
        this->offersTable->insertRow(i);
        EnergyOffer* offer = offers[i];
        ContractTariff current = offer->getCurrentTariff(); // Affiche le prix du jour

        this->offersTable->setItem(i, 0, new QTableWidgetItem(offer->providerName));
        this->offersTable->setItem(i, 1, new QTableWidgetItem(offer->offerName));
        this->offersTable->setItem(i, 2, new QTableWidgetItem(offer->energyType));
        this->offersTable->setItem(i, 3, new QTableWidgetItem(QString::number(current.subscriptionPrice, 'f', 2) + " €"));
        this->offersTable->setItem(i, 4, new QTableWidgetItem(QString::number(current.kwhPrice, 'f', 2) + " €"));
    }
}

void OffersManagementPage::createOffer() {
    OfferDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        EnergyOffer* offer = new EnergyOffer(dialog.getProviderEdit()->text(), dialog.getNameEdit()->text(), dialog.getTypeCombo()->currentText());
        offer->addTariffVersion(dialog.getDateEdit()->date(), dialog.getSubPriceBox()->value(), dialog.getKwhPriceBox()->value());

        DataManager::getInstance().addOffer(offer);
        this->refreshTable();
    }
}

void OffersManagementPage::manageTariffs() {
    int row = this->offersTable->currentRow();
    if (row < 0) return;

    EnergyOffer* o = DataManager::getInstance().getOffers()[row];
    ManageTariffsDialog dlg(o, this);

    dlg.exec(); // On attend la fermeture

    this->refreshTable(); // On met à jour le tableau principal au cas où le prix "actuel" aurait changé
}

void OffersManagementPage::deleteOffer() {
    int row = this->offersTable->currentRow();
    if (row < 0) return;

    std::vector<EnergyOffer*>& offers = DataManager::getInstance().getOffers();
    EnergyOffer* offerToDelete = offers[row];

    // SÉCURITÉ : Ne pas supprimer une offre si un client l'utilise
    for (User* user : DataManager::getInstance().getUsers()) {
        if (user->contract && user->contract->linkedOffer == offerToDelete) {
            QMessageBox::warning(this, "Erreur", "Impossible de supprimer cette offre : des clients l'utilisent actuellement.");
            return;
        }
    }

    if (QMessageBox::question(this, "Supprimer", "Voulez-vous supprimer l'offre " + offerToDelete->offerName + " ?") == QMessageBox::Yes) {
        delete offerToDelete; // Libère la mémoire
        offers.erase(offers.begin() + row);
        DataManager::getInstance().saveData();
        this->refreshTable();
    }
}

void OffersManagementPage::editOffer() {
    int row = this->offersTable->currentRow();
    if (row < 0) return;

    std::vector<EnergyOffer*>& offers = DataManager::getInstance().getOffers();
    EnergyOffer* offer = offers[row];

    OfferDialog dialog(this);
    dialog.getProviderEdit()->setText(offer->providerName);
    dialog.getNameEdit()->setText(offer->offerName);
    dialog.getTypeCombo()->setCurrentText(offer->energyType);

    // On pré-remplit avec le tarif le plus récent de l'offre
    ContractTariff current = offer->getCurrentTariff();
    dialog.getSubPriceBox()->setValue(current.subscriptionPrice);
    dialog.getKwhPriceBox()->setValue(current.kwhPrice);

    // On récupère la date de ce tarif récent
    if (!offer->tariffHistory.empty()) {
        dialog.getDateEdit()->setDate(offer->tariffHistory.rbegin()->first);
    }

    if (dialog.exec() == QDialog::Accepted) {
        offer->providerName = dialog.getProviderEdit()->text();
        offer->offerName = dialog.getNameEdit()->text();
        offer->energyType = dialog.getTypeCombo()->currentText();

        // On remplace le tarif récent par les valeurs modifiées
        if (!offer->tariffHistory.empty()) {
            QDate oldDate = offer->tariffHistory.rbegin()->first;
            offer->removeTariffVersion(oldDate);
        }

        offer->addTariffVersion(dialog.getDateEdit()->date(), dialog.getSubPriceBox()->value(), dialog.getKwhPriceBox()->value());

        DataManager::getInstance().saveData();
        this->refreshTable();
    }
}