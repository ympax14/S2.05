#include "InvoiceFormField.hpp"

InvoiceFormField::InvoiceFormField(InvoiceField* const &field) :
    QGroupBox{field->getName()},

    form(new QFormLayout(this)),
    quantity(field->isFixed() ? nullptr : new QSpinBox(this)),
    price(new QDoubleSpinBox(this)),
    constant(field->isFixed() ? nullptr : new QDoubleSpinBox(this))
{
    this->price->setMinimum(0.0);
    this->price->setMaximum(999999.99);
    this->price->setValue(field->getPrice());

    if (field->isFixed()) {
        this->form->addRow("Prix (€)", this->price);
    } else {
        this->quantity->setMinimum(1);
        this->quantity->setMaximum(999999);
        this->quantity->setValue(field->getQuantity());

        this->form->addRow("Quantité ou Prix (€)", this->quantity);
        this->form->addRow("Prix unitaire (€) ou Coefficient", this->price);

        this->constant->setMinimum(0.0);
        this->constant->setValue(field->getConstant());
        this->form->addRow("Constante (€)", this->constant);

        QObject::connect(this->quantity, QOverload<int>::of(&QSpinBox::valueChanged), [field](int val){
            field->quantity = val;
        });

        QObject::connect(this->constant, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [field](double val){
            field->constant = val;
        });
    }

    QObject::connect(this->price, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [field](double val){
        field->price = val;
    });
}
