#ifndef INVOICEFORM_HPP
#define INVOICEFORM_HPP

#include "../../../Models/Invoice.hpp"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QScrollArea>
#include <QGroupBox>
#include <QDateEdit>

class InvoiceForm : public QWidget
{
    Q_OBJECT

public:
    explicit InvoiceForm(Invoice* const &_invoice, QWidget* const &parent = nullptr);

private slots:
    void calculateTotal();
    void updateTariffsFromDate();

private:
    Invoice * const invoice;

    QVBoxLayout * const mainLayout;
    QScrollArea * const scrollArea;

    QWidget * const scrollAreaWidgetContents;
    QHBoxLayout * const centeringLayout;

    QWidget * const documentContainer;
    QVBoxLayout * const documentLayout;

    QGroupBox * const detailsGroup;
    QFormLayout * const detailsGroupLayout;
    QLabel * const clientNameLabel, * const contractLabel;
    QDateEdit * const billingDateEdit;

    QGroupBox * const meterGroup;
    QFormLayout * const meterGroupLayout;

    // Index Base
    QSpinBox * const initialIndexBox, * const currentIndexBox;
    // Index Heures Pleines
    QSpinBox * const initialIndexHPBox, * const currentIndexHPBox;
    // Index Heures Creuses
    QSpinBox * const initialIndexHCBox, * const currentIndexHCBox;

    QGroupBox * const tariffGroup;
    QFormLayout * const tariffGroupLayout;

    QDoubleSpinBox * const subscriptionPriceBox;
    // Champs de prix détaillés
    QDoubleSpinBox * const kwhPriceBox; // Pour le tarif de Base
    QDoubleSpinBox * const hpKwhPriceBox; // Pour le tarif HP
    QDoubleSpinBox * const hcKwhPriceBox; // Pour le tarif HC

    QLabel * const taxesInfoLabel;

    QHBoxLayout * const footerLayout;
    QLabel * const consumptionLabel;
    QLabel * const totalLabel;

    void setupUI();
    void connectSignals();
};

#endif // INVOICEFORM_HPP