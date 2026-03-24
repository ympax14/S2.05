#ifndef CREATEUSERDIALOG_H
#define CREATEUSERDIALOG_H

#include <QMessageBox>
#include <QInputDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QDialogButtonBox>

class CreateUserDialog : public QDialog {
private:
    QFormLayout * const layout;

    QLineEdit * const idEdit, * const nameEdit, * const pwdEdit;
    QCheckBox * const adminCheck;

    QDialogButtonBox * const btnBox;

public:
    CreateUserDialog(QWidget* parent = nullptr);

    inline const QLineEdit * getIdEdit() const { return this->idEdit; };
    inline const QLineEdit * getNameEdit() const { return this->nameEdit; };
    inline const QLineEdit * getPwdEdit() const { return this->pwdEdit; };
    inline const QCheckBox * getAdminCheck() const { return this->adminCheck; };
};

#endif // CREATEUSERDIALOG_H
