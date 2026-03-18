#include "EnerSenseWindow.hpp"

EnerSenseWindow::EnerSenseWindow(QWidget *parent, QApplication *app) :
    QMainWindow{parent},
    fileMenu(nullptr), newBillAction(nullptr), addBillAction(nullptr)
{
    // Titre de la Fenêtre Principale.
    this->setWindowTitle("EnerSense | Votre Application de Suivi Énergétique");

    // Modification de la taille de la Fenêtre Principale et taille fixe.
    this->setFixedSize(EnerSenseWindow::WINDOW_WIDTH, EnerSenseWindow::WINDOW_HEIGHT);

    // Création du menu "Fichier" dans la barre de menu
    this->fileMenu = this->menuBar()->addMenu("&Fichier");

    // Création et ajout de l'option "Nouvelle facture" dans le menu "Fichier"
    this->newBillAction = new QAction(QIcon(":/assets/menubar/new_bill.png"), tr("&Nouvelle facture"), this);
    this->newBillAction->setShortcut(QKeySequence::New);
    this->newBillAction->setStatusTip(tr("Créer une nouvelle facture"));
    this->fileMenu->addAction(this->newBillAction);

    // Création et ajout de l'option "Ajouter une facture" dans le menu "Fichier"
    this->addBillAction = new QAction(QIcon(":/assets/menubar/open_bill.png"), tr("&Ajouter une facture..."), this);
    this->addBillAction->setShortcut(QKeySequence::Open);
    this->addBillAction->setStatusTip(tr("Ajouter une facture existante"));
    this->fileMenu->addAction(this->addBillAction);

    this->quitAction = new QAction(QIcon(":/assets/menubar/quit.png"), tr("&Quitter"), this);
    this->quitAction->setShortcut(QKeySequence::Quit);
    this->quitAction->setStatusTip(tr("Fermer l'Application"));
    QObject::connect(this->quitAction, &QAction::triggered, app, &QApplication::quit);
    this->fileMenu->addAction(this->quitAction);

    // Création du Widget Principal (QTabWidget) permettant l'ajout d'onglets
    this->tabWidget = new QTabWidget(this);

    // Page "Factures"
    this->billsPage = new QWidget(this->tabWidget);
    this->billsPageLayout = new QVBoxLayout(this->billsPage);
    this->billsArea = new QMdiArea(this->billsPage);
    this->billsPageLayout->addWidget(this->billsArea);
    this->billsPage->setLayout(this->billsPageLayout);

    this->tabWidget->addTab(this->billsPage, "Factures");

    // Page "Estimations"
    this->estimatePage = new QWidget(this->tabWidget);
    this->tabWidget->addTab(this->estimatePage, "Estimations");

    // Page "Comparateur"
    this->offersComparatorPage = new QWidget(this->tabWidget);
    this->tabWidget->addTab(this->offersComparatorPage, "Comparateur");

    this->setCentralWidget(this->tabWidget);
}