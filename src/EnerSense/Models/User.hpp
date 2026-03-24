#ifndef USER_HPP
#define USER_HPP

#include "Contract.hpp"

#include <QString>

class User {
public:
    QString id;
    QString password;
    QString fullName;
    bool isAdmin;
    Contract* contract; // Le contrat lié à l'utilisateur

    User(const QString& _id, const QString& _pwd, const QString& _name, const bool& _isAdmin, Contract* _contract) : id(_id), password(_pwd), fullName(_name), isAdmin(_isAdmin), contract(_contract) {}

    // SÉRIALISATION JSON
    QJsonObject toJson() const {
        QJsonObject json;
        json["id"] = id;
        json["password"] = password;
        json["fullName"] = fullName;
        json["isAdmin"] = isAdmin;
        if (contract) {
            json["contract"] = contract->toJson();
        }
        return json;
    }
};

#endif // USER_HPP
