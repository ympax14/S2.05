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

    historyGroup(new QGroupBox("Historique des Tarifs (HT)", this)),
    historyLayout(new QVBoxLayout(this->historyGroup)),
    tariffTable(new QTableWidget(this))
{
    this->setupUI();
    this->loadContractData();
}

void MyContractPage::setupUI() {
    this->mainLayout->setSpacing(20);
    this->mainLayout->setContentsMargins(30, 30, 30, 30);

    this->providerLabel->setProperty("class", "myContractPageLabels");
    this->offerLabel->setProperty("class", "myContractPageLabels");
    this->energyTypeLabel->setProperty("class", "myContractPageLabels");

    this->infoLayout->addRow("Fournisseur :", this->providerLabel);
    this->infoLayout->addRow("Offre souscrite :", this->offerLabel);
    this->infoLayout->addRow("Énergie :", this->energyTypeLabel);
    this->mainLayout->addWidget(this->infoGroup);

    this->tariffTable->setColumnCount(5);
    this->tariffTable->setHorizontalHeaderLabels({"Date", "Abo. HT", "Prix kWh HT", "CTA", "Accise (kWh)"});
    this->tariffTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->tariffTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->tariffTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->historyLayout->addWidget(this->tariffTable);

    this->mainLayout->addWidget(this->historyGroup, 1);
}

void MyContractPage::loadContractData() {
    User* currentUser = DataManager::getInstance().getCurrentUser();

    if (currentUser && currentUser->contract && currentUser->contract->linkedOffer) {
        this->providerLabel->setText(currentUser->contract->linkedOffer->providerName);

        QString optText = currentUser->contract->isHPHC ? " (Heures Pleines / Creuses)" : " (Base)";
        this->offerLabel->setText(currentUser->contract->linkedOffer->offerName + " | " + QString::number(currentUser->contract->powerKVA) + " kVA" + optText);

        this->energyTypeLabel->setText(currentUser->contract->linkedOffer->energyType);

        this->refreshTable();
    }
}

void MyContractPage::refreshTable() {
    User* currentUser = DataManager::getInstance().getCurrentUser();
    if (!currentUser || !currentUser->contract || !currentUser->contract->linkedOffer) return;

    const auto& history = currentUser->contract->linkedOffer->tariffHistory;
    int userKVA = currentUser->contract->powerKVA;
    bool isHPHC = currentUser->contract->isHPHC;

    this->tariffTable->setRowCount(0);
    int row = 0;

    for (auto it = history.rbegin(); it != history.rend(); ++it) {
        this->tariffTable->insertRow(row);

        ContractTariff t = it->second;
        double subHT = 0.0;
        QString kwhStr = "N/A";

        if (t.powerTiers.count(userKVA) > 0) {
            PowerTierTariff pt = t.powerTiers.at(userKVA);
            subHT = pt.subscriptionPriceHT;
            if (isHPHC) {
                kwhStr = QString("HP: %1€ | HC: %2€").arg(pt.hpKwhPriceHT, 0, 'f', 4).arg(pt.hcKwhPriceHT, 0, 'f', 4);
            } else {
                kwhStr = QString::number(pt.baseKwhPriceHT, 'f', 4) + " €";
            }
        }

        QTableWidgetItem* dateItem = new QTableWidgetItem(it->first.toString("dd/MM/yyyy"));
        QTableWidgetItem* subItem = new QTableWidgetItem(QString::number(subHT, 'f', 2) + " €");
        QTableWidgetItem* kwhItem = new QTableWidgetItem(kwhStr);
        QTableWidgetItem* ctaItem = new QTableWidgetItem(QString::number(t.taxes.cta, 'f', 2) + " €");
        QTableWidgetItem* acciseItem = new QTableWidgetItem(QString::number(t.taxes.accise, 'f', 4) + " €");

        dateItem->setTextAlignment(Qt::AlignCenter);
        subItem->setTextAlignment(Qt::AlignCenter);
        kwhItem->setTextAlignment(Qt::AlignCenter);
        ctaItem->setTextAlignment(Qt::AlignCenter);
        acciseItem->setTextAlignment(Qt::AlignCenter);

        this->tariffTable->setItem(row, 0, dateItem);
        this->tariffTable->setItem(row, 1, subItem);
        this->tariffTable->setItem(row, 2, kwhItem);
        this->tariffTable->setItem(row, 3, ctaItem);
        this->tariffTable->setItem(row, 4, acciseItem);

        row++;
    }
}