#include "morrisboardwidget.h"
#include <QPainter>
#include <QMouseEvent>

MorrisBoardWidget::MorrisBoardWidget(QWidget *parent) : QWidget(parent){

}

void MorrisBoardWidget::setGame(NineMensMorris *g) {
    game = g;

    if (game->notPlaced(1) > 0 || game->notPlaced(2) > 0)
        placingPhase = true;
    else
        placingPhase = false;

    calculatePositions();
    update();
}

void MorrisBoardWidget::paintEvent(QPaintEvent *) {
    if(!game)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor("#00E5FF"));
    pen.setWidth(4);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    // outer square
    painter.drawLine(positions[0], positions[2]);
    painter.drawLine(positions[2], positions[4]);
    painter.drawLine(positions[4], positions[6]);
    painter.drawLine(positions[6], positions[0]);

    // middle square
    painter.drawLine(positions[8], positions[10]);
    painter.drawLine(positions[10], positions[12]);
    painter.drawLine(positions[12], positions[14]);
    painter.drawLine(positions[14], positions[8]);

    // inner square
    painter.drawLine(positions[16], positions[18]);
    painter.drawLine(positions[18], positions[20]);
    painter.drawLine(positions[20], positions[22]);
    painter.drawLine(positions[22], positions[16]);


    // connecting them(squares) together
    painter.drawLine(positions[1],positions[9]);
    painter.drawLine(positions[9],positions[17]);

    painter.drawLine(positions[3],positions[11]);
    painter.drawLine(positions[11],positions[19]);

    painter.drawLine(positions[5],positions[13]);
    painter.drawLine(positions[13],positions[21]);

    painter.drawLine(positions[7],positions[15]);
    painter.drawLine(positions[15],positions[23]);
    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);

    for(const QPoint &p : positions)
        painter.drawEllipse(p,8,8);

    painter.setBrush(QColor(255,255,0,120));
    painter.setPen(Qt::NoPen);

    for(int pos : highlightedMoves)
        painter.drawEllipse(positions[pos],10,10);

    // showing the selected piece
    if(selectedFrom != -1)
    {
        painter.setBrush(QColor(255,255,0,80));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(positions[selectedFrom],22,22);
    }
    // showing the removable pieces
    if(game->mustRemove())
    {
        painter.setBrush(QColor(255,50,50,170));
        painter.setPen(Qt::NoPen);
        std::vector<Move> legal = game->legalMoves();
        for(const Move &m : legal)
            painter.drawEllipse(positions[m.to],22,22);
    }

    for(int i = 0; i < 24; i++)
    {
        int state = game->posStatus(i);
        if(state==0)
            continue;

        QColor color;
        if(state==1)
            color = QColor("#FF3B3B");

        else
            color = QColor("#2F80FF");

        painter.setBrush(color);
        QPen piecePen(Qt::black);
        piecePen.setWidth(2);
        painter.setPen(piecePen);
        painter.drawEllipse(positions[i],16,16);
    }
}

void MorrisBoardWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    calculatePositions();
}

void MorrisBoardWidget::mousePressEvent(QMouseEvent *event) {
    if(!game)
        return;

    int pos = positionAt(event->pos());
    if(pos == -1)
        return;


    if(game->mustRemove())      // removing
    {
        std::vector<Move> legal = game->legalMoves();

        for(const Move &m : legal)
            if(m.to == pos)
            {
                if(game->applyMove(m))
                {
                    highlightedMoves.clear();
                    selectedFrom = -1;

                    if (game->notPlaced(1) > 0 || game->notPlaced(2) > 0)
                        placingPhase = true;
                    else
                        placingPhase = false;

                    update();
                    emit boardChanged();
                }

                return;
            }

        return;
    }

    if(placingPhase)      // if we were still in placing phase
    {
        std::vector<Move> legal = game->legalMoves();
        for(const Move &m : legal)
            if(m.to == pos)
            {
                if(game->applyMove(m))
                {
                    if (game->notPlaced(1) > 0 || game->notPlaced(2) > 0)
                        placingPhase = true;
                    else
                        placingPhase = false;
                    update();
                    emit boardChanged();
                }
                return;
            }

        return;
    }

    int currentPlayer = game->currentPlayer();
    highlightedMoves.clear();
    if(game->posStatus(pos) == currentPlayer)
    {
        selectedFrom = pos;
        highlightedMoves.clear();
        std::vector<Move> legal = game->legalMoves();

        for(const Move &m : legal)
            if(m.from == selectedFrom)
                highlightedMoves.push_back(m.to);

        update();
        return;
    }

    if(selectedFrom != -1)
    {
        std::vector<Move> legal = game->legalMoves();

        for(const Move &m : legal)
        {
            if(m.from == selectedFrom && m.to == pos)
            {
                if(game->applyMove(m))
                {
                    highlightedMoves.clear();
                    selectedFrom = -1;

                    if (game->notPlaced(1) > 0 || game->notPlaced(2) > 0)
                        placingPhase = true;
                    else
                        placingPhase = false;

                    update();
                    emit boardChanged();
                }

                return;
            }
        }

        highlightedMoves.clear();
        selectedFrom = -1;
        update();
        return;
    }
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

int MorrisBoardWidget::positionAt(const QPoint &pos) const {
    for(int i=0;i<positions.size();i++)
        if(QLineF(pos, positions[i]).length() <= CLICK_RADIUS)
            return i;

    return -1;
}