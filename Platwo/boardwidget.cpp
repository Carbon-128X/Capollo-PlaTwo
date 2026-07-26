#include "boardwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <QDebug>
BoardWidget::BoardWidget(QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);

    for(int r=0;r<rows;r++) {
        for(int c=0;c<cols;c++) {
            dots.append(QPoint( margin + c*spacing, margin + r*spacing ));
        }
    }
}
void BoardWidget::setGame(DotsAndBoxes *g) {
    game = g;
    if(game) {
        rows = game->rows();
        cols = game->cols();
        margin = 30;
        int usableWidth  = width()  - 2 * margin;
        int usableHeight = height() - 2 * margin;

        spacing = qMin( usableWidth / (cols - 1), usableHeight / (rows - 1) );
        dots.clear();

        for(int r = 0; r < rows; r++) {
            for(int c = 0; c < cols; c++) {
                dots.append( QPoint( margin + c * spacing, margin + r * spacing ) );
            }
        }
    }

 update();
}

void BoardWidget::setPlayers(
    const QString &p1Name,
    const QString &p2Name,
    const QColor &p1Color,
    const QColor &p2Color)
{
    player1Name = p1Name;
    player2Name = p2Name;

    player1Color = p1Color;
    player2Color = p2Color;

    update();
}

void BoardWidget::setMyTurn(bool value) {
    myTurn = value;
}

void BoardWidget::applyRemoteMove(const Move &move) {
    if(game == nullptr){
        return;
    }

    if(game->applyMove(move)) {
        emit boardChanged();
    }

    update();
}

void BoardWidget::setPlayers(const QString &p1, const QString &p2){
    player1Name = p1;
    player2Name = p2;
    update();
}

void BoardWidget::setPlayerColors(const QColor &c1, const QColor &c2){
    player1Color = c1;
    player2Color = c2;
   update();
}

void BoardWidget::resetBoard() {
    hoverValid=false;
    firstSelected=false;
    update();
}

QPoint BoardWidget::pointFromIndex(int index) const {
    return dots[index];
}

int BoardWidget::nearestDot(const QPoint &pos) const {
    for(int i=0;i<dots.size();i++) {
        if(QLineF(pos,dots[i]).length()<12){
            return i;
        }
    }

 return -1;
}

bool BoardWidget::areNeighbours(int a,int b) const {
    if(a==-1 || b==-1) {
        return false;
    }

    int r1=a/cols;
    int c1=a%cols;
    int r2=b/cols;
    int c2=b%cols;
    if(r1==r2 && qAbs(c1-c2)==1){
        return true;
    }
    if(c1==c2 && qAbs(r1-r2)==1){
        return true;
    }
 return false;
}

void BoardWidget::paintEvent(QPaintEvent *) {
    if(dots.isEmpty()){
        return;
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::transparent);

    // Hover
    if(hoverValid) {
        QPen pen(QColor(120,180,255));
        pen.setWidth(5);
        painter.setPen(pen);
        painter.drawLine(hoverLine);
    }
    //Boxes
    if(game) {
        QFont font;
        font.setBold(true);
        font.setPointSize(10);
        painter.setFont(font);

        for(int r=0;r<rows-1;r++) {
            for(int c=0;c<cols-1;c++) {
                int owner = game->boxOwner(r,c);
                if(owner==0){
                 continue;
                }
                QRect boxRect( margin + c*spacing + 5, margin + r*spacing + 5, spacing - 10, spacing - 10 );
                QColor fillColor;
                QString text;

                if(owner==1) {
                 fillColor = player1Color;
                 text = player1Name;
                }
                else {
                 fillColor = player2Color;
                 text = player2Name;
                }

 fillColor.setAlpha(90);
 painter.fillRect(boxRect,fillColor);
 painter.setPen(Qt::white);
 painter.drawText(boxRect, Qt::AlignCenter, text);
            }
        }
    }
    // Lines

    if(game) {
        auto lines = game->drawnLines();
        for(const Move &m : lines) {
            int owner = game->lineOwner(m.from,m.to);
            QColor color;
            switch(owner) {
            case 1: color = player1Color; break;
            case 2: color = player2Color; break;
            default: color = Qt::white;
            }
            QPen pen(color);
            pen.setWidth(5);
            painter.setPen(pen);
            painter.drawLine( pointFromIndex(m.from), pointFromIndex(m.to) );
        }
    }
    // Draw Dots
    QFont nodeFont("Segoe UI Emoji");
    nodeFont.setPixelSize(22);
    nodeFont.setBold(true);
    painter.setFont(nodeFont);
    painter.setPen(QColor("#121212"));

    for(const QPoint &p : dots) {
        QRect rect(p.x()-12, p.y()-12, 24, 24);
        painter.drawText( rect, Qt::AlignCenter, QStringLiteral("◉") );
    }
}
void BoardWidget::mouseMoveEvent(QMouseEvent *event) {
    hoverValid=false;

    if(firstSelected) {
        int second=nearestDot(event->pos());
        int first=dots.indexOf(firstDot);
        if(areNeighbours(first,second)) {
            Move m;
            m.from=first;
            m.to=second;
            bool legal=false;
            auto moves=game->legalMoves();
            for(const Move &mv:moves) {
                if(m==mv) {
                 legal=true;
                 break;
                }
            }
            if(legal) {
             hoverLine=QLine(firstDot,dots[second]);
             hoverValid=true;
            }
        }
    }

 update();
 QWidget::mouseMoveEvent(event);
}

void BoardWidget::mousePressEvent(QMouseEvent *event) {
    if(game==nullptr){
        return;
    }

    if(!myTurn) {
        return;
    }

    int index=nearestDot(event->pos());
    if(index==-1){
        return;
    }

    if(!firstSelected) {
        firstDot=dots[index];
        firstSelected=true;
        update();
        return;
    }
    int first=dots.indexOf(firstDot);

    if(areNeighbours(first,index)) {
        Move move;
        move.from=first;
        move.to=index;

        emit moveSelected(move);


    }
    firstSelected=false;
    hoverValid=false;
    update();
    QWidget::mousePressEvent(event);

 }
void BoardWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if(game){
        setGame(game);
    }
}