#include "MyContractPage.hpp"
#include "../../utils/DataManager.hpp"

#include <QHeaderView>
#include <QMessageBox>

MyContractPage::MyContractPage(QWidget* const &parent)
    : QWidget{parent},
    mainLayout(new QVBoxLayout(this)),

    infoGroup(new QGroupBox("Informations du Contrat Actuel", this)),
    infoLayout(new QFormLayout(this->infoGroup)),
    providerLabel(new QLabel("-", this)),
    energyTypeLabel(new QLabel("-", this)),

    historyGroup(new QGroupBox("Historique des Tarifs", this)),
    historyLayout(new QVBoxLayout(this->historyGroup)),
    tariffTable(new QTableWidget(this)),

    updateGroup(new QGroupBox("Mettre à jour le Contrat", this)),
    updateLayout(new QFormLayout(this->updateGroup)),
    newDateEdit(new QDateEdit(QDate::currentDate(), this)),
    newSubPriceBox(new QDoubleSpinBox(this)),
    newKwhPriceBox(new QDoubleSpinBox(this)),
    addTariffBtn(new QPushButton("Ajouter ce nouveau tarif", this))
{
    this->setupUI();
    this->connectSignals();
    this->loadContractData();
}

void MyContractPage::setupUI() {
    this->mainLayout->setSpacing(20);
    this->mainLayout->setContentsMargins(30, 30, 30, 30);

    // Informations du contrat
    this->providerLabel->setStyleSheet("font-weight: bold; color: #2c3e50;");
    this->energyTypeLabel->setStyleSheet("font-weight: bold; color: #2c3e50;");
    this->infoLayout->addRow("Fournisseur :", this->providerLabel);
    this->infoLayout->addRow("Énergie :", this->energyTypeLabel);
    this->mainLayout->addWidget(this->infoGroup);

    // Historique sous forme de tableau
    this->tariffTable->setColumnCount(4);
    this->tariffTable->setHorizontalHeaderLabels({"Date d'application", "Abonnement (€/mois)", "Prix du kWh (€)"});
    this->tariffTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->tariffTable->setEditTriggers(QAbstractItemView::NoEditTriggers); // Lecture seule
    this->tariffTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->historyLayout->addWidget(this->tariffTable);
    this->mainLayout->addWidget(this->historyGroup, 1);

    // Mises à jour du contrat
    this->newDateEdit->setCalendarPopup(true);

    this->newSubPriceBox->setSuffix(" €");
    this->newSubPriceBox->setMaximum(9999.99);
    this->newSubPriceBox->setAlignment(Qt::AlignRight);

    this->newKwhPriceBox->setSuffix(" €");
    this->newKwhPriceBox->setDecimals(4);
    this->newKwhPriceBox->setMaximum(999.9999);
    this->newKwhPriceBox->setAlignment(Qt::AlignRight);

    this->addTariffBtn->setCursor(Qt::PointingHandCursor);
    this->addTariffBtn->setStyleSheet("QPushButton { background-color: #2980b9; color: white; font-weight: bold; padding: 8px; border-radius: 4px; } QPushButton:hover { background-color: #3498db; }");

    this->updateLayout->addRow("Date de début :", this->newDateEdit);
    this->updateLayout->addRow("Nouvel Abonnement :", this->newSubPriceBox);
    this->updateLayout->addRow("Nouveau prix du kWh :", this->newKwhPriceBox);
    this->updateLayout->addRow("", this->addTariffBtn);

    this->mainLayout->addWidget(this->updateGroup);
}

void MyContractPage::connectSignals() {
    QObject::connect(this->addTariffBtn, &QPushButton::clicked, this, &MyContractPage::addNewTariff);
}

void MyContractPage::loadContractData() {
    User* currentUser = DataManager::getInstance().getCurrentUser();
    if (currentUser && currentUser->contract) {
        this->providerLabel->setText(currentUser->contract->providerName);
        this->energyTypeLabel->setText(currentUser->contract->energyType);
        this->refreshTable();
    }
}

void MyContractPage::refreshTable() {
    User* currentUser = DataManager::getInstance().getCurrentUser();
    if (!currentUser || !currentUser->contract) return;

    const auto& history = currentUser->contract->tariffHistory;
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

        QDate dateKey = it->first;
        QPushButton* deleteBtn = new QPushButton("Supprimer");
        deleteBtn->setCursor(Qt::PointingHandCursor);
        deleteBtn->setStyleSheet("color: #d9534f; border: none; text-decoration: underline; background: transparent;");

        QObject::connect(deleteBtn, &QPushButton::clicked, [this, dateKey]() {
            this->removeTariff(dateKey);
        });

        this->tariffTable->setCellWidget(row, 3, deleteBtn);

        row++;
    }
}

void MyContractPage::addNewTariff() {
    User* currentUser = DataManager::getInstance().getCurrentUser();
    if (!currentUser || !currentUser->contract) return;

    QDate effectiveDate = this->newDateEdit->date();
    double subPrice = this->newSubPriceBox->value();
    double kwhPrice = this->newKwhPriceBox->value();

    if (subPrice <= 0 || kwhPrice <= 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer des tarifs valides (supérieurs à 0).");
        return;
    }

    currentUser->contract->addTariffVersion(effectiveDate, subPrice, kwhPrice);
    DataManager::getInstance().saveData();
    this->refreshTable();

    QMessageBox::information(this, "Succès", "Le contrat a été mis à jour avec succès !");
}

void MyContractPage::removeTariff(const QDate& date) {
    User* currentUser = DataManager::getInstance().getCurrentUser();
    if (!currentUser || !currentUser->contract) return;

    // Petite sécurité : demander confirmation à l'utilisateur
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce tarif de l'historique ?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // On supprime du contrat
        currentUser->contract->removeTariffVersion(date);

        // On sauvegarde dans le fichier JSON
        DataManager::getInstance().saveData();

        // On rafraîchit l'affichage
        this->refreshTable();
    }
}