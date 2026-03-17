#include "LoginDialog.hpp"

LoginDialog::LoginDialog() :
    QDialog(nullptr),
    mainLayout(new QVBoxLayout(this)), titleLabel(new QLabel("Connexion", this)), titleFont(this->titleLabel->font()),
    formLayout(new QFormLayout()), emailInput(new QLineEdit(this)), passwordInput(new QLineEdit(this)),
    errorLabel(new QLabel("", this)),
    buttonLayout(new QHBoxLayout()), loginBtn(new QPushButton("Se connecter", this)),
    success(false) {

    // Fenêtre
    this->setWindowTitle("EnerSense | Connexion");
    this->setFixedSize(400, 250); // Taille fixe

    // Layout principal vertical
    this->mainLayout->setContentsMargins(20, 20, 20, 20); // marges
    this->mainLayout->setSpacing(15);

    // Label titre
    this->titleLabel->setAlignment(Qt::AlignCenter);
    this->titleFont.setPointSize(16);
    this->titleFont.setBold(true);
    this->titleLabel->setFont(this->titleFont);
    this->mainLayout->addWidget(this->titleLabel);

    // Formulaire Email / Mot de passe
    this->formLayout->setSpacing(10);

    this->emailInput->setPlaceholderText("Entrez votre email ou identifiant");
    this->formLayout->addRow("Email / ID :", this->emailInput);

    this->passwordInput->setEchoMode(QLineEdit::Password);
    this->passwordInput->setPlaceholderText("Mot de passe");
    this->formLayout->addRow("Mot de passe :", this->passwordInput);

    this->mainLayout->addLayout(this->formLayout);

    // Message d'erreur
    this->errorLabel->setStyleSheet("color: red;");
    this->errorLabel->setAlignment(Qt::AlignCenter);
    this->mainLayout->addWidget(errorLabel);

    // Bouton connexion centré
    this->buttonLayout->addStretch();
    this->loginBtn->setFixedHeight(25);
    this->loginBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #007ACC;"
        "  color: white;"
        "  border-radius: 5px;"
        "  padding-left: 5px;"
        "  padding-right: 5px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #005F99;"
        "}"
    );
    this->buttonLayout->addWidget(this->loginBtn);
    this->buttonLayout->addStretch();
    this->mainLayout->addLayout(this->buttonLayout);

    // Connecter le bouton
    QObject::connect(this->loginBtn, SIGNAL(clicked(bool)), this, SLOT(tryLogin()));
}

void LoginDialog::tryLogin() {
    QString emailText = emailInput->text();
    QString passwordText = passwordInput->text();

    /*
     *  Exemple de validation "simple", normalement il faudrait envoyer une requête
     *  à une API (REST par exemple) pour valider les informations et ensuite obtenir une session valide
     */

    if(emailText.isEmpty() || passwordText.isEmpty()) {
        errorLabel->setText("Veuillez remplir tous les champs !");
        return;
    }

    if(emailText == "user@example.com" && passwordText == "1234") {
        success = true;
        accept(); // Ferme le QDialog
    } else {
        errorLabel->setText("Email ou mot de passe incorrect !");
    }
}