#ifndef STRETCHTABWIDGET_HPP
#define STRETCHTABWIDGET_HPP

#include <QTabWidget>
#include <QTabBar>
#include <QResizeEvent>

class StretchTabBar : public QTabBar {
public:
    explicit StretchTabBar(QWidget* const &parent = nullptr);

    QSize tabSizeHint(int index) const override;
};

class StretchTabWidget : public QTabWidget {
public:
    explicit StretchTabWidget(QWidget* const &parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // STRETCHTABWIDGET_HPP
