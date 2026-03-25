#ifndef MANAGETARIFFSDIALOG_HPP
#define MANAGETARIFFSDIALOG_HPP

#include "../../Models/Offer.hpp"
#include <QDialog>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>

class ManageTariffsDialog : public QDialog {
    EnergyOffer * const offer;
    QVBoxLayout * const layout;

    QHBoxLayout * const topLayout;
    QComboBox * const kvaFilterCombo;

    QTableWidget * const table;
    QHBoxLayout * const btnLayout;
    QPushButton * const btnAdd, * const btnDel;
public:
    ManageTariffsDialog(EnergyOffer* targetOffer, QWidget* parent = nullptr);
    void refresh();
private slots:
    void addTariff();
    void deleteTariff();
};

#endif // MANAGETARIFFSDIALOG_HPP