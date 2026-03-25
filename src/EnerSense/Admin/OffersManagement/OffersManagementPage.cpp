#include "OffersManagementPage.hpp"
#include "../../../utils/DataManager.hpp"
#include "ManageTariffsDialog.hpp"
#include "OfferDialog.hpp"

#include <QHeaderView>
#include <QMessageBox>

OffersManagementPage::OffersManagementPage(QWidget* const &parent) :
    QWidget(parent), mainLayout(new QVBoxLayout(this)),

    offersTable(new QTableWidget(this)), btnLayout(new QHBoxLayout()),
    btnCreate(new QPushButton("Créer une offre", this)),
    btnEdit(new QPushButton("Éditer l'offre", this)),
    btnManageTariffs(new QPushButton("Gérer l'historique des prix", this)),
    btnDelete(new QPushButton("Supprimer l'offre", this))
{
    this->setupUI();

    QObject::connect(this->btnCreate, &QPushButton::clicked, this, &OffersManagementPage::createOffer);
    QObject::connect(this->btnEdit, &QPushButton::clicked, this, &OffersManagementPage::editOffer);
    QObject::connect(this->btnManageTariffs, &QPushButton::clicked, this, &OffersManagementPage::manageTariffs);
    QObject::connect(this->btnDelete, &QPushButton::clicked, this, &OffersManagementPage::deleteOffer);

    this->refreshTable();
}

void OffersManagementPage::setupUI() {
    this->offersTable->setColumnCount(5);
    this->offersTable->setHorizontalHeaderLabels({"Fournisseur", "Nom de l'offre", "Énergie", "Abo. Actuel (HT)", "Prix Base Actuel (HT)"});
    this->offersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->offersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->offersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    this->offersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->btnDelete->setObjectName("offersManagementBtnDelete");
    this->btnManageTariffs->setObjectName("offersManagementBtnManageTariffs");

    this->btnLayout->addWidget(this->btnCreate);
    this->btnLayout->addWidget(this->btnEdit);
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
        ContractTariff current = offer->getCurrentTariff();

        double subHT = current.powerTiers.count(6) ? current.powerTiers.at(6).subscriptionPriceHT : 0.0;
        double baseHT = current.powerTiers.count(6) ? current.powerTiers.at(6).baseKwhPriceHT : 0.0;

        this->offersTable->setItem(i, 0, new QTableWidgetItem(offer->providerName));
        this->offersTable->setItem(i, 1, new QTableWidgetItem(offer->offerName));
        this->offersTable->setItem(i, 2, new QTableWidgetItem(offer->energyType));
        this->offersTable->setItem(i, 3, new QTableWidgetItem(QString::number(subHT, 'f', 2) + " €"));
        this->offersTable->setItem(i, 4, new QTableWidgetItem(QString::number(baseHT, 'f', 4) + " €"));
    }
}

void OffersManagementPage::createOffer() {
    OfferDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        EnergyOffer* offer = new EnergyOffer(dialog.providerEdit->text(), dialog.nameEdit->text(), dialog.typeCombo->currentText());

        ContractTariff t;
        t.taxes.cta = dialog.ctaBox->value();
        t.taxes.accise = dialog.acciseBox->value();
        t.powerTiers = dialog.getTiers();

        offer->addTariffVersion(dialog.dateEdit->date(), t);
        DataManager::getInstance().addOffer(offer);
        this->refreshTable();
    }
}

void OffersManagementPage::editOffer() {
    int row = this->offersTable->currentRow();
    if (row < 0) return;

    std::vector<EnergyOffer*>& offers = DataManager::getInstance().getOffers();
    EnergyOffer* offer = offers[row];

    OfferDialog dialog(this);
    dialog.providerEdit->setText(offer->providerName);
    dialog.nameEdit->setText(offer->offerName);
    dialog.typeCombo->setCurrentText(offer->energyType);

    ContractTariff current = offer->getCurrentTariff();
    dialog.ctaBox->setValue(current.taxes.cta);
    dialog.acciseBox->setValue(current.taxes.accise);
    dialog.setTiers(current.powerTiers);

    if (!offer->tariffHistory.empty()) {
        dialog.dateEdit->setDate(offer->tariffHistory.rbegin()->first);
    }

    if (dialog.exec() == QDialog::Accepted) {
        offer->providerName = dialog.providerEdit->text();
        offer->offerName = dialog.nameEdit->text();
        offer->energyType = dialog.typeCombo->currentText();

        if (!offer->tariffHistory.empty()) {
            QDate oldDate = offer->tariffHistory.rbegin()->first;
            offer->removeTariffVersion(oldDate);
        }

        ContractTariff t;
        t.taxes.cta = dialog.ctaBox->value();
        t.taxes.accise = dialog.acciseBox->value();
        t.powerTiers = dialog.getTiers();

        offer->addTariffVersion(dialog.dateEdit->date(), t);
        DataManager::getInstance().saveData();

        this->refreshTable();
    }
}

void OffersManagementPage::manageTariffs() {
    int row = this->offersTable->currentRow();
    if (row < 0) return;

    EnergyOffer* o = DataManager::getInstance().getOffers()[row];
    ManageTariffsDialog dlg(o, this);
    dlg.exec();

    this->refreshTable();
}

void OffersManagementPage::deleteOffer() {
    int row = this->offersTable->currentRow();
    if (row < 0) return;

    std::vector<EnergyOffer*>& offers = DataManager::getInstance().getOffers();
    EnergyOffer* offerToDelete = offers[row];

    for (User* user : DataManager::getInstance().getUsers()) {
        if (user->contract && user->contract->linkedOffer == offerToDelete) {
            QMessageBox::warning(this, "Erreur", "Impossible de supprimer cette offre : des clients l'utilisent actuellement.");
            return;
        }
    }

    if (QMessageBox::question(this, "Supprimer", "Voulez-vous supprimer l'offre " + offerToDelete->offerName + " ?") == QMessageBox::Yes) {
        delete offerToDelete;
        offers.erase(offers.begin() + row);

        DataManager::getInstance().saveData();

        this->refreshTable();
    }
}