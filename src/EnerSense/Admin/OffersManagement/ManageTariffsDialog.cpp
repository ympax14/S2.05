#include "ManageTariffsDialog.hpp"
#include "NewTariffDialog.hpp"
#include "../../../utils/DataManager.hpp"
#include <QHeaderView>
#include <QMessageBox>

ManageTariffsDialog::ManageTariffsDialog(EnergyOffer* targetOffer, QWidget* parent) :
    QDialog(parent),
    offer(targetOffer),

    layout(new QVBoxLayout(this)),
    topLayout(new QHBoxLayout()),
    kvaFilterCombo(new QComboBox(this)),
    table(new QTableWidget(this)),
    btnLayout(new QHBoxLayout()),
    btnAdd(new QPushButton("Ajouter un tarif", this)),
    btnDel(new QPushButton("Supprimer le tarif", this))
{
    this->setWindowTitle("Historique des tarifs : " + this->offer->offerName);
    this->setMinimumSize(700, 400);

    this->kvaFilterCombo->addItems({"3 kVA", "6 kVA", "9 kVA", "12 kVA", "15 kVA", "18 kVA", "36 kVA"});
    this->kvaFilterCombo->setCurrentText("6 kVA");
    this->topLayout->addWidget(new QLabel("Afficher les prix pour :"));
    this->topLayout->addWidget(this->kvaFilterCombo);
    this->topLayout->addStretch();
    this->layout->addLayout(this->topLayout);

    this->table->setColumnCount(5);
    this->table->setHorizontalHeaderLabels({"Date", "Abo HT", "Base HT", "HP/HC HT", "Taxes (CTA/Accise)"});
    this->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->table->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->table->setSelectionMode(QAbstractItemView::SingleSelection);
    this->layout->addWidget(this->table);

    this->btnDel->setObjectName("manageTariffsBtnDelete");
    this->btnLayout->addWidget(this->btnAdd);
    this->btnLayout->addWidget(this->btnDel);
    this->layout->addLayout(this->btnLayout);

    QObject::connect(this->kvaFilterCombo, &QComboBox::currentTextChanged, this, &ManageTariffsDialog::refresh);
    QObject::connect(this->btnAdd, &QPushButton::clicked, this, &ManageTariffsDialog::addTariff);
    QObject::connect(this->btnDel, &QPushButton::clicked, this, &ManageTariffsDialog::deleteTariff);

    this->refresh();
}

void ManageTariffsDialog::addTariff() {
    NewTariffDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        ContractTariff t;
        t.taxes.cta = dialog.ctaBox->value();
        t.taxes.accise = dialog.acciseBox->value();
        t.powerTiers = dialog.getTiers();

        this->offer->addTariffVersion(dialog.dateEdit->date(), t);
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
    int kva = this->kvaFilterCombo->currentText().replace(" kVA", "").toInt();
    this->table->setRowCount(0);

    int row = 0;

    for (auto it = this->offer->tariffHistory.rbegin(); it != this->offer->tariffHistory.rend(); ++it) {
        this->table->insertRow(row);
        ContractTariff t = it->second;

        double subHT = 0.0, baseHT = 0.0;
        QString hphcStr = "-";

        if (t.powerTiers.count(kva)) {
            PowerTierTariff pt = t.powerTiers.at(kva);
            subHT = pt.subscriptionPriceHT;
            baseHT = pt.baseKwhPriceHT;
            hphcStr = QString("%1 / %2").arg(pt.hpKwhPriceHT, 0, 'f', 4).arg(pt.hcKwhPriceHT, 0, 'f', 4);
        }

        QString taxesStr = QString("%1€ / %2€").arg(t.taxes.cta).arg(t.taxes.accise, 0, 'f', 4);

        this->table->setItem(row, 0, new QTableWidgetItem(it->first.toString("dd/MM/yyyy")));
        this->table->setItem(row, 1, new QTableWidgetItem(QString::number(subHT, 'f', 2) + " €"));
        this->table->setItem(row, 2, new QTableWidgetItem(QString::number(baseHT, 'f', 4) + " €"));
        this->table->setItem(row, 3, new QTableWidgetItem(hphcStr));
        this->table->setItem(row, 4, new QTableWidgetItem(taxesStr));
        row++;
    }
}