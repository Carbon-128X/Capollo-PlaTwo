#include "fanoronaboardwindow.h"
#include "ui_fanoronaboardwindow.h"
#include "custommessagebox.h"

FanoronaBoardWindow::FanoronaBoardWindow( bool timer, int time, const QString &p1Name, const QString &p2Name, const QColor &p1Color,
                                         const QColor &p2Color, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FanoronaBoardWindow),
    timerEnabled(timer),
    gameTime(time),
    player1Name(p1Name),
    player2Name(p2Name),
    player1Color(p1Color),
    player2Color(p2Color)
{
    ui->setupUi(this);

    turnTime = gameTime;
    remainingTime = turnTime;
    turnTimer = new QTimer(this);
    connect(turnTimer, &QTimer::timeout, this,&FanoronaBoardWindow::onTimerTick);

    game = new Fanorona();
    ui->boardWidget->setGame(game);
    connect(ui->boardWidget, &FanoronaBoardWidget::boardChanged, this, &FanoronaBoardWindow::refreshGameUI);
    ui->backgroundLabel->setPixmap(QPixmap(":/images/images/4545.jpg"));

    initializeWindow();
}

FanoronaBoardWindow::~FanoronaBoardWindow(){
    delete game;
    delete ui;
}

void FanoronaBoardWindow::initializeWindow() {
    initializePlayers();
    updateScores( game->score(1), game->score(2));
    initializeButtons();
    updateTurn(game->currentPlayer());

    if(timerEnabled){
        updateTimer(gameTime);
    }else{
        ui->timerLabel->setText("--:--");
    }
}

void FanoronaBoardWindow::initializePlayers(){
    ui->player1NameLabel->setText(player1Name);
    ui->player2NameLabel->setText(player2Name);
    ui->boardWidget->setPlayers(player1Name,player2Name,player1Color, player2Color);
    ui->player1ColorLabel->setText("꧁                     ꧂");
    ui->player2ColorLabel->setText("꧁                     ꧂");

    ui->player1ColorLabel->setStyleSheet( QString("color:%1;").arg(player1Color.name()));
    ui->player2ColorLabel->setStyleSheet( QString("color:%1;").arg(player2Color.name()));
}

void FanoronaBoardWindow::initializeButtons() {
    ui->saveButton->setCursor(Qt::PointingHandCursor);
    ui->restartButton->setCursor(Qt::PointingHandCursor);
    ui->exitButton->setCursor(Qt::PointingHandCursor);
    if(timerEnabled){
        startTurnTimer();
    }
}

void FanoronaBoardWindow::updateTurn(int player) {
    ui->currentTurnLabel->setText(QString("Player %1").arg(player));
}

void FanoronaBoardWindow::updateTimer(int seconds) {
    int minute = seconds / 60;
    int second = seconds % 60;
    ui->timerLabel->setText( QString("%1:%2").arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0')));
}

void FanoronaBoardWindow::startTurnTimer() {
    if(!timerEnabled){
        return;
    }

    turnTimer->stop();
    remainingTime = turnTime;
    updateTimer(remainingTime);
    turnTimer->start(1000);
}

void FanoronaBoardWindow::onTimerTick() {
    remainingTime--;
    updateTimer(remainingTime);
    if(remainingTime > 0){
        return;
    }

    turnTimer->stop();
    gameFinished = true;
    winnerPlayer = (game->currentPlayer()==1)?2:1;

    QString loserName = (game->currentPlayer()==1)?player1Name:player2Name;
    QString winnerName =(winnerPlayer==1)?player1Name:player2Name;
    CustomMessageBox::information( this, "Time Out", QString("%1 ran out of time!\n\nWinner : %2").arg(loserName) .arg(winnerName));
    ui->boardWidget->setEnabled(false);
}


void FanoronaBoardWindow::refreshGameUI(){
    if(gameFinished){
        return;
    }
    updateScores( game->score(1), game->score(2));
    updateTurn( game->currentPlayer());

    startTurnTimer();
    if(networkGame) ui->boardWidget->setMyTurn(networkGame->session()->isMyTurn());
    else ui->boardWidget->setMyTurn(true);
    ui->boardWidget->update();

    if(game->isGameOver()){
        turnTimer->stop();
        QString text;

        if(game->winner()==0){
            text="Draw!";
        }else{
            text=QString("Winner : Player %1").arg(game->winner());
        }
        CustomMessageBox::information( this, "Game Over",text);
    }
}

void FanoronaBoardWindow::on_restartButton_clicked(){

    turnTimer->stop();
    delete game;
    game = new Fanorona();
    ui->boardWidget->setGame(game);
    gameFinished=false;
    winnerPlayer=-1;
    ui->boardWidget->setEnabled(true);
    initializeWindow();

    if(timerEnabled){
        startTurnTimer();
    }
    else{
        updateTimer(gameTime);
        ui->boardWidget->update();
    }
}

void FanoronaBoardWindow::on_exitButton_clicked(){
    close();
}

void FanoronaBoardWindow::on_saveButton_clicked(){
    CustomMessageBox::information(this, "Save","Save system will be implemented later.");
}

void FanoronaBoardWindow::updateScores(int p1,int p2){
    ui->player1ScoreLabel->setText( QString("Score : %1").arg(p1));
    ui->player2ScoreLabel->setText( QString("Score : %1").arg(p2));
}

void FanoronaBoardWindow::setNetworkGame(NetworkGame *net) {
    networkGame = net;

    if(!networkGame)
        return;

    connect(ui->boardWidget,&FanoronaBoardWidget::moveSelected,this,[=](const Move &move){
        networkGame->playMove(move);
    });

    connect(networkGame,&NetworkGame::remoteMoveReceived,this,[=](const Move &move) {
        ui->boardWidget->applyRemoteMove(move);
    });

    connect(networkGame, &NetworkGame::boardChanged,this,[=]() {
        refreshGameUI();
    });
}
