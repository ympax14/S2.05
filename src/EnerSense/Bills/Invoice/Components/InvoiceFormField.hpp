#ifndef INVOICEFORMFIELD_H
#define INVOICEFORMFIELD_H

#include "../Invoice.hpp"

#include <QGroupBox>
#include <QFormLayout>
#include <QSpinBox>

class InvoiceFormField : public QGroupBox
{
    Q_OBJECT

private:
    QFormLayout *form;
    QSpinBox *quantity;
    QDoubleSpinBox *price, *constant;

public:
    InvoiceFormField(InvoiceField* const &field);
};

#endif // INVOICEFORMFIELD_H
