#ifndef BILLSPAGE_H
#define BILLSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QMdiArea>
#include <QMdiSubWindow>

class BillsPage : public QWidget
{
    Q_OBJECT

private:
    QVBoxLayout *layout;
    QMdiArea *mdiArea;
public:
    explicit BillsPage(QWidget* const &parent = nullptr);

public slots:
    void newBill();
signals:
};

#endif // BILLSPAGE_H
