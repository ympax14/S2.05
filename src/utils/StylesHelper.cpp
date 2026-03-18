#include "StylesHelper.hpp"

void StylesHelper::loadStyle(QWidget* const &widget, const QString& filePath) {
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Impossible d'ouvrir le fichier à l'adresse: " << filePath;
        exit(EXIT_FAILURE);
    }

    widget->setStyleSheet(file.readAll());
}