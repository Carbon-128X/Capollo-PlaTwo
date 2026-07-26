#include <QPainter>
#include <QMouseEvent>
#include "fanoronaboardwidget.h"
#include "logic/move.h"
void FanoronaBoardWidget::calculatePositions() {
    positions.clear();
    int margin = 45;

    int usableW = width() - margin * 2;
    int usableH = height() - margin * 2;
    double dx = usableW / 8.0;
    double dy = usableH / 4.0;

    for(int r=0;r<5;r++) {
        for(int c=0;c<9;c++) {
            positions.push_back( QPoint( margin + qRound(c*dx), margin + qRound(r*dy)));
        }
    }
}

void FanoronaBoardWidget::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    calculatePositions();
}
void FanoronaBoardWidget::setGame(Fanorona *g){
    game = g;
    calculatePositions();
    update();
}

void FanoronaBoardWidget::setPlayers(const QString &p1,const QString &p2,const QColor &c1,const QColor &c2) {
    player1Name = p1;
    player2Name = p2;
    player1Color = c1;
    player2Color = c2;
    update();

}

void FanoronaBoardWidget::setMyTurn(bool value) {
    myTurn = value;
}

void FanoronaBoardWidget::applyRemoteMove(const Move &move) {
    if(game == nullptr){
        return;
    }

    if(game->applyMove(move)) {
        emit boardChanged();
    }

    update();
}

int FanoronaBoardWidget::positionAt(const QPoint &pos) const {
    for(int i=0;i<positions.size();i++) {
        if(QLineF(pos,positions[i]).length()<=CLICK_RADIUS){
            return i;
        }
    }

    return -1;
}


struct Edge {
    int from;
    int to;
};


void FanoronaBoardWidget::paintEvent(QPaintEvent *) {

    qDebug() << "isStrong(12)=" << game->isStrongPoint(12)
    << "isStrong(22)=" << game->isStrongPoint(22)
    << "isStrong(32)=" << game->isStrongPoint(32);
    if(!game) return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //خط ها

    QPen pen(QColor("#00E5FF"));
    pen.setWidth(3);
    pen.setCapStyle(Qt::RoundCap);
    painter.setPen(pen);

    auto draw=[&](int a,int b) {
        painter.drawLine(positions[a],positions[b]);
    };


    // Horizontal

    for(int r=0;r<5;r++){
        for(int c=0;c<8;c++){
            draw(r*9+c,r*9+c+1);
        }
    }

    // Vertical

    for(int c=0;c<9;c++){
        for(int r=0;r<4;r++){
            draw(r*9+c,(r+1)*9+c);
        }
    }
    // Diagonal


    for(int r=0;r<4;r++) {
        for(int c=0;c<8;c++) {
            int id=r*9+c;
            if(!game->isStrongPoint(id)){
                continue;
            }

            draw(id,id+10);
            draw(id+2,id+10);
        }
    }
    draw(10,18);
    draw(28,36);

    // Highlight

    painter.setBrush(QColor(255,255,0,120));
    painter.setPen(Qt::NoPen);
    for(int p:highlightedMoves){
        painter.drawEllipse(positions[p],10,10);
    }

    if(selectedFrom!=-1){
        painter.drawEllipse(positions[selectedFrom],20,20);
    }

    // مهره ها

    for(int i=0;i<45;i++) {
        int s=game->posStatus(i);
        if(s==0) continue;

        QColor color=(s==1)?player1Color:player2Color;
        QRadialGradient g(positions[i]-QPoint(6,6),18);
        g.setColorAt(0,color.lighter(170));
        g.setColorAt(0.6,color);
        g.setColorAt(1,color.darker(180));
        painter.setBrush(g);
        QPen pp(Qt::black);
        pp.setWidth(2);
        painter.setPen(pp);
        painter.drawEllipse(positions[i],15,15);
        painter.setBrush(QColor(255,255,255,80));
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(positions[i].x()-6, positions[i].y()-8, 7, 7 );
    }
}
void FanoronaBoardWidget::mousePressEvent(QMouseEvent *event) {
    if(!game){
        return;
    }

    int pos = positionAt(event->pos());
    if(pos == -1){
        return;
    }

    // اگر قبلاً مهره انتخاب شده

    if(selectedFrom != -1){
        std::vector<Move> legal = game->legalMoves();

        for(const Move &m : legal){
            if(m.from == selectedFrom && m.to == pos){

                emit moveSelected(m);


                    highlightedMoves.clear();
                    selectedFrom = -1;
                    update();



                return;
            }
        }

        highlightedMoves.clear();
        selectedFrom = -1;
        update();
        return;
    }

    if(game->inCaptureChain()) {
        std::vector<Move> legal = game->legalMoves();
        for(const Move &m : legal){
            if(m.from == selectedFrom && m.to == pos){

                emit moveSelected(m);


                    highlightedMoves.clear();
                    selectedFrom = pos;
                    update();


                return;
            }
        }
    }

    // انتخاب مهره

    if(game->posStatus(pos) == game->currentPlayer()) {
        selectedFrom = pos;
        highlightedMoves.clear();
        std::vector<Move> legal = game->legalMoves();
        for(const Move &m : legal){
            if(m.from == pos){
                highlightedMoves.push_back(m.to);
            }
        }

        update();
    }
}

FanoronaBoardWidget::FanoronaBoardWidget(QWidget *parent): QWidget(parent){}