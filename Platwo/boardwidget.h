#ifndef BOARDWIDGET_H
#define BOARDWIDGET_H
#include <QWidget>
#include <QPoint>
#include <QVector>
#include <QLine>
#include <QColor>

#include "logic/dotsAndBoxes.h"
class BoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit BoardWidget(QWidget *parent = nullptr);
    void resetBoard();
    void setGame(DotsAndBoxes *g);
    void setPlayers(const QString& p1, const QString& p2);
    void setPlayerColors(const QColor& c1, const QColor& c2);
    void setPlayers( const QString &p1Name, const QString &p2Name, const QColor &p1Color, const QColor &p2Color );

    void applyRemoteMove(const Move &move);
    void setMyTurn(bool value);
signals:
    void boardChanged();
    void moveSelected(const Move &move);
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:

    // Logic , Board , Hover , Players

    DotsAndBoxes *game = nullptr;
    int rows = 6;
    int cols = 6;
    int margin = 35;
    int spacing = 70;
    QVector<QPoint> dots;
    bool hoverValid = false;
    QLine hoverLine;
    QPoint firstDot;
    bool firstSelected = false;
    QString player1Name = "Player1";
    QString player2Name = "Player2";
    QColor player1Color = QColor(40,170,255);
    QColor player2Color = QColor(255,80,80);

    int nearestDot(const QPoint &pos) const;
    bool areNeighbours(int a,int b) const;
    QPoint pointFromIndex(int index) const;

    bool myTurn = true;
};

#endif