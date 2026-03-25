#include "UsersManagementPage.hpp"
#include "../../../utils/DataManager.hpp"
#include "AssignOfferDialog.hpp"
#include "CreateUserDialog.hpp"
#include <QHeaderView>
#include <QComboBox>

UsersManagementPage::UsersManagementPage(QWidget* const &parent) :
    QWidget(parent),

    mainLayout(new QVBoxLayout(this)),
    usersTable(new QTableWidget(this)),
    btnLayout(new QHBoxLayout()),
    btnCreate(new QPushButton("Créer un compte", this)),
    btnDelete(new QPushButton("Supprimer", this)),
    btnResetPwd(new QPushButton("Réinitialiser MDP", this)),
    btnAssignOffer(new QPushButton("Assigner Offre", this)),
    btnCancelContract(new QPushButton("Résilier Contrat", this))
{
    this->setupUI();

    QObject::connect(this->btnCreate, &QPushButton::clicked, this, &UsersManagementPage::createUser);
    QObject::connect(this->btnDelete, &QPushButton::clicked, this, &UsersManagementPage::deleteUser);
    QObject::connect(this->btnResetPwd, &QPushButton::clicked, this, &UsersManagementPage::resetPassword);
    QObject::connect(this->btnAssignOffer, &QPushButton::clicked, this, &UsersManagementPage::assignOffer);
    QObject::connect(this->btnCancelContract, &QPushButton::clicked, this, &UsersManagementPage::cancelContract);

    this->refreshTable();
}

void UsersManagementPage::setupUI() {
    this->usersTable->setColumnCount(4);
    this->usersTable->setHorizontalHeaderLabels({"ID", "Nom", "Rôle", "Statut Contrat"});
    this->usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->usersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    this->usersTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    this->btnDelete->setObjectName("usersManagementBtnDelete");

    this->btnLayout->addWidget(this->btnCreate);
    this->btnLayout->addWidget(this->btnResetPwd);
    this->btnLayout->addWidget(this->btnAssignOffer);
    this->btnLayout->addWidget(this->btnCancelContract);
    this->btnLayout->addStretch();
    this->btnLayout->addWidget(this->btnDelete);

    this->mainLayout->addWidget(this->usersTable);
    this->mainLayout->addLayout(this->btnLayout);
}

void UsersManagementPage::refreshTable() {
    std::vector<User*>& users = DataManager::getInstance().getUsers();
    this->usersTable->setRowCount(0);

    for (size_t i = 0; i < users.size(); ++i) {
        this->usersTable->insertRow(i);
        User* user = users[i];

        this->usersTable->setItem(i, 0, new QTableWidgetItem(user->id));
        this->usersTable->setItem(i, 1, new QTableWidgetItem(user->fullName));

        QTableWidgetItem* roleItem = new QTableWidgetItem(user->isAdmin ? "Administrateur" : "Client");
        roleItem->setForeground(user->isAdmin ? QBrush(QColorConstants::Svg::darkgoldenrod) : QBrush(QColorConstants::Svg::royalblue));
        this->usersTable->setItem(i, 2, roleItem);

        QString contractStr = "Aucun contrat";
        if (user->contract != nullptr) {
            QString opt = user->contract->isHPHC ? "HP/HC" : "Base";
            contractStr = QString("%1 - %2 (%3 kVA %4)").arg(user->contract->linkedOffer->providerName).arg(user->contract->linkedOffer->offerName).arg(user->contract->powerKVA).arg(opt);
        }
        this->usersTable->setItem(i, 3, new QTableWidgetItem(contractStr));
    }
}

void UsersManagementPage::createUser() {
    CreateUserDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        if (dialog.getIdEdit()->text().isEmpty() || dialog.getPwdEdit()->text().isEmpty()) {
            QMessageBox::warning(this, "Erreur", "L'ID et le mot de passe sont obligatoires.");
            return;
        }
        User* newUser = new User(dialog.getIdEdit()->text(), dialog.getPwdEdit()->text(), dialog.getNameEdit()->text(), dialog.getAdminCheck()->isChecked(), nullptr);
        DataManager::getInstance().addUser(newUser);
        this->refreshTable();
    }
}

void UsersManagementPage::deleteUser() {
    int row = this->usersTable->currentRow();
    if (row < 0) return;

    std::vector<User*>& users = DataManager::getInstance().getUsers();
    User* user = users[row];

    if (user->id == DataManager::getInstance().getCurrentUser()->id) {
        QMessageBox::warning(this, "Erreur", "Vous ne pouvez pas supprimer votre propre compte !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer le compte de " + user->fullName + " ?") == QMessageBox::Yes) {
        delete user;
        users.erase(users.begin() + row);
        DataManager::getInstance().saveData();
        this->refreshTable();
    }
}

void UsersManagementPage::resetPassword() {
    int row = this->usersTable->currentRow();
    if (row < 0) return;

    bool ok;
    QString newPwd = QInputDialog::getText(this, "Réinitialiser MDP", "Nouveau mot de passe :", QLineEdit::Normal, "", &ok);

    if (ok && !newPwd.isEmpty()) {
        DataManager::getInstance().getUsers()[row]->password = newPwd;
        DataManager::getInstance().saveData();
        QMessageBox::information(this, "Succès", "Mot de passe modifié !");
    }
}

void UsersManagementPage::assignOffer() {
    int row = this->usersTable->currentRow();
    if (row < 0) return;

    User* user = DataManager::getInstance().getUsers()[row];
    if (user->isAdmin) {
        QMessageBox::warning(this, "Erreur", "Un administrateur n'a pas besoin de contrat.");
        return;
    }

    if (DataManager::getInstance().getOffers().empty()) {
        QMessageBox::warning(this, "Erreur", "Aucune offre disponible. Créez-en une d'abord.");
        return;
    }

    AssignOfferDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int offerIndex = dialog.getOfferCombo()->currentIndex();
        EnergyOffer* offer = DataManager::getInstance().getOffers()[offerIndex];

        if (user->contract) delete user->contract;

        // On crée le contrat avec la puissance et l'option choisies
        user->contract = new Contract(offer, dialog.getKva(), dialog.isHphc());

        DataManager::getInstance().saveData();
        this->refreshTable();
        QMessageBox::information(this, "Succès", "Le contrat a été assigné !");
    }
}

void UsersManagementPage::cancelContract() {
    int row = this->usersTable->currentRow();
    if (row < 0) return;

    User* user = DataManager::getInstance().getUsers()[row];
    if (user->contract) {
        if (QMessageBox::question(this, "Résilier", "Résilier le contrat de " + user->fullName + " ?") == QMessageBox::Yes) {
            delete user->contract;
            user->contract = nullptr;
            DataManager::getInstance().saveData();
            this->refreshTable();
        }
    }
}