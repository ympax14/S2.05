#include "EnerSenseClientWindow.hpp"
#include "../../utils/StylesHelper.hpp"

std::list<Invoice*> EnerSenseClientWindow::invoices;

EnerSenseClientWindow::EnerSenseClientWindow(QWidget* const &parent, QApplication* const &app) :
    QMainWindow{parent},

    fileMenu(this->menuBar()->addMenu("&Fichier")),
    newEstimationAction(new QAction(QIcon(":/assets/menubar/new_bill.png"), tr("&Nouvelle estimation"), this)),
    quitAction(new QAction(QIcon(":/assets/menubar/quit.png"), tr("&Quitter"), this)),

    tabWidget(new StretchTabWidget(this)),

    myContractPage(new MyContractPage(this->tabWidget)),
    estimatePage(new EstimatePage(this->tabWidget)),
    offersComparatorPage(new OffersComparatorPage(this->tabWidget))
{
    StylesHelper::loadStyle(this, ":/assets/styles/enersense.qss");

    this->setupWindow();

    this->setupMenuBar();
    this->setupTabs();

    this->connectButtons(app);
}

EnerSenseClientWindow::~EnerSenseClientWindow() {
    for (Invoice* invoice : this->getInvoices())
        delete invoice;
}

void EnerSenseClientWindow::setupWindow() {
    // Titre de la Fenêtre Principale.
    this->setWindowTitle("EnerSense | Votre Application de Suivi Énergétique");

    // Modification de la taille de la Fenêtre Principale et taille minimum fixe.
    this->setMinimumSize(EnerSenseClientWindow::WINDOW_WIDTH, EnerSenseClientWindow::WINDOW_HEIGHT);
}

void EnerSenseClientWindow::setupMenuBar() {
    // Création et ajout de l'option "Nouvelle estimation" dans le menu "Fichier"
    this->newEstimationAction->setShortcut(QKeySequence::New);
    this->newEstimationAction->setStatusTip(tr("Créer une nouvelle facture"));
    this->fileMenu->addAction(this->newEstimationAction);

    // Création et ajout de l'option "Quitter" dans le menu "Fichier"
    this->quitAction->setShortcut(QKeySequence::Quit);
    this->quitAction->setStatusTip(tr("Fermer l'Application"));
    this->fileMenu->addAction(this->quitAction);
}

void EnerSenseClientWindow::setupTabs() {
    // Page "Mon Contrat"
    this->setupMyContractPage();

    // Page "Estimations"
    this->setupEstimatePage();

    // Page "Comparateur"
    this->setupComparatorPage();

    // Assignation du QTabWidget comme Widget central
    this->setCentralWidget(this->tabWidget);
}

void EnerSenseClientWindow::setupMyContractPage() {
    this->tabWidget->addTab(this->myContractPage, "Mon Contrat");
}

void EnerSenseClientWindow::setupEstimatePage() {
    this->tabWidget->addTab(this->estimatePage, "Estimations");
}

void EnerSenseClientWindow::setupComparatorPage() {
    this->tabWidget->addTab(this->offersComparatorPage, "Comparateur");
}

void EnerSenseClientWindow::connectButtons(QApplication* const &app) {
    QObject::connect(this->newEstimationAction, &QAction::triggered, this, [this]() {
        this->tabWidget->setCurrentWidget(this->estimatePage);
        this->estimatePage->newBill();
    });

    QObject::connect(this->quitAction, &QAction::triggered, app, &QApplication::quit);
}