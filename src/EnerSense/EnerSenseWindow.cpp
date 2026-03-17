#include "EnerSenseWindow.hpp"

EnerSenseWindow::EnerSenseWindow(QWidget *parent) : QMainWindow{parent} {
    // Titre de la Fenêtre Principale.
    this->setWindowTitle("EnerSense");

    // Modification de la taille de la Fenêtre Principale et taille fixe.
    this->resize(EnerSenseWindow::WINDOW_WIDTH, EnerSenseWindow::WINDOW_HEIGHT);
    this->setFixedSize(this->size());
}
