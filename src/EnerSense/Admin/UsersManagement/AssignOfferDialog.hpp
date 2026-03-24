#ifndef ASSIGNOFFERDIALOG_HPP
#define ASSIGNOFFERDIALOG_HPP

#include <QDialog>
#include <QComboBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDIalogButtonBox>

class AssignOfferDialog : public QDialog {
private:
    QVBoxLayout * const layout;

    QLabel * const assignOfferLabel;
    QComboBox * const offerCombo;

    QDialogButtonBox * const btnBox;

public:
    AssignOfferDialog(QWidget* parent = nullptr);

    inline const QComboBox * getOfferCombo() const { return this->offerCombo; };
};

#endif // ASSIGNOFFERDIALOG_HPP
