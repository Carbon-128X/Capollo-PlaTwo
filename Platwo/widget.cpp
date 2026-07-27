#include "widget.h"
#include "ui_widget.h"
#include "welcomewindow.h"
#include <QLabel>
#include <QEasingCurve>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) , timer(new QTimer(this)) , progress(0) {
    ui->setupUi(this);

    videoBackground = new VideoBackgroundWidget(this);
    videoBackground->setGeometry(rect());
    videoBackground->lower();
    videoBackground->setVideo(":/images/images/1.mp4");

    ui->loadingBar->setValue(0);
    ui->subtitleLabel->hide();
    startLogoAnimation();
}

Widget::~Widget() {
    delete ui;
}

void Widget::startLogoAnimation() {
    group = new QSequentialAnimationGroup(this);
    QLabel *letters[] =
        {
            ui->labelP,
            ui->labelL,
            ui->labelA,
            ui->labelT,
            ui->labelW,
            ui->labelO
        };

       // انیمیشن پایین آمدن حروف در ابتدای صفحه بازی
    for(int i = 0; i < 6; i++) {
        QPoint endPos = letters[i]->pos();
        QPoint startPos = endPos;
        startPos.setY(-150);
        letters[i]->move(startPos);
         QPropertyAnimation *anim = new QPropertyAnimation(letters[i], "pos");
        anim->setDuration(250);
        anim->setStartValue(startPos);
        anim->setEndValue(endPos);
        anim->setEasingCurve(QEasingCurve::OutBounce);
        group->addAnimation(anim);
    }


    connect(group, &QSequentialAnimationGroup::finished, this, [=]() {
            ui->subtitleLabel->show();
            QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(ui->subtitleLabel);
            ui->subtitleLabel->setGraphicsEffect(effect);

            effect->setOpacity(0);
            QPropertyAnimation *fade = new QPropertyAnimation(effect, "opacity");
                fade->setDuration(800);
                fade->setStartValue(0);
                fade->setEndValue(1);

  connect(fade, &QPropertyAnimation::finished, this, [=]() {

            connect(timer, &QTimer::timeout, this, [=]() {
                    progress++;

                    ui->loadingBar->setValue(progress);

                    if(progress >= 100) {
                        timer->stop();
                        WelcomeWindow *welcome = new WelcomeWindow();

                        welcome->show();

                         close();
                     }
             });

      timer->start(25);

     });

    fade->start(QAbstractAnimation::DeleteWhenStopped);

});

    group->start();
}

void Widget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if(videoBackground){
        videoBackground->setGeometry(rect());
    }
}