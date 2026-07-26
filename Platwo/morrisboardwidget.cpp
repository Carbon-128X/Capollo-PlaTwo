#include "morrisboardwidget.h"
#include <QPainter>
#include <QMouseEvent>

MorrisBoardWidget::MorrisBoardWidget(QWidget *parent) : QWidget(parent){

}

void MorrisBoardWidget::setGame(NineMensMorris *g) {
    game = g;
    update();
}

void MorrisBoardWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
}

void MorrisBoardWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    calculatePositions();
}

void MorrisBoardWidget::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
}

void MorrisBoardWidget::calculatePositions(){}