#ifndef ENERSENSEWINDOW_H
#define ENERSENSEWINDOW_H

#include "StretchTabWidget.hpp"

#include "./Bills/BillsPage.hpp"
#include "./Estimate/EstimatePage.hpp"
#include "./OffersComparator/OffersComparatorPage.hpp"

#include "./Bills/Invoice/Invoice.hpp"

#include <list>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QTabWidget>
#include <QMdiArea>
#include <QVBoxLayout>
#include <QApplication>
#include <QTabBar>

class EnerSenseWindow : public QMainWindow {
    Q_OBJECT
private:
    static std::list<Invoice*> invoices;

    QMenu *fileMenu;
    QAction *newBillAction, *addBillAction, *quitAction;

    StretchTabWidget *tabWidget;

    BillsPage *billsPage;

    EstimatePage *estimatePage;

    OffersComparatorPage *offersComparatorPage;

    void setupWindow();
    void setupMenuBar();
    void setupTabs();

    void setupBillsPage();
    void setupEstimatePage();
    void setupComparatorPage();

    void connectButtons(QApplication* const &app);
public:
    static const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

    explicit EnerSenseWindow(QWidget* const &parent = nullptr, QApplication* const &app = nullptr);
    ~EnerSenseWindow();

    inline static std::list<Invoice*>& getInvoices() { return invoices; };
};

#endif // ENERSENSEWINDOW_H
