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

private slots:
    void addNewTariff();
    void removeTariff(const QDate& date);

private:
    QVBoxLayout* mainLayout;

    // Informations du contrat
    QGroupBox* infoGroup;
    QFormLayout* infoLayout;
    QLabel* providerLabel;
    QLabel* energyTypeLabel;

    // Historique des tarifs
    QGroupBox* historyGroup;
    QVBoxLayout* historyLayout;
    QTableWidget* tariffTable;

    // Mises à jour (Nouveaux tarifs)
    QGroupBox* updateGroup;
    QFormLayout* updateLayout;
    QDateEdit* newDateEdit;
    QDoubleSpinBox* newSubPriceBox;
    QDoubleSpinBox* newKwhPriceBox;
    QPushButton* addTariffBtn;

    void setupUI();
    void connectSignals();
    void refreshTable();
};

#endif // MYCONTRACTPAGE_HPP