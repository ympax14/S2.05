#include "InvoiceForm.hpp"

#include "Components/InvoiceFormAddFieldDialog.hpp"

#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>

InvoiceForm::InvoiceForm(Invoice* const &_invoice, QWidget* const &parent) :
    QWidget(parent),
    invoice(_invoice),

    mainLayout(new QVBoxLayout(this)),
    totalLabel(new QLabel(this)),
    calculateBtn(new QPushButton("Calculer", this)),

    formLayout(new QFormLayout),
    clientNameEdit(new QLineEdit(this)),
    itemNameEdit(new QLineEdit(this)),
    addFieldBtn(new QPushButton("Ajouter un champ", this))
{
    /********** Layout formulaire **********/
    this->updateWindowName();

    // Création d'un widget container pour le formLayout, cela permettra de scroll
    QWidget* formContainer = new QWidget(this);
    formContainer->setLayout(formLayout);

    // Mettre le container dans une QScrollArea
    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);  // important pour que le scroll s’adapte
    scrollArea->setWidget(formContainer);

    // Informations
    this->detailsGroup = new QGroupBox("Informations");
    this->detailsGroupLayout = new QFormLayout(this->detailsGroup);

    this->detailsGroupLayout->addRow("Client :", this->clientNameEdit);
    QObject::connect(this->clientNameEdit, &QLineEdit::textEdited, [this](QString val){
        this->invoice->clientName = val;
        this->updateWindowName();
    });

    this->detailsGroupLayout->addRow("Intitulé :", this->itemNameEdit);
    QObject::connect(this->itemNameEdit, &QLineEdit::textEdited, [this](QString val){
        this->invoice->billName = val;
        this->updateWindowName();
    });

    this->formLayout->addRow(this->detailsGroup);

    // Champs
    const std::map<std::string, InvoiceField*>& fields = this->invoice->getFields();
    for (std::map<std::string, InvoiceField*>::const_iterator it = fields.cbegin(); it != fields.cend(); it++) {
        InvoiceField* field = it->second;
        InvoiceFormField* group = new InvoiceFormField(field);

        if (this->fields.find(it->first) != this->fields.end())
            throw std::invalid_argument("La clé est déjà présente !");
        else this->fields[it->first] = group;

        this->formLayout->addRow(group);
    }

    this->addFieldBtn->resize(200, 50);
    this->formLayout->addWidget(this->addFieldBtn);

    this->mainLayout->addWidget(scrollArea);

    this->totalLabel->hide();
    this->mainLayout->addWidget(this->totalLabel);

    this->mainLayout->addWidget(this->calculateBtn);

    this->connectBtns();
}

void InvoiceForm::connectBtns() {
    QObject::connect(this->calculateBtn, &QPushButton::clicked, this, &InvoiceForm::calculateTotal);
    QObject::connect(this->addFieldBtn, &QPushButton::clicked, this, &InvoiceForm::addField);
}

void InvoiceForm::updateWindowName() {
    this->setWindowTitle('[' + this->invoice->clientName + "] " + this->invoice->billName);
}

void InvoiceForm::calculateTotal()
{
    QString clientName = this->clientNameEdit->text();
    QString billName = this->itemNameEdit->text();

    double price = 0.0;
    const std::map<std::string, InvoiceField*>& fields = this->invoice->getFields();
    for (std::map<std::string, InvoiceField*>::const_iterator it = fields.cbegin(); it != fields.cend(); it++) {
        InvoiceField* field = it->second;
        price += field->getConstant() + field->getPrice() * field->getQuantity();
    }

    QString labelText = QString(R"(
    Client : %1
    Facture : %2
    Total : %3€
    )").arg(clientName).arg(billName).arg(price, 0, 'f', 2);

    this->totalLabel->setText(labelText);
    this->totalLabel->show();
}

bool InvoiceForm::containsField(const std::string& key) {
    return this->fields.find(key) != this->fields.end();
}

void InvoiceForm::addField() {
    InvoiceFormAddFieldDialog dialog(this);

    int result = dialog.exec();

    if (result == QDialog::Accepted) {
        std::string key = dialog.getName().toStdString();
        if (this->containsField(key))
            throw std::invalid_argument("La clé est déjà présente !");

        InvoiceField* field = this->invoice->addField(key, dialog.getName(), dialog.isFixed());
        InvoiceFormField* group = new InvoiceFormField(field);

        this->fields[key] = group;
        this->formLayout->addRow(group);
    }
}