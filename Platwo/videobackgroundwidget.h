#ifndef VIDEOBACKGROUNDWIDGET_H
#define VIDEOBACKGROUNDWIDGET_H
#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QMediaPlayer>
#include <QAudioOutput>

class VideoBackgroundWidget : public QWidget {
    Q_OBJECT

public:
    explicit VideoBackgroundWidget(QWidget *parent = nullptr);
    void setVideo(const QString &resourcePath);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QMediaPlayer *player;
    QAudioOutput *audio;
    QGraphicsView *view;
    QGraphicsScene *scene;
    QGraphicsVideoItem *videoItem;

};

#endif