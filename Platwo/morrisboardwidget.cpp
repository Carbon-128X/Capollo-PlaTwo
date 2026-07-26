#include "morrisboardwidget.h"
#include <QPainter>
#include <QMouseEvent>

MorrisBoardWidget::MorrisBoardWidget(QWidget *parent) : QWidget(parent){

}

void MorrisBoardWidget::setGame(NineMensMorris *g) {
    game = g;
    calculatePositions();
    update();
}

void MorrisBoardWidget::paintEvent(QPaintEvent *) {
    if(!game)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor(25,25,25));

    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);

    for(const QPoint &p : positions)
        painter.drawEllipse(p,6,6);
}

void MorrisBoardWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    calculatePositions();
}

void MorrisBoardWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}

void MorrisBoardWidget::calculatePositions(){
    positions.clear();

    int w = width();
    int h = height();
    int s = qMin(w, h);
    int margin = s / 10;
    int outer = s - 2 * margin;
    int middle = outer * 2 / 3;
    int inner = outer / 3;
    QPoint center(w / 2, h / 2);
    int o = outer / 2;
    int m = middle / 2;
    int i = inner / 2;

    // outer square in board

    positions << QPoint(center.x()-o, center.y()-o);
    positions << QPoint(center.x(),   center.y()-o);
    positions << QPoint(center.x()+o, center.y()-o);
    positions << QPoint(center.x()+o, center.y());
    positions << QPoint(center.x()+o, center.y()+o);
    positions << QPoint(center.x(),   center.y()+o);
    positions << QPoint(center.x()-o, center.y()+o);
    positions << QPoint(center.x()-o, center.y());

    // middle square in board

    positions << QPoint(center.x()-m, center.y()-m);
    positions << QPoint(center.x(),   center.y()-m);
    positions << QPoint(center.x()+m, center.y()-m);
    positions << QPoint(center.x()+m, center.y());
    positions << QPoint(center.x()+m, center.y()+m);
    positions << QPoint(center.x(),   center.y()+m);
    positions << QPoint(center.x()-m, center.y()+m);
    positions << QPoint(center.x()-m, center.y());

    // inner square in board

    positions << QPoint(center.x()-i, center.y()-i);
    positions << QPoint(center.x(),   center.y()-i);
    positions << QPoint(center.x()+i, center.y()-i);
    positions << QPoint(center.x()+i, center.y());
    positions << QPoint(center.x()+i, center.y()+i);
    positions << QPoint(center.x(),   center.y()+i);
    positions << QPoint(center.x()-i, center.y()+i);
    positions << QPoint(center.x()-i, center.y());
}