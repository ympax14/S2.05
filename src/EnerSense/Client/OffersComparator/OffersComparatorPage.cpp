#include "OffersComparatorPage.hpp"
#include "../../../utils/DataManager.hpp"

#include <QHeaderView>
#include <vector>
#include <algorithm>

// Structure temporaire pour faciliter le tri des résultats
struct ComparisonResult {
    EnergyOffer* offer;
    double annualCost;
};

OffersComparatorPage::OffersComparatorPage(QWidget* const &parent) :
    QWidget{parent},

    mainLayout(new QVBoxLayout(this)),

    simulationGroup(new QGroupBox("Simuler ma consommation annuelle", this)),
    simulationLayout(new QHBoxLayout(this->simulationGroup)),
    instructionLabel(new QLabel("Consommation estimée :", this->simulationGroup)),
    consumptionBox(new QSpinBox(this->simulationGroup)),
    unitLabel(new QLabel("kWh / an", this->simulationGroup)),
    energyFilterCombo(new QComboBox(this->simulationGroup)),

    resultsTable(new QTableWidget(this))
{
    this->setupUI();
    this->connectSignals();

    // Valeur par défaut pour une consommation moyenne
    this->consumptionBox->setValue(4500);
    this->calculateComparisons();
}

void OffersComparatorPage::setupUI() {
    this->mainLayout->setSpacing(20);
    this->mainLayout->setContentsMargins(30, 30, 30, 30);

    this->simulationGroup->setStyleSheet("QGroupBox { font-weight: bold; border: 1px solid #e0e0e0; border-radius: 5px; margin-top: 10px; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; color: #555; }");

    QFont labelFont = this->instructionLabel->font();
    labelFont.setPointSize(11);
    this->instructionLabel->setFont(labelFont);
    this->unitLabel->setFont(labelFont);

    this->consumptionBox->setMaximum(999999);
    this->consumptionBox->setMinimum(0);
    this->consumptionBox->setSingleStep(100);
    this->consumptionBox->setGroupSeparatorShown(true);
    this->consumptionBox->setMinimumWidth(150);
    this->consumptionBox->setAlignment(Qt::AlignRight);

    QFont spinFont = this->consumptionBox->font();
    spinFont.setPointSize(12);
    spinFont.setBold(true);
    this->consumptionBox->setFont(spinFont);

    this->energyFilterCombo->addItems({"Électricité", "Gaz", "Toutes les énergies"});

    this->simulationLayout->addWidget(this->instructionLabel);
    this->simulationLayout->addWidget(this->consumptionBox);
    this->simulationLayout->addWidget(this->unitLabel);
    this->simulationLayout->addSpacing(20); // Un peu d'espace
    this->simulationLayout->addWidget(new QLabel("Type d'énergie :", this->simulationGroup));
    this->simulationLayout->addWidget(this->energyFilterCombo);
    this->simulationLayout->addStretch(); // Pousse les champs à gauche

    this->mainLayout->addWidget(this->simulationGroup);

    // Tableau des résultats
    this->resultsTable->setColumnCount(6);
    this->resultsTable->setHorizontalHeaderLabels({
        "Fournisseur",
        "Offre",
        "Énergie",
        "Abo. Annuel",
        "Coût Conso.",
        "COÛT TOTAL ESTIMÉ"
    });

    this->resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    this->resultsTable->setSortingEnabled(false); // On gère le tri nous-mêmes

    this->mainLayout->addWidget(this->resultsTable, 1);
}

void OffersComparatorPage::connectSignals() {
    // Recalcule tout dès que le client change la valeur
    QObject::connect(this->consumptionBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &OffersComparatorPage::calculateComparisons);

    // Permet le tri du type d'énergie
    QObject::connect(this->energyFilterCombo, &QComboBox::currentTextChanged, this, &OffersComparatorPage::calculateComparisons);
}

void OffersComparatorPage::calculateComparisons() {
    int consumption = this->consumptionBox->value();
    QString selectedEnergy = this->energyFilterCombo->currentText();
    std::vector<EnergyOffer*>& allOffers = DataManager::getInstance().getOffers();

    if (allOffers.empty()) {
        this->resultsTable->setRowCount(0);
        return;
    }

    // Calculer le coût pour chaque offre
    std::vector<ComparisonResult> results;
    for (EnergyOffer* offer : allOffers) {
        if (selectedEnergy != "Toutes les énergies" && offer->energyType != selectedEnergy) {
            continue; // On passe à l'offre suivante si l'énergie ne correspond pas
        }

        ContractTariff current = offer->getCurrentTariff();

        // Coût annuel = (Abonnement mensuel * 12) + (Consommation * Prix du kWh)
        double annualCost = (current.subscriptionPrice * 12.0) + (consumption * current.kwhPrice);

        results.push_back({offer, annualCost});
    }

    // Trier du moins cher au plus cher
    std::sort(results.begin(), results.end(), [](const ComparisonResult& a, const ComparisonResult& b) {
        return a.annualCost < b.annualCost;
    });

    this->resultsTable->setRowCount(0);

    for (size_t i = 0; i < results.size(); ++i) {
        this->resultsTable->insertRow(i);

        EnergyOffer* offer = results[i].offer;
        ContractTariff current = offer->getCurrentTariff();
        double totalAbo = current.subscriptionPrice * 12.0;
        double totalConso = consumption * current.kwhPrice;

        QTableWidgetItem* providerItem = new QTableWidgetItem(offer->providerName);
        QTableWidgetItem* nameItem = new QTableWidgetItem(offer->offerName);
        QTableWidgetItem* typeItem = new QTableWidgetItem(offer->energyType);
        QTableWidgetItem* aboItem = new QTableWidgetItem(QString::number(totalAbo, 'f', 2) + " €");
        QTableWidgetItem* consoItem = new QTableWidgetItem(QString::number(totalConso, 'f', 2) + " €");

        QTableWidgetItem* totalItem = new QTableWidgetItem(QString::number(results[i].annualCost, 'f', 2) + " €");

        QFont totalFont;
        totalFont.setBold(true);
        totalItem->setFont(totalFont);

        aboItem->setTextAlignment(Qt::AlignCenter);
        consoItem->setTextAlignment(Qt::AlignCenter);
        totalItem->setTextAlignment(Qt::AlignCenter);

        // Mettre en évidence la meilleure offre (la première après le tri)
        if (i == 0) {
            providerItem->setBackground(QColorConstants::Svg::darkolivegreen);
            nameItem->setBackground(QColorConstants::Svg::darkolivegreen);
            typeItem->setBackground(QColorConstants::Svg::darkolivegreen);
            aboItem->setBackground(QColorConstants::Svg::darkolivegreen);
            consoItem->setBackground(QColorConstants::Svg::darkolivegreen);
            totalItem->setBackground(QColorConstants::Svg::darkolivegreen);
        }

        // Mettre en évidence la pire offre (la dernière après le tri)
        if (i == results.size() - 1) {
            providerItem->setBackground(QColorConstants::Svg::red);
            nameItem->setBackground(QColorConstants::Svg::red);
            typeItem->setBackground(QColorConstants::Svg::red);
            aboItem->setBackground(QColorConstants::Svg::red);
            consoItem->setBackground(QColorConstants::Svg::red);
            totalItem->setBackground(QColorConstants::Svg::red);
        }

        this->resultsTable->setItem(i, 0, providerItem);
        this->resultsTable->setItem(i, 1, nameItem);
        this->resultsTable->setItem(i, 2, typeItem);
        this->resultsTable->setItem(i, 3, aboItem);
        this->resultsTable->setItem(i, 4, consoItem);
        this->resultsTable->setItem(i, 5, totalItem);
    }
}