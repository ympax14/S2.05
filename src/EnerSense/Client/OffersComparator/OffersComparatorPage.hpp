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

class OffersComparatorPage : public QWidget
{
    Q_OBJECT
public:
    explicit OffersComparatorPage(QWidget* const &parent = nullptr);

private slots:
    void calculateComparisons();

private:
    QVBoxLayout * const mainLayout;

    QGroupBox * const simulationGroup;
    QHBoxLayout * const simulationLayout;
    QLabel * const instructionLabel;
    QSpinBox * const consumptionBox;
    QLabel * const unitLabel;
    QComboBox * const energyFilterCombo;

    QTableWidget * const resultsTable;

    void setupUI();
    void connectSignals();
};

#endif // OFFERSCOMPARATORPAGE_H