#ifndef MORRISBOARDWIDGET_H
#define MORRISBOARDWIDGET_H
#include <QWidget>
#include <QPoint>
#include <QColor>
#include <QString>
#include <QVector>
#include "logic/nineMensMorris.h"

class MorrisBoardWidget : public QWidget {
    Q_OBJECT

private:

    NineMensMorris *game = nullptr;
    QVector<QPoint> positions;
    void calculatePositions();
    int selectedFrom = -1;
    QVector<int> highlightedMoves;
    bool placingPhase = true;
    static constexpr int CLICK_RADIUS = 20;

    QString player1Name;
    QString player2Name;

    QColor player1Color;
    QColor player2Color;

    int positionAt(const QPoint &pos) const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    // constructor
    explicit MorrisBoardWidget(QWidget *parent = nullptr);
    void setGame(NineMensMorris *g);
    void setPlayers(const QString &p1, const QString &p2, const QColor &c1, const QColor &c2);

signals:
    void boardChanged();

};

#endif