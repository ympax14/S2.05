#include "EnerSenseWindow.hpp"
#include "../utils/StylesHelper.hpp"

EnerSenseWindow::EnerSenseWindow(QWidget* const &parent, QApplication* const &app) :
    QMainWindow{parent},

    fileMenu(this->menuBar()->addMenu("&Fichier")),
    newBillAction(new QAction(QIcon(":/assets/menubar/new_bill.png"), tr("&Nouvelle facture"), this)),
    addBillAction(new QAction(QIcon(":/assets/menubar/open_bill.png"), tr("&Ajouter une facture..."), this)),
    quitAction(new QAction(QIcon(":/assets/menubar/quit.png"), tr("&Quitter"), this)),

    tabWidget(new QTabWidget(this)),

    billsPage(new QWidget(this->tabWidget)),
    billsPageLayout(new QVBoxLayout(this->billsPage)),
    billsArea(new QMdiArea(this->billsPage)),

    estimatePage(new QWidget(this->tabWidget)),

    offersComparatorPage(new QWidget(this->tabWidget))
{
    this->setupWindow();
    StylesHelper::loadStyle(this, ":/assets/styles/enersense.qss");

    this->setupMenuBar();
    this->setupTabs();

    this->connectButtons(app);
}

void EnerSenseWindow::setupWindow() {
    // Titre de la Fenêtre Principale.
    this->setWindowTitle("EnerSense | Votre Application de Suivi Énergétique");

    // Modification de la taille de la Fenêtre Principale et taille fixe.
    this->setFixedSize(EnerSenseWindow::WINDOW_WIDTH, EnerSenseWindow::WINDOW_HEIGHT);
}

void EnerSenseWindow::setupMenuBar() {
    // Création et ajout de l'option "Nouvelle facture" dans le menu "Fichier"
    this->newBillAction->setShortcut(QKeySequence::New);
    this->newBillAction->setStatusTip(tr("Créer une nouvelle facture"));
    this->fileMenu->addAction(this->newBillAction);

    // Création et ajout de l'option "Ajouter une facture" dans le menu "Fichier"
    this->addBillAction->setShortcut(QKeySequence::Open);
    this->addBillAction->setStatusTip(tr("Ajouter une facture existante"));
    this->fileMenu->addAction(this->addBillAction);

    // Création et ajout de l'option "Quitter" dans le menu "Fichier"
    this->quitAction->setShortcut(QKeySequence::Quit);
    this->quitAction->setStatusTip(tr("Fermer l'Application"));
    this->fileMenu->addAction(this->quitAction);
}

void EnerSenseWindow::setupTabs() {
    // Page "Factures"
    this->setupBillsPage();

    // Page "Estimations"
    this->setupEstimatePage();

    // Page "Comparateur"
    this->setupComparatorPage();

    // Assignation du QTabWidget comme Widget central
    this->setCentralWidget(this->tabWidget);
}

void EnerSenseWindow::setupBillsPage() {
    this->billsPageLayout->addWidget(this->billsArea);

    this->billsPage->setLayout(this->billsPageLayout);

    this->tabWidget->addTab(this->billsPage, "Factures");
}

void EnerSenseWindow::setupEstimatePage() {
    this->tabWidget->addTab(this->estimatePage, "Estimations");
}

void EnerSenseWindow::setupComparatorPage() {
    this->tabWidget->addTab(this->offersComparatorPage, "Comparateur");
}

void EnerSenseWindow::connectButtons(QApplication* const &app) {
    QObject::connect(this->quitAction, &QAction::triggered, app, &QApplication::quit);
}