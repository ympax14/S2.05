#ifndef ENERSENSEWINDOW_H
#define ENERSENSEWINDOW_H

#include <QMainWindow>

class EnerSenseWindow : public QMainWindow {
    Q_OBJECT
private:

public:
    static const int WINDOW_WIDTH = 1280, WINDOW_HEIGHT = 720;

    explicit EnerSenseWindow(QWidget *parent = nullptr);

signals:
};

#endif // ENERSENSEWINDOW_H
