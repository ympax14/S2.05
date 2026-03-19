#include "BillsPage.hpp"

#include "Invoice/InvoiceForm.hpp"
#include "../EnerSenseWindow.hpp"

BillsPage::BillsPage(QWidget* const &parent) :
    QWidget{parent},

    layout(new QVBoxLayout(this)),
    mdiArea(new QMdiArea(this))
{
    this->mdiArea->setViewMode(QMdiArea::TabbedView);

    this->layout->addWidget(this->mdiArea);
    this->setLayout(this->layout);
}

void BillsPage::newBill() {
    Invoice* invoice = new Invoice("Inconnu", "Nouveau");
    EnerSenseWindow::getInvoices().push_back(invoice);

    QMdiSubWindow *subWindow = this->mdiArea->addSubWindow(new InvoiceForm(invoice, this->mdiArea));
    this->mdiArea->setActiveSubWindow(subWindow);
    subWindow->show();
}
