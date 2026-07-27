#include "videobackgroundwidget.h"
#include <QUrl>
#include <QDebug>
#include <QFrame>

VideoBackgroundWidget::VideoBackgroundWidget(QWidget *parent) : QWidget(parent) {
    setAttribute(Qt::WA_TransparentForMouseEvents);
    view = new QGraphicsView(this);
    scene = new QGraphicsScene(this);
    videoItem = new QGraphicsVideoItem();
    scene->addItem(videoItem);
    view->setScene(scene);
    view->setFrameShape(QFrame::NoFrame);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setStyleSheet("background: transparent; border: none;");
    view->setBackgroundBrush(Qt::NoBrush);
    scene->setBackgroundBrush(Qt::NoBrush);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    player = new QMediaPlayer(this);

    audio = new QAudioOutput(this);
    audio->setVolume(0);

    player->setAudioOutput(audio);
    player->setVideoOutput(videoItem);

    connect(player, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status) {
         qDebug() << "Media Status =" << status;
        if(status == QMediaPlayer::EndOfMedia) {
            player->setPosition(0);
            player->play();
        }
    });

    connect(player, &QMediaPlayer::playbackStateChanged, this, [](QMediaPlayer::PlaybackState state) {
        qDebug() << "Playback State =" << state;
     });

    connect(player, &QMediaPlayer::errorOccurred,this,[](QMediaPlayer::Error error,const QString &errorString) {
        qDebug() << "Player Error =" << error;
        qDebug() << errorString;
     });
}

void VideoBackgroundWidget::setVideo(const QString &resourcePath) {
    QString path = resourcePath;
    if(path.startsWith(":/")){
        path = "qrc" + path;
    }
    qDebug() << "Loading Video:" << path;
    player->setSource(QUrl(path));
    qDebug() << "Source =" << player->source();
    player->play();
}

void VideoBackgroundWidget::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
    view->setGeometry(rect());
    scene->setSceneRect(rect());
    videoItem->setPos(0,0);
    videoItem->setSize(rect().size());
}