#ifndef DATAMANAGER_HPP
#define DATAMANAGER_HPP

#include "../EnerSense/Models/User.hpp"
#include <vector>
#include <QString>

class DataManager {
private:
    std::vector<User*> users;
    User* currentUser;
    const QString filePath = "users_data.json";

    // Constructeur privé "Singleton"
    DataManager();
    ~DataManager();

public:
    // Récupérer l'instance qui est unique
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

    inline User* getCurrentUser() const {
        return currentUser;
    }

    // Pour créer un compte si besoin plus tard
    void addUser(User* user);
};

#endif // DATAMANAGER_HPP