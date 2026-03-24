#ifndef ENERSENSEADMINWINDOW_HPP
#define ENERSENSEADMINWINDOW_HPP

#include "../StretchTabWidget.hpp"
#include "UsersManagement/UsersManagementPage.hpp"
#include "OffersManagement/OffersManagementPage.hpp"

#include <QMainWindow>
#include <QMenuBar>
#include <QApplication>

class EnerSenseAdminWindow : public QMainWindow {
    Q_OBJECT
private:
    QMenu * const fileMenu;
    QAction * const quitAction;

    StretchTabWidget * const tabWidget;

    UsersManagementPage * const usersPage;
    OffersManagementPage * const offersPage;

    void setupWindow();
    void setupMenuBar();
    void setupTabs();

public:
    explicit EnerSenseAdminWindow(QWidget* const &parent = nullptr, QApplication* const &app = nullptr);
};

#endif // ENERSENSEADMINWINDOW_HPP