#include "boxesboardwindow.h"
#include "ui_boxesboardwindow.h"
#include "custommessagebox.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include "usermanager.h"
#include <QDateTime>
#include "mainwindow.h"
BoxesBoardWindow::BoxesBoardWindow( int board, bool timer, int time,
     const QString &p1Name, const QString &p2Name,
     const QColor &p1Color, const QColor &p2Color,
     QWidget *parent) : QWidget(parent), ui(new Ui::BoxesBoardWindow), boardSize(board), timerEnabled(timer), gameTime(time),
     player1Name(p1Name),
     player2Name(p2Name),
     player1Color(p1Color),
     player2Color(p2Color)
{
    ui->setupUi(this);
    turnTime = gameTime;
    remainingTime = turnTime;
    turnTimer = new QTimer(this);
    connect( turnTimer, &QTimer::timeout, this, &BoxesBoardWindow::onTimerTick );
    connect(ui->boardWidget, &BoardWidget::boardChanged, this, &BoxesBoardWindow::refreshGameUI);
    ui->backgroundLabel->setPixmap( QPixmap(":/images/images/454545.png"));
    ui->backgroundLabel->setScaledContents(true);

    //---------------------------------

    ui->backgroundLabel->lower();
    ui->boardWidget->raise();
    game = new DotsAndBoxes(boardSize, boardSize);

    //------------------------------------------------

    ui->boardWidget->setGame(game);


    initializeWindow();
}

BoxesBoardWindow::~BoxesBoardWindow() {
    if(turnTimer){
        turnTimer->stop();
    }
    delete game;
    delete ui;
}

void BoxesBoardWindow::initializeWindow() {
    initializePlayers();
    initializeButtons();
    updateTurn(game->currentPlayer());

    updateScores( game->score(1), game->score(2));

    if(timerEnabled){
        updateTimer(gameTime);
    }
    else{
        ui->timerLabel->setText("--:--");
    }
}

void BoxesBoardWindow::initializePlayers() {
    QString hostName = player1Name.left(4);
    QString guestName = player2Name.left(4);
    ui->player1NameLabel->setText(player1Name);
    ui->player2NameLabel->setText(player2Name);

    ui->player1ColorLabel->setText("꧁                     ꧂");
    ui->player2ColorLabel->setText("꧁                     ꧂ ");
    ui->player1ColorLabel->setStyleSheet(QString("color:%1;").arg(player1Color.name()));
    ui->player2ColorLabel->setStyleSheet( QString("color:%1;").arg(player2Color.name()));
    ui->player1ScoreLabel->setText( QString("Score : %1").arg(game->score(1)));
    ui->player2ScoreLabel->setText( QString("Score : %1").arg(game->score(2)));
    ui->boardWidget->setPlayers( hostName, guestName, player1Color, player2Color);
}

void BoxesBoardWindow::initializeButtons() {
    ui->saveButton->setCursor(Qt::PointingHandCursor);
    ui->restartButton->setCursor(Qt::PointingHandCursor);
    ui->exitButton->setCursor(Qt::PointingHandCursor);
    startTurnTimer();
}

void BoxesBoardWindow::updateTurn(int player) {
    ui->currentTurnLabel->setText( QString("Player %1").arg(player));
}

void BoxesBoardWindow::updateScores(int p1,int p2) {
    ui->player1ScoreLabel->setText( QString("Score : %1").arg(p1));
    ui->player2ScoreLabel->setText( QString("Score : %1").arg(p2));
}

void BoxesBoardWindow::updateTimer(int seconds) {
    int minute = seconds / 60;
    int second = seconds % 60;
    ui->timerLabel->setText( QString("%1:%2").arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0')) );
}

void BoxesBoardWindow::on_saveButton_clicked() {
    CustomMessageBox::information( this, "Save", "Save system will be implemented later.");
}

void BoxesBoardWindow::on_restartButton_clicked() {
    turnTimer->stop();
    delete game;
    game = new DotsAndBoxes(boardSize, boardSize);
    ui->boardWidget->setGame(game);
    // ریست وضعیت پایان بازی
    gameFinished = false;
    winnerPlayer = -1;

    historySaved = false;
    ui->boardWidget->setEnabled(true);

    initializeWindow();

    if (timerEnabled){
        startTurnTimer();
    }
    else{
        updateTimer(gameTime);
    }

    ui->boardWidget->update();
}


void BoxesBoardWindow::on_exitButton_clicked() {
    MainWindow *main = new MainWindow();
    main->show();
    close();
}

void BoxesBoardWindow::refreshGameUI() {

    if (gameFinished){
        return;
    }
    updateTurn(game->currentPlayer());
    updateScores(game->score(1), game->score(2));
    startTurnTimer();
    if(networkGame) ui->boardWidget->setMyTurn(networkGame->session()->isMyTurn());
    else ui->boardWidget->setMyTurn(true);
    ui->boardWidget->update();

    if(game->isGameOver() && !historySaved) {


        historySaved = true;
        saveHistory();

        QString text;
        int winner = game->winner();
        if(winner == 0) {
            text = "Game Finished!\n\nDraw!";
        }
        else {
            text = QString("Game Finished!\n\nWinner : Player %1").arg(winner);
        }


        turnTimer->stop();
        CustomMessageBox::information( this, "Game Over", text );
    }
}
void BoxesBoardWindow::startTurnTimer() {
    if(!timerEnabled){
        return;
    }
    turnTimer->stop();
    remainingTime = turnTime;
    updateTimer(remainingTime);
    turnTimer->start(1000);
}

void BoxesBoardWindow::onTimerTick() {
    remainingTime--;
    updateTimer(remainingTime);
    if (remainingTime > 0){
        return;
    }
    turnTimer->stop();
    gameFinished = true;

    // برنده کسی است که نوبتش نیست
    winnerPlayer = (game->currentPlayer() == 1) ? 2 : 1;
    QString loserName  = (game->currentPlayer() == 1) ? player1Name: player2Name;
    QString winnerName = (winnerPlayer == 1)? player1Name: player2Name;

    CustomMessageBox::information( this, "Time Out", QString("%1 ran out of time!\n\nWinner: %2")
        .arg(loserName).arg(winnerName));

    ui->boardWidget->setEnabled(false);

}

void BoxesBoardWindow::setNetworkGame(NetworkGame *net) {
    networkGame = net;
    if(!networkGame)return;

    connect(ui->boardWidget, &BoardWidget::moveSelected, this,[=](const Move &move){
     networkGame->playMove(move);
     });

    connect(networkGame, &NetworkGame::boardChanged, this,[=](){
      refreshGameUI();
    });

    connect(networkGame, &NetworkGame::remoteMoveReceived,this, [=](const Move &move){
     ui->boardWidget->applyRemoteMove(move);
    });
}

void BoxesBoardWindow::saveHistory() {




    if(!networkGame)
        return;

    GameHistory h;

    h.username = UserManager::currentUser.username;

    h.game = "Boxes";

    h.opponent =
        networkGame->isHost() ?
            player2Name :
            player1Name;

    h.role =
        networkGame->isHost() ?
            "Host" :
            "Guest";

    if(game->winner()==0)
        h.winner="Draw";
    else if(game->winner()==1)
        h.winner=player1Name;
    else
        h.winner=player2Name;

    h.score =
        QString("%1-%2")
            .arg(game->score(1))
            .arg(game->score(2));

    h.date =
        QDateTime::currentDateTime()
            .toString("yyyy/MM/dd hh:mm");
    qDebug() << "saveHistory called";
    qDebug() << "username =" << h.username;
    qDebug() << "opponent =" << h.opponent;

    UserManager::addHistory(h);
}
