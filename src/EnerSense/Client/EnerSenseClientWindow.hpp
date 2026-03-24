#ifndef ENERSENSECLIENTWINDOW_H
#define ENERSENSECLIENTWINDOW_H

#include "../StretchTabWidget.hpp"
#include "./MyContract/MyContractPage.hpp"
#include "./Estimate/EstimatePage.hpp"
#include "./OffersComparator/OffersComparatorPage.hpp"
#include "../Models/Invoice.hpp"

#include <list>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QTabWidget>
#include <QMdiArea>
#include <QVBoxLayout>
#include <QApplication>
#include <QTabBar>

class EnerSenseClientWindow : public QMainWindow {
    Q_OBJECT
private:
    static std::list<Invoice*> invoices;

    QMenu * const fileMenu;
    QAction * const newEstimationAction, * const quitAction;

    StretchTabWidget * const tabWidget;

    MyContractPage * const myContractPage;
    EstimatePage * const estimatePage;
    OffersComparatorPage * const offersComparatorPage;

    void setupWindow();
    void setupMenuBar();
    void setupTabs();

    void setupMyContractPage();
    void setupEstimatePage();
    void setupComparatorPage();

    void connectButtons(QApplication* const &app);
public:
    static const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

    explicit EnerSenseClientWindow(QWidget* const &parent = nullptr, QApplication* const &app = nullptr);
    ~EnerSenseClientWindow();

    inline static std::list<Invoice*>& getInvoices() { return invoices; };
};

#endif // ENERSENSECLIENTWINDOW_H
