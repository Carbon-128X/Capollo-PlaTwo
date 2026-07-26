#pragma once
#include <QWidget>
#include <QVector>
#include <QPoint>
#include <QColor>
#include "logic/fanorona.h"

class FanoronaBoardWidget : public QWidget {
    Q_OBJECT

public:
    explicit FanoronaBoardWidget(QWidget *parent = nullptr);
    void setGame(Fanorona *g);
    void setPlayers(const QString &p1,const QString &p2,const QColor &c1,const QColor &c2);

signals:
    void boardChanged();

protected:

    void paintEvent(QPaintEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    void mousePressEvent(QMouseEvent *) override;

private:

    Fanorona *game=nullptr;
    QVector<QPoint> positions;
    QString player1Name;
    QString player2Name;
    QColor player1Color;
    QColor player2Color;
    int selectedFrom=-1;

    QVector<int> highlightedMoves;
    static constexpr int CLICK_RADIUS=22;
    void calculatePositions();
    int positionAt(const QPoint &pos) const;
};