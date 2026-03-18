#ifndef ENERSENSEWINDOW_H
#define ENERSENSEWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QTabWidget>
#include <QMdiArea>
#include <QVBoxLayout>
#include <QApplication>

class EnerSenseWindow : public QMainWindow {
    Q_OBJECT
private:
    QMenu *fileMenu;
    QAction *newBillAction, *addBillAction, *quitAction;

    QTabWidget *tabWidget;

    QWidget *billsPage;
    QVBoxLayout *billsPageLayout;
    QMdiArea *billsArea;

    QWidget *estimatePage;

    QWidget *offersComparatorPage;
public:
    static const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

    explicit EnerSenseWindow(QWidget *parent = nullptr, QApplication *app = nullptr);

signals:
};

#endif // ENERSENSEWINDOW_H
