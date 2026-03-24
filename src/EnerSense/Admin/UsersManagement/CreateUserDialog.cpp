#include "CreateUserDialog.hpp"

CreateUserDialog::CreateUserDialog(QWidget* parent) :
    QDialog(parent),

    layout(new QFormLayout(this)),

    idEdit(new QLineEdit(this)),
    nameEdit(new QLineEdit(this)),
    pwdEdit(new QLineEdit(this)),
    adminCheck(new QCheckBox("Administrateur", this)),

    btnBox(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this))
{
    this->setWindowTitle("Créer un utilisateur");

    this->layout->addRow("ID / Email :", this->idEdit);
    this->layout->addRow("Nom complet :", this->nameEdit);
    this->layout->addRow("Mot de passe :", this->pwdEdit);
    this->layout->addRow("", this->adminCheck);
    this->layout->addWidget(this->btnBox);

    QObject::connect(this->btnBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    QObject::connect(this->btnBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
