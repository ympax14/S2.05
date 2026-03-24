#include "ManageTariffsDialog.hpp"

#include "NewTariffDialog.hpp"
#include "../../../utils/DataManager.hpp"

#include <QHeaderView>
#include <QMessageBox>

ManageTariffsDialog::ManageTariffsDialog(EnergyOffer* targetOffer, QWidget* parent) :
    QDialog(parent),

    offer(targetOffer),

    layout(new QVBoxLayout(this)),

    table(new QTableWidget(this)),

    btnLayout(new QHBoxLayout()),
    btnAdd(new QPushButton("Ajouter un tarif", this)),
    btnDel(new QPushButton("Supprimer le tarif", this))
{
    this->setWindowTitle("Historique des tarifs : " + this->offer->offerName);
    this->setMinimumSize(500, 300);

    this->table->setColumnCount(3);
    this->table->setHorizontalHeaderLabels({"Date", "Abonnement", "Prix kWh"});
    this->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->table->setSelectionMode(QAbstractItemView::SingleSelection);
    this->layout->addWidget(table);

    this->btnDel->setStyleSheet("color: red;");
    this->btnLayout->addWidget(btnAdd);
    this->btnLayout->addWidget(btnDel);
    this->layout->addLayout(btnLayout);

    QObject::connect(this->btnAdd, &QPushButton::clicked, this, &ManageTariffsDialog::addTariff);
    QObject::connect(this->btnDel, &QPushButton::clicked, this, &ManageTariffsDialog::deleteTariff);

    this->refresh();
}

void ManageTariffsDialog::addTariff() {
    NewTariffDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        this->offer->addTariffVersion(dialog.dateEdit->date(), dialog.subPriceBox->value(), dialog.kwhPriceBox->value());

        DataManager::getInstance().saveData();

        this->refresh();
    }
}

void ManageTariffsDialog::deleteTariff() {
    int row = this->table->currentRow();
    if (row < 0) return;

    if (this->offer->tariffHistory.size() <= 1) {
        QMessageBox::warning(this, "Erreur", "Une offre doit avoir au moins un tarif !");
        return;
    }

    QDate selectedDate = QDate::fromString(this->table->item(row, 0)->text(), "dd/MM/yyyy");
    this->offer->removeTariffVersion(selectedDate);

    DataManager::getInstance().saveData();

    this->refresh();
}

void ManageTariffsDialog::refresh()  {
    this->table->setRowCount(0);

    int row = 0;
    for (auto it = offer->tariffHistory.rbegin(); it != offer->tariffHistory.rend(); ++it) {
        this->table->insertRow(row);
        this->table->setItem(row, 0, new QTableWidgetItem(it->first.toString("dd/MM/yyyy")));
        this->table->setItem(row, 1, new QTableWidgetItem(QString::number(it->second.subscriptionPrice, 'f', 2) + " €"));
        this->table->setItem(row, 2, new QTableWidgetItem(QString::number(it->second.kwhPrice, 'f', 2) + " €"));
        row++;
    }
}