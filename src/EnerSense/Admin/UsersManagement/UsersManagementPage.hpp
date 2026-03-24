#ifndef USERSMANAGEMENTPAGE_HPP
#define USERSMANAGEMENTPAGE_HPP

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QPushButton>

class UsersManagementPage : public QWidget {
    Q_OBJECT

private:
    QVBoxLayout * const mainLayout;
    QTableWidget * const usersTable;

    QHBoxLayout * const btnLayout;
    QPushButton * const btnCreate;
    QPushButton * const btnDelete;
    QPushButton * const btnResetPwd;
    QPushButton * const btnAssignOffer;
    QPushButton * const btnCancelContract;

    void setupUI();

public:
    explicit UsersManagementPage(QWidget* const &parent = nullptr);

private slots:
    void refreshTable();
    void createUser();
    void deleteUser();
    void resetPassword();
    void assignOffer();
    void cancelContract();
};

#endif // USERSMANAGEMENTPAGE_HPP