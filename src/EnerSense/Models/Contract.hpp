#ifndef CONTRACT_HPP
#define CONTRACT_HPP

#include "Offer.hpp"
#include <QJsonObject>

class Contract {
public:
    EnergyOffer* linkedOffer;
    int powerKVA; // Puissance souscrite (ex: 6 ou 9)
    bool isHPHC; // True = Heures Pleines / Heures Creuses, False = Base

    Contract(EnergyOffer* offer, int kva = 6, bool hphc = false) : linkedOffer(offer), powerKVA(kva), isHPHC(hphc) {}

    QJsonObject toJson() const {
        QJsonObject json;
        if (linkedOffer) {
            json["providerName"] = linkedOffer->providerName;
            json["offerName"] = linkedOffer->offerName;
        }
        json["powerKVA"] = powerKVA;
        json["isHPHC"] = isHPHC;
        return json;
    }
};

#endif // CONTRACT_HPP