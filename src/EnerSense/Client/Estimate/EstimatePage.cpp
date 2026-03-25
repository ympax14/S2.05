#include "EstimatePage.hpp"

#include "Invoice/InvoiceForm.hpp"
#include "../EnerSenseClientWindow.hpp"
#include "../../../utils/DataManager.hpp"

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
    // Valeurs par défaut
    QString clientName = "Client Inconnu";
    QString contractName = "Aucun Contrat";
    int kva = 6;
    bool isHPHC = false;

    // On récupère les données de l'utilisateur connecté
    User* currentUser = DataManager::getInstance().getCurrentUser();
    if (currentUser) {
        clientName = currentUser->fullName;

        if (currentUser->contract != nullptr) {
            EnergyOffer* offer = currentUser->contract->linkedOffer;
            if (offer != nullptr) {
                contractName = QString("%1 - %2").arg(offer->providerName, offer->offerName);
            }

            // On récupère la puissance et l'option tarifaire
            kva = currentUser->contract->powerKVA;
            isHPHC = currentUser->contract->isHPHC;
        }
    }

    // On crée la facture
    Invoice* invoice = new Invoice(clientName, contractName, kva, isHPHC);

    EnerSenseClientWindow::getInvoices().push_back(invoice);

    QMdiSubWindow *subWindow = this->mdiArea->addSubWindow(new InvoiceForm(invoice, this->mdiArea));
    this->mdiArea->setActiveSubWindow(subWindow);
    subWindow->show();
}