#ifndef INVOICE_HPP
#define INVOICE_HPP

#include <stdexcept>
#include <map>

#include <QString>

enum InvoiceType {
    ELECTRICITY,
    GAS
};

struct InvoiceField {
    QString name;
    bool fixed;
    double price, constant;
    int quantity;

    inline QString getName() const { return this->name; };
    inline bool isFixed() const { return this->fixed; };
    inline double getPrice() const { return this->price; };
    inline double getConstant() const { return this->constant; };
    inline int getQuantity() const { return this->quantity; };
    inline double getTotal() const {
        return (this->price * this->quantity) + this->constant;
    };
};

class Invoice {
private:
    std::map<std::string, InvoiceField*> fields;
public:
    QString clientName, billName;
    InvoiceType type;

    Invoice(const QString& _clientName, const QString& _billName, const InvoiceType& _type = InvoiceType::ELECTRICITY) : clientName(_clientName), billName(_billName), type(_type) {};
    ~Invoice() {
        for (std::map<std::string, InvoiceField*>::const_iterator it = fields.cbegin(); it != fields.cend(); it++)
            delete it->second;
    };

    InvoiceField* addField(const std::string& key, const QString& name, bool fixed = true, double price = 0.0, double constant = 0.0, int quantity = 1) {
        if (fields.find(key) != fields.end()) {
            throw std::invalid_argument("La clé est déjà présente !");
        } else {
            InvoiceField* field = new InvoiceField{name, fixed, price, constant, quantity};
            fields[key] = field;

            return field;
        }
    }

    inline const std::map<std::string, InvoiceField*>& getFields() const {
        return this->fields;
    }

    inline const InvoiceType& getType() const { return this->type; };
    inline const QString& getClientName() const { return this->clientName; };
    inline const QString& getBillName() const { return this->billName; };
};

#endif // INVOICE_HPP
