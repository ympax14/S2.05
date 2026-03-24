#include "MyContractPage.hpp"
#include "../../../utils/DataManager.hpp"

#include <QHeaderView>
#include <QMessageBox>

MyContractPage::MyContractPage(QWidget* const &parent)
    : QWidget{parent},
    mainLayout(new QVBoxLayout(this)),

    infoGroup(new QGroupBox("Informations du Contrat Actuel", this)),
    infoLayout(new QFormLayout(this->infoGroup)),
    providerLabel(new QLabel("-", this)),
    offerLabel(new QLabel("-", this)),
    energyTypeLabel(new QLabel("-", this)),

    historyGroup(new QGroupBox("Historique des Tarifs", this)),
    historyLayout(new QVBoxLayout(this->historyGroup)),
    tariffTable(new QTableWidget(this))
{
    this->setupUI();
    this->loadContractData();
}

void MyContractPage::setupUI() {
    this->mainLayout->setSpacing(20);
    this->mainLayout->setContentsMargins(30, 30, 30, 30);

    // Informations du contrat
    this->providerLabel->setStyleSheet("font-weight: bold; color: #2c3e50;");
    this->offerLabel->setStyleSheet("font-weight: bold; color: #2c3e50;");
    this->energyTypeLabel->setStyleSheet("font-weight: bold; color: #2c3e50;");
    this->infoLayout->addRow("Fournisseur :", this->providerLabel);
    this->infoLayout->addRow("Offre souscrite :", this->offerLabel);
    this->infoLayout->addRow("Énergie :", this->energyTypeLabel);
    this->mainLayout->addWidget(this->infoGroup);

    // Historique sous forme de tableau
    this->tariffTable->setColumnCount(3);
    this->tariffTable->setHorizontalHeaderLabels({"Date d'application", "Abonnement (€/mois)", "Prix du kWh (€)", "Actions"});
    this->tariffTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->tariffTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // Lecture seule
    this->tariffTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->historyLayout->addWidget(this->tariffTable);

    this->mainLayout->addWidget(this->historyGroup, 1);
}

void MyContractPage::loadContractData() {
    User* currentUser = DataManager::getInstance().getCurrentUser();

    if (currentUser && currentUser->contract && currentUser->contract->linkedOffer) {
        this->providerLabel->setText(currentUser->contract->linkedOffer->providerName);
        this->offerLabel->setText(currentUser->contract->linkedOffer->offerName);
        this->energyTypeLabel->setText(currentUser->contract->linkedOffer->energyType);
        this->refreshTable();
    }
}

void MyContractPage::refreshTable() {
    User* currentUser = DataManager::getInstance().getCurrentUser();
    if (!currentUser || !currentUser->contract || !currentUser->contract->linkedOffer) return;

    const auto& history = currentUser->contract->linkedOffer->tariffHistory;
    this->tariffTable->setRowCount(0);

    int row = 0;

    // Parcourt la map à l'envers pour afficher les tarifs les plus récents en haut
    for (auto it = history.rbegin(); it != history.rend(); ++it) {
        this->tariffTable->insertRow(row);

        QTableWidgetItem* dateItem = new QTableWidgetItem(it->first.toString("dd/MM/yyyy"));
        QTableWidgetItem* subItem = new QTableWidgetItem(QString::number(it->second.subscriptionPrice, 'f', 2) + " €");
        QTableWidgetItem* kwhItem = new QTableWidgetItem(QString::number(it->second.kwhPrice, 'f', 4) + " €");

        dateItem->setTextAlignment(Qt::AlignCenter);
        subItem->setTextAlignment(Qt::AlignCenter);
        kwhItem->setTextAlignment(Qt::AlignCenter);

        this->tariffTable->setItem(row, 0, dateItem);
        this->tariffTable->setItem(row, 1, subItem);
        this->tariffTable->setItem(row, 2, kwhItem);

        row++;
    }
}