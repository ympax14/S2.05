#include "InvoiceFormAddFieldDialog.hpp"

InvoiceFormAddFieldDialog::InvoiceFormAddFieldDialog(InvoiceForm* const &_parentForm) :
    QDialog{nullptr},
    parentForm(_parentForm),

    mainLayout(new QVBoxLayout(this)),

    formLayout(new QFormLayout),
    fieldName(new QLineEdit(this)),
    fieldFixed(new QCheckBox(this)),

    btnLayout(new QHBoxLayout),
    confirmBtn(new QPushButton("Confirmer", this)),
    closeBtn(new QPushButton("Fermer", this))
{
    this->setWindowTitle("Ajouter un champ");
    //this->setFixedSize(400, 200);

    // Layout formulaire
    this->fieldName->setPlaceholderText("Nom du champ de la facture");
    this->formLayout->addRow("Nom", this->fieldName);

    this->fieldFixed->setCheckState(Qt::Checked);
    this->formLayout->addRow("Coût Fixe?", this->fieldFixed);

    this->mainLayout->addLayout(this->formLayout);

    // Layout Boutons
    btnLayout->addStretch();
    btnLayout->addWidget(this->confirmBtn);
    btnLayout->addWidget(this->closeBtn);
    btnLayout->addStretch();

    this->mainLayout->addLayout(btnLayout);

    this->connectButtons();
}

void InvoiceFormAddFieldDialog::connectButtons() {
    QObject::connect(this->closeBtn, &QPushButton::clicked, this, &QDialog::reject);
    QObject::connect(this->confirmBtn, &QPushButton::clicked, this, &InvoiceFormAddFieldDialog::confirm);
}

void InvoiceFormAddFieldDialog::confirm() {
    QString key = this->fieldName->text();
    if (key.isEmpty()) {
        // Erreur nom du champ vide
        return;
    }

    if (this->parentForm->containsField(key.toStdString())) {
        // Erreur nom du champ existe
        return;
    }

    this->accept();
}