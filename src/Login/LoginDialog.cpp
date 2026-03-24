#include "LoginDialog.hpp"

#include "../utils/StylesHelper.hpp"
#include "../utils/DataManager.hpp"

LoginDialog::LoginDialog() :
    QDialog(nullptr),

    mainLayout(new QVBoxLayout(this)),
    titleLabel(new QLabel("Connexion", this)),

    formLayout(new QFormLayout),
    emailInput(new QLineEdit(this)),
    passwordInput(new QLineEdit(this)),

    errorTimer(new QTimer(this)),
    errorLabel(new QLabel("", this)),

    buttonLayout(new QHBoxLayout),
    loginBtn(new QPushButton("Se connecter", this)),
    quitBtn(new QPushButton("Quitter", this)),

    success(false)
{
    StylesHelper::loadStyle(this, ":/assets/styles/login.qss");

    // Fenêtre
    this->setupWindow();

    // Layout principal
    this->setupMainLayout();

    // Connecter les boutons
    this->connectButtons();
}

void LoginDialog::setupWindow() {
    this->setWindowTitle("EnerSense | Connexion");
    this->setFixedSize(LoginDialog::WINDOW_WIDTH, LoginDialog::WINDOW_HEIGHT); // Taille fixe
}

void LoginDialog::setupMainLayout() {
    // Layout principal vertical
    this->mainLayout->setContentsMargins(20, 20, 20, 20); // marges
    this->mainLayout->setSpacing(15);

    // Label titre
    QFont titleFont = this->titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);

    this->titleLabel->setFont(titleFont);
    this->titleLabel->setAlignment(Qt::AlignCenter);
    this->mainLayout->addWidget(this->titleLabel);

    // Formulaire Email / Mot de passe
    this->setupFormLayout();
    this->mainLayout->addLayout(this->formLayout);

    // Message d'erreur
    this->setupErrors();
    this->mainLayout->addWidget(errorLabel);

    // Bouton connexion centré
    this->setupButtonsLayout();
    this->mainLayout->addLayout(this->buttonLayout);
}

void LoginDialog::setupFormLayout() {
    this->formLayout->setSpacing(10);

    this->emailInput->setPlaceholderText("Entrez votre email ou identifiant");
    this->formLayout->addRow("Email / ID :", this->emailInput);

    this->passwordInput->setEchoMode(QLineEdit::Password);
    this->passwordInput->setPlaceholderText("Mot de passe");
    this->formLayout->addRow("Mot de passe :", this->passwordInput);
}

void LoginDialog::setupErrors() {
    this->errorTimer->setSingleShot(true);

    // Nettoyer et cacher le QLabel relatif aux erreurs
    QObject::connect(errorTimer, &QTimer::timeout, this, [this]() {
        errorLabel->hide();
        errorLabel->clear();
    });

    this->errorLabel->hide();
    this->errorLabel->setStyleSheet("color: red;");
    this->errorLabel->setAlignment(Qt::AlignCenter);
}

void LoginDialog::setupButtonsLayout() {
    this->buttonLayout->addStretch();

    this->loginBtn->setFixedSize(100, 25);
    this->loginBtn->setObjectName("loginBtn"); // Donne un ID au bouton afin d'y assigner un style (voir /assets/styles/style.qss)
    this->buttonLayout->addWidget(this->loginBtn);

    this->quitBtn->setFixedSize(100, 25);
    this->quitBtn->setObjectName("quitBtn");
    this->buttonLayout->addWidget(this->quitBtn);

    this->buttonLayout->addStretch();
}

void LoginDialog::connectButtons() {
    QObject::connect(this->loginBtn, SIGNAL(clicked(bool)), this, SLOT(tryLogin()));
    QObject::connect(this->quitBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void LoginDialog::showError(const QString &message) {
    this->errorLabel->setText(message);
    this->errorLabel->show();

    // Réinitialisation du timer
    this->errorTimer->stop();
    this->errorTimer->start(3000);
}

void LoginDialog::tryLogin() {
    QString emailText = emailInput->text();
    QString passwordText = passwordInput->text();

    if(emailText.isEmpty() || passwordText.isEmpty()) {
        this->showError("Veuillez remplir tous les champs !");
        return;
    }

    // Utilisation du DataManager pour vérifier les identifiants
    if(DataManager::getInstance().authenticate(emailText, passwordText)) {
        success = true;
        accept(); // Ferme le QDialog
    } else {
        this->showError("Email ou mot de passe incorrect !");
    }
}