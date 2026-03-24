#ifndef INVOICEFORM_HPP
#define INVOICEFORM_HPP

#include "../../Models/Invoice.hpp"

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
    Invoice* const invoice;

    // --- Conteneur global (la zone de défilement) ---
    QVBoxLayout * const mainLayout;
    QScrollArea * const scrollArea;

    // --- Le "Document" (la page de la facture) ---
    QWidget * const scrollAreaWidgetContents;
    QHBoxLayout * const centeringLayout;

    QWidget * const documentContainer;
    QVBoxLayout * const documentLayout;

    // --- En-tête (Informations du Client & Date) ---
    QGroupBox * const detailsGroup;
    QFormLayout * const detailsGroupLayout;
    QLabel * const clientNameLabel;
    QDateEdit * const billingDateEdit;

    // --- Corps (Relevés Compteur & Tarifs) ---
    QGroupBox * const meterGroup;
    QFormLayout * const meterGroupLayout;
    QSpinBox * const initialIndexBox;
    QSpinBox * const currentIndexBox;

    QGroupBox * const tariffGroup;
    QFormLayout * const tariffGroupLayout;
    QDoubleSpinBox * const subscriptionPriceBox;
    QDoubleSpinBox * const kwhPriceBox;

    // --- Pied de page (Total) ---
    QHBoxLayout * const footerLayout;
    QLabel * const consumptionLabel;
    QLabel * const totalLabel;

    void setupUI();
    void connectSignals();
};

#endif // INVOICEFORM_HPP