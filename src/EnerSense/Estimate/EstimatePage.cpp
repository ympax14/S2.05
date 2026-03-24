#include "EstimatePage.hpp"

#include "Invoice/InvoiceForm.hpp"
#include "../EnerSenseWindow.hpp"
#include "../../utils/DataManager.hpp"

EstimatePage::EstimatePage(QWidget* const &parent) :
    QWidget{parent},

    layout(new QVBoxLayout(this)),
    mdiArea(new QMdiArea(this))
{
    this->mdiArea->setViewMode(QMdiArea::TabbedView);

    this->layout->addWidget(this->mdiArea);
    this->setLayout(this->layout);
}

void EstimatePage::newBill() {
    // On récupère le nom de l'utilisateur connecté
    QString clientName = "Client Inconnu";
    User* currentUser = DataManager::getInstance().getCurrentUser();
    if (currentUser) {
        clientName = currentUser->fullName;
    }

    // On crée la facture
    Invoice* invoice = new Invoice(clientName);

    EnerSenseWindow::getInvoices().push_back(invoice);

    QMdiSubWindow *subWindow = this->mdiArea->addSubWindow(new InvoiceForm(invoice, this->mdiArea));
    this->mdiArea->setActiveSubWindow(subWindow);
    subWindow->show();
}
