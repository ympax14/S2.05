#ifndef INVOICEFORM_HPP
#define INVOICEFORM_HPP

#include <map>

#include "Invoice.hpp"
#include "Components/InvoiceFormField.hpp"

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QScrollArea>

class InvoiceForm : public QWidget
{
    Q_OBJECT

public:
    explicit InvoiceForm(Invoice* const &_invoice, QWidget* const &parent = nullptr);

    inline const std::map<std::string, InvoiceFormField*>& getFields() const { return this->fields; };
    bool containsField(const std::string& key);

private slots:
    void calculateTotal();
    void addField();

private:
    Invoice* const invoice;
    std::map<std::string, InvoiceFormField*> fields;

    QVBoxLayout *mainLayout;
    QLabel *totalLabel;
    QPushButton *calculateBtn;

    QFormLayout *formLayout;

    QGroupBox* detailsGroup;
    QFormLayout* detailsGroupLayout;
    QLineEdit *clientNameEdit;
    QLineEdit *itemNameEdit;

    QPushButton *addFieldBtn;

    void updateWindowName();
    void setupForm();
    void connectBtns();
};

#endif // INVOICEFORM_HPP