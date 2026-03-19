#include "StretchTabWidget.hpp"

StretchTabBar::StretchTabBar(QWidget* const &parent) : QTabBar(parent) {}

QSize StretchTabBar::tabSizeHint(int index) const {
    QSize s = QTabBar::tabSizeHint(index);

    if (count() > 0) {
        int width = this->parentWidget()->width() / this->count();
        return QSize(width, s.height());
    }

    return s;
}

StretchTabWidget::StretchTabWidget(QWidget* const &parent) : QTabWidget(parent) {
    this->setTabBar(new StretchTabBar(this));
    this->setDocumentMode(true);  // Permet l'expansion
}

void StretchTabWidget::resizeEvent(QResizeEvent *event) {
    QTabWidget::resizeEvent(event);
    this->tabBar()->update();  // Force le recalcul
}
