#ifndef MORRISBOARDWIDGET_H
#define MORRISBOARDWIDGET_H
#include <QWidget>
#include <QPoint>
#include <QColor>
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

    int positionAt(const QPoint &pos) const;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

public:
    // constructor
    explicit MorrisBoardWidget(QWidget *parent = nullptr);
    void setGame(NineMensMorris *g);

signals:
    void boardChanged();

};

#endif