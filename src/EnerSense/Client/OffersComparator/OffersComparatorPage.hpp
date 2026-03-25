#ifndef OFFERSCOMPARATORPAGE_H
#define OFFERSCOMPARATORPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QSpinBox>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>

class OffersComparatorPage : public QWidget
{
    Q_OBJECT
public:
    explicit OffersComparatorPage(QWidget* const &parent = nullptr);

private slots:
    void calculateComparisons();
    void updateUIState();

private:
    QVBoxLayout * const mainLayout;

    QGroupBox * const simulationGroup;
    QFormLayout * const simulationLayout;

    QComboBox * const energyFilterCombo, * const kvaFilterCombo;
    QCheckBox * const hphcFilterCheck;

    QSpinBox * const consumptionBaseBox, * const consumptionHPBox, * const consumptionHCBox;

    QTableWidget * const resultsTable;

    void setupUI();
    void connectSignals();
};

#endif // OFFERSCOMPARATORPAGE_H