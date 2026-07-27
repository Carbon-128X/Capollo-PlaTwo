#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QTimer>
#include <QLabel>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>
#include "videobackgroundwidget.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void startLogoAnimation();

    Ui::Widget *ui;
    QTimer *timer;
    int progress;
    QSequentialAnimationGroup *group;
    VideoBackgroundWidget *videoBackground;


protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // WIDGET_H