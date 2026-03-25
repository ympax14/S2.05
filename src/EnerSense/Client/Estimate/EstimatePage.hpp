#ifndef BILLSPAGE_H
#define BILLSPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QMdiArea>
#include <QMdiSubWindow>

class EstimatePage : public QWidget
{
    Q_OBJECT

private:
    QVBoxLayout * const layout;
    QMdiArea * const mdiArea;
public:
    explicit EstimatePage(QWidget* const &parent = nullptr);

public slots:
    void newBill();
signals:
};

#endif // BILLSPAGE_H
