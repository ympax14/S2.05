#ifndef MYCONTRACTPAGE_HPP
#define MYCONTRACTPAGE_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QTableWidget>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QPushButton>

class MyContractPage : public QWidget
{
    Q_OBJECT
public:
    explicit MyContractPage(QWidget* const &parent = nullptr);
    void loadContractData();

private:
    QVBoxLayout * const mainLayout;

    // Informations du contrat
    QGroupBox * const infoGroup;
    QFormLayout * const infoLayout;
    QLabel * const providerLabel, * const offerLabel, * const energyTypeLabel;

    // Historique des tarifs
    QGroupBox * const historyGroup;
    QVBoxLayout * const historyLayout;
    QTableWidget * const tariffTable;

    void setupUI();
    void refreshTable();
};

#endif // MYCONTRACTPAGE_HPP