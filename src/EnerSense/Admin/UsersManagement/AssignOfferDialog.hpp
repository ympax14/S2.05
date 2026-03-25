#ifndef ASSIGNOFFERDIALOG_HPP
#define ASSIGNOFFERDIALOG_HPP

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>

class AssignOfferDialog : public QDialog {
private:
    QVBoxLayout * const layout;

    QLabel * const assignOfferLabel;
    QComboBox * const offerCombo;

    QComboBox * const kvaCombo;
    QCheckBox * const hphcCheck;

    QDialogButtonBox * const btnBox;

public:
    AssignOfferDialog(QWidget* parent = nullptr);

    inline const QComboBox * getOfferCombo() const { return this->offerCombo; };
    inline int getKva() const { return this->kvaCombo->currentText().replace(" kVA", "").toInt(); }
    inline bool isHphc() const { return this->hphcCheck->isChecked(); }
};

#endif // ASSIGNOFFERDIALOG_HPP