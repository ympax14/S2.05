#ifndef USER_HPP
#define USER_HPP

#include "Contract.hpp"

#include <QString>

class User {
public:
    QString id;
    QString password;
    QString fullName;
    Contract* contract; // Le contrat lié à l'utilisateur

    User(const QString& _id, const QString& _pwd, const QString& _name, Contract* _contract) : id(_id), password(_pwd), fullName(_name), contract(_contract) {}

    // SÉRIALISATION JSON
    QJsonObject toJson() const {
        QJsonObject json;
        json["id"] = id;
        json["password"] = password;
        json["fullName"] = fullName;
        if (contract) {
            json["contract"] = contract->toJson();
        }
        return json;
    }

    static User* fromJson(const QJsonObject& json) {
        Contract* c = nullptr;
        if (json.contains("contract")) {
            c = Contract::fromJson(json["contract"].toObject());
        }
        return new User(json["id"].toString(), json["password"].toString(), json["fullName"].toString(), c);
    }
};

#endif // USER_HPP
