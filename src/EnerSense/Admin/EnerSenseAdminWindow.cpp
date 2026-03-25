#include "EnerSenseAdminWindow.hpp"

#include "../../utils/StylesHelper.hpp"

#include <QIcon>

EnerSenseAdminWindow::EnerSenseAdminWindow(QWidget* const &parent, QApplication* const &app) :
    QMainWindow{parent},

    fileMenu(this->menuBar()->addMenu("&Fichier")),
    quitAction(new QAction(QIcon(":/assets/menubar/quit.png"), tr("&Quitter"), this)),

    tabWidget(new StretchTabWidget(this)),
    usersPage(new UsersManagementPage(this->tabWidget)),
    offersPage(new OffersManagementPage(this->tabWidget))
{
    StylesHelper::loadStyle(this, ":/assets/styles/enersense.qss");

    this->setupWindow();
    this->setupMenuBar();
    this->setupTabs();

    QObject::connect(this->quitAction, &QAction::triggered, app, &QApplication::quit);
}

void EnerSenseAdminWindow::setupWindow() {
    this->setWindowTitle("EnerSense Administration | Tableau de bord");
    this->setMinimumSize(1000, 600);
}

void EnerSenseAdminWindow::setupMenuBar() {
    this->quitAction->setShortcut(QKeySequence::Quit);
    this->fileMenu->addAction(this->quitAction);
}

void EnerSenseAdminWindow::setupTabs() {
    this->tabWidget->addTab(this->usersPage, "Comptes Utilisateurs");
    this->tabWidget->addTab(this->offersPage, "Gestion des Offres");
    this->setCentralWidget(this->tabWidget);
}