#ifndef CONTRACT_HPP
#define CONTRACT_HPP

#include "Offer.hpp"
#include <QJsonObject>

class Contract {
public:
    EnergyOffer* linkedOffer;

    Contract(EnergyOffer* offer) : linkedOffer(offer) {}

    QJsonObject toJson() const {
        QJsonObject json;

        if (linkedOffer) {
            // On sauvegarde juste le nom de l'offre pour la retrouver au chargement
            json["providerName"] = linkedOffer->providerName;
            json["offerName"] = linkedOffer->offerName;
        }

        return json;
    }
};

#endif // CONTRACT_HPP