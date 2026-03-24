#ifndef OFFERSMANAGEMENTPAGE_HPP
#define OFFERSMANAGEMENTPAGE_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>

class OffersManagementPage : public QWidget {
    Q_OBJECT
public:
    explicit OffersManagementPage(QWidget* const &parent = nullptr);

private slots:
    void refreshTable();
    void createOffer();
    void editOffer();
    void deleteOffer();

private:
    QVBoxLayout* mainLayout;
    QTableWidget* offersTable;

    QHBoxLayout * btnLayout;
    QPushButton * btnCreate, * btnEdit, * btnManageTariffs, * btnDelete;

    void setupUI();

public slots:
    void manageTariffs();
};

#endif // OFFERSMANAGEMENTPAGE_HPP