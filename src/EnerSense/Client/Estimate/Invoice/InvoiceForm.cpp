#include "InvoiceForm.hpp"
#include "../../../../utils/DataManager.hpp"
#include <QFont>
#include <QFrame>

InvoiceForm::InvoiceForm(Invoice* const &_invoice, QWidget* const &parent) :
    QWidget(parent),
    invoice(_invoice),

    mainLayout(new QVBoxLayout(this)),
    scrollArea(new QScrollArea(this)),
    scrollAreaWidgetContents(new QWidget(this->scrollArea)),
    centeringLayout(new QHBoxLayout(this->scrollAreaWidgetContents)),
    documentContainer(new QWidget(this->scrollAreaWidgetContents)),
    documentLayout(new QVBoxLayout(this->documentContainer)),

    detailsGroup(new QGroupBox("Informations Générales", this->documentContainer)),
    detailsGroupLayout(new QFormLayout(this->detailsGroup)),
    clientNameLabel(new QLabel(this->detailsGroup)),
    contractLabel(new QLabel(this->detailsGroup)),
    billingDateEdit(new QDateEdit(this->detailsGroup)),

    meterGroup(new QGroupBox("Relevés Compteur", this->documentContainer)),
    meterGroupLayout(new QFormLayout(this->meterGroup)),
    initialIndexBox(new QSpinBox(this->meterGroup)),
    currentIndexBox(new QSpinBox(this->meterGroup)),
    initialIndexHPBox(new QSpinBox(this->meterGroup)),
    currentIndexHPBox(new QSpinBox(this->meterGroup)),
    initialIndexHCBox(new QSpinBox(this->meterGroup)),
    currentIndexHCBox(new QSpinBox(this->meterGroup)),

    tariffGroup(new QGroupBox("Tarifs Appliqués (HT)", this->documentContainer)),
    tariffGroupLayout(new QFormLayout(this->tariffGroup)),
    subscriptionPriceBox(new QDoubleSpinBox(this->tariffGroup)),
    kwhPriceBox(new QDoubleSpinBox(this->tariffGroup)),
    hpKwhPriceBox(new QDoubleSpinBox(this->tariffGroup)),
    hcKwhPriceBox(new QDoubleSpinBox(this->tariffGroup)),
    taxesInfoLabel(new QLabel(this->tariffGroup)),

    footerLayout(new QHBoxLayout()),
    consumptionLabel(new QLabel("Consommation : 0 kWh", this->documentContainer)),
    totalLabel(new QLabel("Total TTC : 0.00 €", this->documentContainer))
{
    this->setWindowTitle(QString("Estimation de %1 [%2]").arg(this->invoice->clientName, this->invoice->billingDate.toString("dd/MM/yyyy")));
    this->setupUI();
    this->connectSignals();
    this->updateTariffsFromDate();
    this->calculateTotal();
}

void InvoiceForm::setupUI() {
    this->mainLayout->setContentsMargins(0, 0, 0, 0);
    this->mainLayout->addWidget(this->scrollArea);
    this->scrollArea->setWidgetResizable(true);
    this->scrollArea->setFrameShape(QFrame::NoFrame);
    this->scrollArea->setObjectName("invoiceFormScrollArea");
    this->scrollArea->setWidget(this->scrollAreaWidgetContents);

    this->centeringLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    this->centeringLayout->setContentsMargins(30, 30, 30, 30);
    this->documentContainer->setMinimumWidth(600);
    this->documentContainer->setMaximumWidth(800);
    this->documentContainer->setObjectName("invoiceDocument");
    this->documentLayout->setContentsMargins(40, 40, 40, 40);
    this->documentLayout->setSpacing(20);
    this->centeringLayout->addWidget(this->documentContainer);

    QLabel* docTitle = new QLabel("ESTIMATION FACTURE D'ÉNERGIE", this->documentContainer);
    docTitle->setObjectName("invoiceFormTitleLabel");
    QFont titleFont = docTitle->font();
    titleFont.setLetterSpacing(QFont::AbsoluteSpacing, 2.0);
    docTitle->setFont(titleFont);
    this->documentLayout->addWidget(docTitle);

    // En-tête
    this->detailsGroup->setProperty("class", "invoiceGroupBox");
    this->clientNameLabel->setText(this->invoice->clientName);
    this->detailsGroupLayout->addRow("Client :", this->clientNameLabel);

    QString optText = invoice->isHPHC ? " (Heures Pleines / Creuses)" : " (Base)";
    this->contractLabel->setText(this->invoice->contract + " | " + QString::number(invoice->powerKVA) + " kVA" + optText);
    this->detailsGroupLayout->addRow("Contrat: ", this->contractLabel);

    this->billingDateEdit->setDate(this->invoice->billingDate);
    this->billingDateEdit->setCalendarPopup(true);
    this->detailsGroupLayout->addRow("Date de facturation :", this->billingDateEdit);
    this->documentLayout->addWidget(this->detailsGroup);

    // Relevés
    this->meterGroup->setProperty("class", "invoiceGroupBox");

    auto setupSpinBox = [](QSpinBox* box) {
        box->setMaximum(999999);
        box->setSuffix(" kWh");
        box->setAlignment(Qt::AlignRight);
    };

    setupSpinBox(this->initialIndexBox);
    setupSpinBox(this->currentIndexBox);
    setupSpinBox(this->initialIndexHPBox);
    setupSpinBox(this->currentIndexHPBox);
    setupSpinBox(this->initialIndexHCBox);
    setupSpinBox(this->currentIndexHCBox);

    if (this->invoice->isHPHC) {
        this->initialIndexBox->hide(); this->currentIndexBox->hide();
        this->meterGroupLayout->addRow("Index HP Précédent (N-1) :", this->initialIndexHPBox);
        this->meterGroupLayout->addRow("Index HP Actuel (N) :", this->currentIndexHPBox);
        this->meterGroupLayout->addRow("Index HC Précédent (N-1) :", this->initialIndexHCBox);
        this->meterGroupLayout->addRow("Index HC Actuel (N) :", this->currentIndexHCBox);
    } else {
        this->initialIndexHPBox->hide(); this->currentIndexHPBox->hide();
        this->initialIndexHCBox->hide(); this->currentIndexHCBox->hide();
        this->meterGroupLayout->addRow("Index Base Précédent (N-1) :", this->initialIndexBox);
        this->meterGroupLayout->addRow("Index Base Actuel (N) :", this->currentIndexBox);
    }
    this->documentLayout->addWidget(this->meterGroup);

    // Tarifs
    this->tariffGroup->setProperty("class", "invoiceGroupBox");

    auto setupDoubleBox = [](QDoubleSpinBox* box, int decimals, const QString& suffix) {
        box->setReadOnly(true);
        box->setButtonSymbols(QAbstractSpinBox::NoButtons);
        box->setSuffix(suffix);
        box->setDecimals(decimals);
        box->setAlignment(Qt::AlignRight);
    };

    setupDoubleBox(this->subscriptionPriceBox, 2, " € / mois");
    setupDoubleBox(this->kwhPriceBox, 4, " € / kWh");
    setupDoubleBox(this->hpKwhPriceBox, 4, " € / kWh");
    setupDoubleBox(this->hcKwhPriceBox, 4, " € / kWh");

    this->tariffGroupLayout->addRow("Abonnement HT :", this->subscriptionPriceBox);

    // Affichage dynamique des prix
    if (this->invoice->isHPHC) {
        this->kwhPriceBox->hide();
        this->tariffGroupLayout->addRow("Prix kWh HP HT :", this->hpKwhPriceBox);
        this->tariffGroupLayout->addRow("Prix kWh HC HT :", this->hcKwhPriceBox);
    } else {
        this->hpKwhPriceBox->hide();
        this->hcKwhPriceBox->hide();
        this->tariffGroupLayout->addRow("Prix du kWh HT :", this->kwhPriceBox);
    }

    this->tariffGroupLayout->addRow("Taxes incluses (CTA / Accise) :", this->taxesInfoLabel);
    this->documentLayout->addWidget(this->tariffGroup);
    this->documentLayout->addStretch(1);

    // Footer
    QFrame* separator = new QFrame(this->documentContainer);
    separator->setFrameShape(QFrame::HLine);
    separator->setObjectName("invoiceFormSeparator");
    this->documentLayout->addWidget(separator);

    this->consumptionLabel->setObjectName("invoiceFormConsumptionLabel");
    this->totalLabel->setObjectName("invoiceFormTotalLabel");
    this->footerLayout->addWidget(this->consumptionLabel);
    this->footerLayout->addStretch();
    this->footerLayout->addWidget(this->totalLabel);
    this->documentLayout->addLayout(this->footerLayout);
}

void InvoiceForm::connectSignals() {
    QObject::connect(this->billingDateEdit, &QDateEdit::dateChanged, this, &InvoiceForm::updateTariffsFromDate);
    QObject::connect(this->initialIndexBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &InvoiceForm::calculateTotal);
    QObject::connect(this->currentIndexBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &InvoiceForm::calculateTotal);
    QObject::connect(this->initialIndexHPBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &InvoiceForm::calculateTotal);
    QObject::connect(this->currentIndexHPBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &InvoiceForm::calculateTotal);
    QObject::connect(this->initialIndexHCBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &InvoiceForm::calculateTotal);
    QObject::connect(this->currentIndexHCBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &InvoiceForm::calculateTotal);
}

void InvoiceForm::updateTariffsFromDate() {
    this->invoice->billingDate = this->billingDateEdit->date();
    this->setWindowTitle(QString("Estimation de %1 [%2]").arg(this->invoice->clientName, this->invoice->billingDate.toString("dd/MM/yyyy")));

    User* currentUser = DataManager::getInstance().getCurrentUser();

    if (currentUser && currentUser->contract && currentUser->contract->linkedOffer) {
        ContractTariff tariff = currentUser->contract->linkedOffer->getTariffAt(this->invoice->billingDate);
        int kva = currentUser->contract->powerKVA;

        if (tariff.powerTiers.count(kva) > 0) {
            PowerTierTariff pt = tariff.powerTiers[kva];

            this->invoice->subscriptionPriceAppliedHT = pt.subscriptionPriceHT;
            this->invoice->baseKwhPriceAppliedHT = pt.baseKwhPriceHT;
            this->invoice->hpKwhPriceAppliedHT = pt.hpKwhPriceHT;
            this->invoice->hcKwhPriceAppliedHT = pt.hcKwhPriceHT;

            this->invoice->ctaApplied = tariff.taxes.cta;
            this->invoice->acciseApplied = tariff.taxes.accise;

            this->subscriptionPriceBox->setValue(pt.subscriptionPriceHT);

            if (this->invoice->isHPHC) {
                this->hpKwhPriceBox->setValue(pt.hpKwhPriceHT);
                this->hcKwhPriceBox->setValue(pt.hcKwhPriceHT);
            } else {
                this->kwhPriceBox->setValue(pt.baseKwhPriceHT);
            }

            this->taxesInfoLabel->setText(QString("CTA: %1€ - Accise: %2€/kWh").arg(tariff.taxes.cta).arg(tariff.taxes.accise, 0, 'f', 4));
        }
    }
    this->calculateTotal();
}

void InvoiceForm::calculateTotal() {
    if (this->invoice->isHPHC) {
        this->invoice->initialIndexHP = this->initialIndexHPBox->value();
        this->invoice->currentIndexHP = this->currentIndexHPBox->value();
        this->invoice->initialIndexHC = this->initialIndexHCBox->value();
        this->invoice->currentIndexHC = this->currentIndexHCBox->value();
    } else {
        this->invoice->initialIndexBase = this->initialIndexBox->value();
        this->invoice->currentIndexBase = this->currentIndexBox->value();
    }

    int consoTotale = this->invoice->getTotalConsumption();
    double totalTTC = this->invoice->getTotalTTC();

    this->consumptionLabel->setText(QString("Consommation : %1 kWh").arg(consoTotale));
    this->totalLabel->setText(QString("Total TTC : %1 €").arg(totalTTC, 0, 'f', 2));
}