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

    // En-tête
    detailsGroup(new QGroupBox("Informations Générales", this->documentContainer)),
    detailsGroupLayout(new QFormLayout(this->detailsGroup)),
    clientNameLabel(new QLabel(this->detailsGroup)),
    billingDateEdit(new QDateEdit(this->detailsGroup)),

    // Relevés
    meterGroup(new QGroupBox("Relevés Compteur", this->documentContainer)),
    meterGroupLayout(new QFormLayout(this->meterGroup)),
    initialIndexBox(new QSpinBox(this->meterGroup)),
    currentIndexBox(new QSpinBox(this->meterGroup)),

    // Tarifs appliqués
    tariffGroup(new QGroupBox("Tarifs Appliqués", this->documentContainer)),
    tariffGroupLayout(new QFormLayout(this->tariffGroup)),
    subscriptionPriceBox(new QDoubleSpinBox(this->tariffGroup)),
    kwhPriceBox(new QDoubleSpinBox(this->tariffGroup)),

    // Pied de page
    footerLayout(new QHBoxLayout()),
    consumptionLabel(new QLabel("Consommation : 0 kWh", this->documentContainer)),
    totalLabel(new QLabel("Total : 0.00 €", this->documentContainer))
{
    this->setWindowTitle(QString("Facture de %1 (%2)").arg(this->invoice->clientName).arg(this->invoice->billingDate.toString("dd/MM/yyyy")));
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
    this->scrollArea->setStyleSheet("QScrollArea { background-color: #f4f6f9; }");
    this->scrollArea->setWidget(this->scrollAreaWidgetContents);

    this->centeringLayout->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    this->centeringLayout->setContentsMargins(30, 30, 30, 30);

    this->documentContainer->setMinimumWidth(600);
    this->documentContainer->setMaximumWidth(800);
    this->documentContainer->setStyleSheet("QWidget#invoiceDocument { background-color: white; border-radius: 10px; border: 1px solid #e0e0e0; }");
    this->documentContainer->setObjectName("invoiceDocument");
    this->documentLayout->setContentsMargins(40, 40, 40, 40);
    this->documentLayout->setSpacing(20);

    this->centeringLayout->addWidget(this->documentContainer);

    QLabel* docTitle = new QLabel("FACTURE D'ÉNERGIE", this->documentContainer);
    QFont titleFont = docTitle->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleFont.setLetterSpacing(QFont::AbsoluteSpacing, 2.0);
    docTitle->setFont(titleFont);
    docTitle->setStyleSheet("color: #2c3e50;");
    this->documentLayout->addWidget(docTitle);

    QString groupStyle = "QGroupBox { font-weight: bold; border: 1px solid #e0e0e0; border-radius: 5px; margin-top: 10px; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; color: #555; }";

    // En-tête
    this->detailsGroup->setStyleSheet(groupStyle);
    this->clientNameLabel->setText(this->invoice->clientName);
    this->detailsGroupLayout->addRow("Client :", this->clientNameLabel);

    this->billingDateEdit->setDate(this->invoice->billingDate);
    this->billingDateEdit->setCalendarPopup(true);
    this->detailsGroupLayout->addRow("Date de facturation :", this->billingDateEdit);

    this->documentLayout->addWidget(this->detailsGroup);

    // Relevés
    this->meterGroup->setStyleSheet(groupStyle);

    this->initialIndexBox->setMaximum(999999);
    this->initialIndexBox->setSuffix(" kWh");
    this->initialIndexBox->setAlignment(Qt::AlignRight);

    this->currentIndexBox->setMaximum(999999);
    this->currentIndexBox->setSuffix(" kWh");
    this->currentIndexBox->setAlignment(Qt::AlignRight);

    this->meterGroupLayout->addRow("Index Précédent (N-1) :", this->initialIndexBox);
    this->meterGroupLayout->addRow("Index Actuel (N) :", this->currentIndexBox);

    this->documentLayout->addWidget(this->meterGroup);

    // Tarifs
    this->tariffGroup->setStyleSheet(groupStyle);

    // On rend ces champs "Read Only" car ils dépendent du contrat à la date de facturation
    this->subscriptionPriceBox->setReadOnly(true);
    this->subscriptionPriceBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    this->subscriptionPriceBox->setSuffix(" € / mois");
    this->subscriptionPriceBox->setAlignment(Qt::AlignRight);

    this->kwhPriceBox->setReadOnly(true);
    this->kwhPriceBox->setButtonSymbols(QAbstractSpinBox::NoButtons);
    this->kwhPriceBox->setSuffix(" € / kWh");
    this->kwhPriceBox->setDecimals(4); // Souvent 4 décimales pour l'énergie
    this->kwhPriceBox->setAlignment(Qt::AlignRight);

    this->tariffGroupLayout->addRow("Abonnement fixe :", this->subscriptionPriceBox);
    this->tariffGroupLayout->addRow("Prix du kWh :", this->kwhPriceBox);

    this->documentLayout->addWidget(this->tariffGroup);
    this->documentLayout->addStretch(1);

    // Pied de page
    QFrame* separator = new QFrame(this->documentContainer);
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("background-color: #e0e0e0; max-height: 1px;");
    this->documentLayout->addWidget(separator);

    QFont consoFont = this->consumptionLabel->font();
    consoFont.setPointSize(12);
    this->consumptionLabel->setFont(consoFont);
    this->consumptionLabel->setStyleSheet("color: #7f8c8d;");

    QFont totalFont = this->totalLabel->font();
    totalFont.setPointSize(18);
    totalFont.setBold(true);
    this->totalLabel->setFont(totalFont);
    this->totalLabel->setStyleSheet("color: #27ae60;");

    this->footerLayout->addWidget(this->consumptionLabel);
    this->footerLayout->addStretch();
    this->footerLayout->addWidget(this->totalLabel);

    this->documentLayout->addLayout(this->footerLayout);
}

void InvoiceForm::connectSignals() {
    // Quand on change la date, on met à jour les tarifs
    QObject::connect(this->billingDateEdit, &QDateEdit::dateChanged, this, &InvoiceForm::updateTariffsFromDate);

    // Quand les index changent, on recalcule le total
    QObject::connect(this->initialIndexBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &InvoiceForm::calculateTotal);
    QObject::connect(this->currentIndexBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &InvoiceForm::calculateTotal);
}

void InvoiceForm::updateTariffsFromDate() {
    this->invoice->billingDate = this->billingDateEdit->date();
    this->setWindowTitle(QString("Facture de %1 (%2)").arg(this->invoice->clientName).arg(this->invoice->billingDate.toString("dd/MM/yyyy")));

    // On récupère l'utilisateur connecté depuis notre Singleton
    User* currentUser = DataManager::getInstance().getCurrentUser();

    if (currentUser && currentUser->contract) {
        // On récupère le tarif exact à la date sélectionnée
        ContractTariff tariff = currentUser->contract->linkedOffer->getTariffAt(this->invoice->billingDate);

        this->invoice->subscriptionPriceApplied = tariff.subscriptionPrice;
        this->invoice->kwhPriceApplied = tariff.kwhPrice;

        this->subscriptionPriceBox->setValue(tariff.subscriptionPrice);
        this->kwhPriceBox->setValue(tariff.kwhPrice);
    }

    this->calculateTotal();
}

void InvoiceForm::calculateTotal() {
    this->invoice->initialIndex = this->initialIndexBox->value();
    this->invoice->currentIndex = this->currentIndexBox->value();

    int conso = this->invoice->getConsumption();
    double total = this->invoice->getTotal();

    this->consumptionLabel->setText(QString("Consommation : %1 kWh").arg(conso));
    this->totalLabel->setText(QString("Total : %1 €").arg(total, 0, 'f', 2));
}