#ifndef NEWTARIFFDIALOG_HPP
#define NEWTARIFFDIALOG_HPP

#include <QDialog>
#include <QDateEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QDialogButtonBox>

class NewTariffDialog : public QDialog {
public:
    QFormLayout * const layout;

    QDateEdit * const dateEdit;
    QDoubleSpinBox * const subPriceBox;
    QDoubleSpinBox * const kwhPriceBox;

    QDialogButtonBox * const btnBox;

    NewTariffDialog(QWidget* parent = nullptr);
};

#endif // NEWTARIFFDIALOG_HPP
