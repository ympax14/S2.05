#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include "../EnerSense/Models/User.hpp"
#include "../EnerSense/Models/Offer.hpp"

#include <vector>
#include <QString>

class DataManager {
private:
    std::vector<User*> users;
    std::vector<EnergyOffer*> offers;
    User* currentUser;
    const QString filePath = "enersense_data.json";

    // Constructeur privé "Singleton"
    DataManager();
    ~DataManager();

public:
    // Récupérer l'instance (qui est unique puisque Singleton)
    static DataManager& getInstance() {
        static DataManager instance;
        return instance;
    }

    // Empêcher la copie
    DataManager(const DataManager&) = delete;
    void operator=(const DataManager&) = delete;

    void loadData();
    void saveData() const;

    bool authenticate(const QString& id, const QString& password);

    EnergyOffer* getOfferByName(const QString& provider, const QString& name);

    inline User* getCurrentUser() const { return currentUser; };
    inline std::vector<User*>& getUsers() { return users; };
    inline std::vector<EnergyOffer*>& getOffers() { return offers; };

    void addOffer(EnergyOffer* offer);
    void addUser(User* user);
};

#endif // DATAMANAGER_HPP