#ifndef INVOICEFORMADDFIELDDIALOG_H
#define INVOICEFORMADDFIELDDIALOG_H

#include "../InvoiceForm.hpp"

#include <QDialog>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>

class InvoiceFormAddFieldDialog : public QDialog {
    Q_OBJECT

public:
    explicit InvoiceFormAddFieldDialog(InvoiceForm* const &_parentForm = nullptr);

    inline QString getName() { return this->fieldName->text(); };
    inline bool isFixed() { return this->fieldFixed->isChecked(); };

private:
    InvoiceForm* parentForm;

    QVBoxLayout *mainLayout;

    QFormLayout *formLayout;
    QLineEdit *fieldName;
    QCheckBox *fieldFixed;

    QHBoxLayout *btnLayout;
    QPushButton *confirmBtn, *closeBtn;

    void connectButtons();

private slots:
    void confirm();
};

#endif // INVOICEFORMADDFIELDDIALOG_H
