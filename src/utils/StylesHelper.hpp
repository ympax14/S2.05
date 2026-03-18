#ifndef STYLESHELPER_HPP
#define STYLESHELPER_HPP

#include <QWidget>
#include <QString>
#include <QFile>

namespace StylesHelper {
    void loadStyle(QWidget* const &widget, const QString& filePath);
}

#endif // STYLESHELPER_HPP
