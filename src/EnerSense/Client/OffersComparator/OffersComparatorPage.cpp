#include "OffersComparatorPage.hpp"
#include "../../../utils/DataManager.hpp"
#include <QHeaderView>
#include <vector>
#include <algorithm>

struct ComparisonResult {
    EnergyOffer* offer;
    double annualCost;
    double annualAbo;
    double annualConso;
};

OffersComparatorPage::OffersComparatorPage(QWidget* const &parent) :
    QWidget{parent},

    mainLayout(new QVBoxLayout(this)),
    simulationGroup(new QGroupBox("Simuler ma consommation annuelle", this)),
    simulationLayout(new QFormLayout(this->simulationGroup)),
    energyFilterCombo(new QComboBox(this->simulationGroup)),
    kvaFilterCombo(new QComboBox(this->simulationGroup)),
    hphcFilterCheck(new QCheckBox("Activer l'option Heures Pleines / Heures Creuses (Électricité)", this->simulationGroup)),
    consumptionBaseBox(new QSpinBox(this->simulationGroup)),
    consumptionHPBox(new QSpinBox(this->simulationGroup)),
    consumptionHCBox(new QSpinBox(this->simulationGroup)),
    resultsTable(new QTableWidget(this))
{
    this->setupUI();
    this->connectSignals();

    this->updateUIState();
    this->calculateComparisons();
}

void OffersComparatorPage::setupUI() {
    this->mainLayout->setSpacing(20);
    this->mainLayout->setContentsMargins(30, 30, 30, 30);
    this->simulationGroup->setObjectName("offersComparatorPageSimulationGroup");

    auto setupBox = [](QSpinBox* box, const QString& suffix, int defaultVal) {
        box->setMaximum(999999);
        box->setMinimum(0);
        box->setSingleStep(100);
        box->setGroupSeparatorShown(true);
        box->setSuffix(suffix);
        box->setValue(defaultVal);
        box->setAlignment(Qt::AlignRight);
        box->setObjectName("offersComparatorConsumptionBox");
    };

    setupBox(this->consumptionBaseBox, " kWh / an", 4500);
    setupBox(this->consumptionHPBox, " kWh / an (HP)", 2700);
    setupBox(this->consumptionHCBox, " kWh / an (HC)", 1800);

    this->energyFilterCombo->addItems({"Électricité", "Gaz", "Toutes les énergies"});
    this->kvaFilterCombo->addItems({"3", "6", "9", "12", "15", "18", "36"});
    this->kvaFilterCombo->setCurrentText("6");

    this->simulationLayout->addRow("Type d'énergie :", this->energyFilterCombo);
    this->simulationLayout->addRow("Puissance souscrite (kVA) :", this->kvaFilterCombo);
    this->simulationLayout->addRow("", this->hphcFilterCheck);
    this->simulationLayout->addRow("Consommation Base / Gaz :", this->consumptionBaseBox);
    this->simulationLayout->addRow("Consommation Heures Pleines :", this->consumptionHPBox);
    this->simulationLayout->addRow("Consommation Heures Creuses :", this->consumptionHCBox);

    this->mainLayout->addWidget(this->simulationGroup);

    this->resultsTable->setColumnCount(6);
    this->resultsTable->setHorizontalHeaderLabels({
        "Fournisseur", "Offre", "Énergie", "Abo. Annuel TTC", "Coût Conso. TTC", "COÛT TOTAL TTC"
    });
    this->resultsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->resultsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->resultsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->resultsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    this->resultsTable->setSortingEnabled(false);

    this->mainLayout->addWidget(this->resultsTable, 1);
}

void OffersComparatorPage::connectSignals() {
    QObject::connect(this->consumptionBaseBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &OffersComparatorPage::calculateComparisons);
    QObject::connect(this->consumptionHPBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &OffersComparatorPage::calculateComparisons);
    QObject::connect(this->consumptionHCBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &OffersComparatorPage::calculateComparisons);

    QObject::connect(this->energyFilterCombo, &QComboBox::currentTextChanged, this, &OffersComparatorPage::updateUIState);
    QObject::connect(this->energyFilterCombo, &QComboBox::currentTextChanged, this, &OffersComparatorPage::calculateComparisons);
    QObject::connect(this->kvaFilterCombo, &QComboBox::currentTextChanged, this, &OffersComparatorPage::calculateComparisons);

    QObject::connect(this->hphcFilterCheck, &QCheckBox::checkStateChanged, this, &OffersComparatorPage::updateUIState);
    QObject::connect(this->hphcFilterCheck, &QCheckBox::checkStateChanged, this, &OffersComparatorPage::calculateComparisons);
}

void OffersComparatorPage::updateUIState() {
    bool isGas = (this->energyFilterCombo->currentText() == "Gaz");
    bool isAll = (this->energyFilterCombo->currentText() == "Toutes les énergies");
    bool isHPHC = this->hphcFilterCheck->isChecked();

    // On bloque la saisie selon le type d'énergie et clause de contrat
    if (isGas) {
        this->hphcFilterCheck->setEnabled(false);
        this->hphcFilterCheck->setChecked(false); // Le gaz n'a pas de HP/HC
        this->consumptionBaseBox->setEnabled(true);
        this->consumptionHPBox->setEnabled(false);
        this->consumptionHCBox->setEnabled(false);
    } else {
        this->hphcFilterCheck->setEnabled(true);
        if (isHPHC) {
            // Si "Toutes les énergies", on garde "Base" activé pour simuler les offres gaz en même temps
            this->consumptionBaseBox->setEnabled(isAll);
            this->consumptionHPBox->setEnabled(true);
            this->consumptionHCBox->setEnabled(true);
        } else {
            this->consumptionBaseBox->setEnabled(true);
            this->consumptionHPBox->setEnabled(false);
            this->consumptionHCBox->setEnabled(false);
        }
    }
}

void OffersComparatorPage::calculateComparisons() {
    int consoBase = this->consumptionBaseBox->value();
    int consoHP = this->consumptionHPBox->value();
    int consoHC = this->consumptionHCBox->value();

    QString selectedEnergy = this->energyFilterCombo->currentText();
    int simulatedKVA = this->kvaFilterCombo->currentText().toInt();
    bool simulateHPHC = this->hphcFilterCheck->isChecked();

    std::vector<EnergyOffer*>& allOffers = DataManager::getInstance().getOffers();
    if (allOffers.empty()) {
        this->resultsTable->setRowCount(0);
        return;
    }

    std::vector<ComparisonResult> results;

    for (EnergyOffer* offer : allOffers) {
        if (selectedEnergy != "Toutes les énergies" && offer->energyType != selectedEnergy) continue;

        ContractTariff current = offer->getCurrentTariff();

        if (current.powerTiers.count(simulatedKVA) == 0) {
            if (current.powerTiers.empty()) continue;
            simulatedKVA = current.powerTiers.begin()->first;
        }

        PowerTierTariff pt = current.powerTiers[simulatedKVA];

        double annualAboHT = pt.subscriptionPriceHT * 12.0;
        double annualCTA = current.taxes.cta * 12.0;
        double costAboTTC = (annualAboHT + annualCTA) * 1.055;

        double annualConsoHT = 0.0;
        int totalConsoForOffer = 0;

        // Si c'est du gaz OU si le mode HP/HC n'est pas coché, on utilise la conso de Base
        if (offer->energyType == "Gaz" || !simulateHPHC) {
            totalConsoForOffer = consoBase;
            annualConsoHT = consoBase * pt.baseKwhPriceHT;
        } else {
            // Électricité avec option HP/HC
            totalConsoForOffer = consoHP + consoHC;
            annualConsoHT = (consoHP * pt.hpKwhPriceHT) + (consoHC * pt.hcKwhPriceHT);
        }

        double annualAccise = totalConsoForOffer * current.taxes.accise;
        double costConsoTTC = (annualConsoHT + annualAccise) * 1.20;

        results.push_back({offer, costAboTTC + costConsoTTC, costAboTTC, costConsoTTC});
    }

    std::sort(results.begin(), results.end(), [](const ComparisonResult& a, const ComparisonResult& b) {
        return a.annualCost < b.annualCost;
    });

    this->resultsTable->setRowCount(0);
    size_t lastIndex =  results.size() - 1;

    for (size_t i = 0; i < results.size(); ++i) {
        this->resultsTable->insertRow(i);

        EnergyOffer* offer = results[i].offer;
        QTableWidgetItem* providerItem = new QTableWidgetItem(offer->providerName);
        QTableWidgetItem* nameItem = new QTableWidgetItem(offer->offerName);
        QTableWidgetItem* typeItem = new QTableWidgetItem(offer->energyType);
        QTableWidgetItem* aboItem = new QTableWidgetItem(QString::number(results[i].annualAbo, 'f', 2) + " €");
        QTableWidgetItem* consoItem = new QTableWidgetItem(QString::number(results[i].annualConso, 'f', 2) + " €");
        QTableWidgetItem* totalItem = new QTableWidgetItem(QString::number(results[i].annualCost, 'f', 2) + " €");

        QFont totalFont; totalFont.setBold(true);
        totalItem->setFont(totalFont);
        aboItem->setTextAlignment(Qt::AlignCenter);
        consoItem->setTextAlignment(Qt::AlignCenter);
        totalItem->setTextAlignment(Qt::AlignCenter);

        if (i == 0) {
            providerItem->setBackground(QColorConstants::Svg::darkolivegreen);
            nameItem->setBackground(QColorConstants::Svg::darkolivegreen);
            typeItem->setBackground(QColorConstants::Svg::darkolivegreen);
            aboItem->setBackground(QColorConstants::Svg::darkolivegreen);
            consoItem->setBackground(QColorConstants::Svg::darkolivegreen);
            totalItem->setBackground(QColorConstants::Svg::darkolivegreen);
        } else if (i == lastIndex && lastIndex > 0) {
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