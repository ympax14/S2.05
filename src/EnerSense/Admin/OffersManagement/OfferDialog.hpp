#ifndef OFFERDIALOG_H
#define OFFERDIALOG_H

#include <QLineEdit>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFormLayout>
#include <QDateEdit>

class OfferDialog : public QDialog {
private:
    QFormLayout * const layout;

    QLineEdit * const providerEdit, * const nameEdit;
    QComboBox * const typeCombo;
    QDateEdit * const dateEdit;
    QDoubleSpinBox * const subPriceBox, * const kwhPriceBox;

    QDialogButtonBox * const btnBox;

public:
    OfferDialog(QWidget* parent = nullptr);

    inline QLineEdit * getProviderEdit() const { return this->providerEdit; };
    inline QLineEdit * getNameEdit() const { return this->nameEdit; };
    inline QComboBox * getTypeCombo() const { return this->typeCombo; };
    inline QDateEdit * getDateEdit() const { return this->dateEdit; };
    inline QDoubleSpinBox * getSubPriceBox() const { return this->subPriceBox; };
    inline QDoubleSpinBox * getKwhPriceBox() const { return this->kwhPriceBox; };
};

#endif // OFFERDIALOG_H
