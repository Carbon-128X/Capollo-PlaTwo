#include "morrisboardwindow.h"
#include "ui_morrisboardwindow.h"
#include "custommessagebox.h"
MorrisBoardWindow::MorrisBoardWindow( bool timer, int time, const QString &p1Name, const QString &p2Name, const QColor &p1Color, const QColor &p2Color, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MorrisBoardWindow),
    timerEnabled(timer),
    gameTime(time),
    player1Name(p1Name),
    player2Name(p2Name),
    player1Color(p1Color),
    player2Color(p2Color) {
    ui->setupUi(this);

    turnTime = gameTime;
    remainingTime = turnTime;

    turnTimer = new QTimer(this);
    connect( turnTimer, &QTimer::timeout, this, &MorrisBoardWindow::onTimerTick );

    game = new NineMensMorris();
    ui->boardWidget->setGame(game);

    connect(ui->boardWidget,&MorrisBoardWidget::boardChanged,this,&MorrisBoardWindow::refreshGameUI);
    ui->backgroundLabel->setPixmap( QPixmap(":/images/images/454545.png"));
    initializeWindow();
}

MorrisBoardWindow::~MorrisBoardWindow() {
    delete game;
    delete ui;
}
void MorrisBoardWindow::initializeWindow() {
    initializePlayers();
    updateScores(game->score(1), game->score(2));
    initializeButtons();
    updateTurn(game->currentPlayer());

    if(timerEnabled){
        updateTimer(gameTime);
    }
    else{
        ui->timerLabel->setText("--:--");
    }
}

void MorrisBoardWindow::initializePlayers() {
    QString hostName = player1Name;
    QString guestName = player2Name;
    ui->player1NameLabel->setText(hostName);
    ui->player2NameLabel->setText(guestName);
    ui->boardWidget->setPlayers( hostName, guestName, player1Color, player2Color);
    ui->player1ColorLabel->setText("꧁                     ꧂");
    ui->player2ColorLabel->setText("꧁                     ꧂");

    ui->player1ColorLabel->setStyleSheet( QString("color:%1;").arg(player1Color.name()));
    ui->player2ColorLabel->setStyleSheet( QString("color:%1;").arg(player2Color.name()));
}

void MorrisBoardWindow::initializeButtons() {
    ui->saveButton->setCursor(Qt::PointingHandCursor);
    ui->restartButton->setCursor(Qt::PointingHandCursor);
    ui->exitButton->setCursor(Qt::PointingHandCursor);

    if(timerEnabled){
        startTurnTimer();
    }
}

void MorrisBoardWindow::updateTurn(int player) {
    ui->currentTurnLabel->setText( QString("Player %1").arg(player));
}

void MorrisBoardWindow::updateTimer(int seconds) {
    int minute = seconds / 60;
    int second = seconds % 60;
    ui->timerLabel->setText(QString("%1:%2").arg(minute,2,10,QChar('0')).arg(second,2,10,QChar('0')));
}

void MorrisBoardWindow::startTurnTimer() {
    if(!timerEnabled){
        return;
    }

    turnTimer->stop();
    remainingTime = turnTime;
    updateTimer(remainingTime);
    turnTimer->start(1000);
}

void MorrisBoardWindow::onTimerTick() {
    remainingTime--;
    updateTimer(remainingTime);

    if (remainingTime > 0){
        return;
    }

    turnTimer->stop();
    gameFinished = true;

    // برنده کسی است که نوبتش نیست
    winnerPlayer = (game->currentPlayer() == 1) ? 2 : 1;

    QString loserName  = (game->currentPlayer() == 1) ? player1Name : player2Name;
    QString winnerName = (winnerPlayer == 1) ? player1Name : player2Name;

    CustomMessageBox::information( this, "Time Out", QString("%1 ran out of time!\n\nWinner: %2")
     .arg(loserName).arg(winnerName));

    ui->boardWidget->setEnabled(false);

}

void MorrisBoardWindow::refreshGameUI() {

    if(gameFinished){
        return;
    }
    updateScores( game->score(1), game->score(2));
    updateTurn(game->currentPlayer());
    startTurnTimer();
    ui->boardWidget->update();
    if(game->isGameOver()) {
        turnTimer->stop();
        QString text;

        if(game->winner()==0){
            text="Draw!";
        }
        else{
            text=QString("Winner : Player %1") .arg(game->winner());
        }
        CustomMessageBox::information( this,"Game Over", text);
    }

}

void MorrisBoardWindow::on_restartButton_clicked() {
    turnTimer->stop();
    delete game;
    game = new NineMensMorris();
    ui->boardWidget->setGame(game);
    // ریست وضعیت پایان بازی
    gameFinished = false;
    winnerPlayer = -1;

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

void MorrisBoardWindow::on_exitButton_clicked() {
    close();
}

void MorrisBoardWindow::on_saveButton_clicked() {
    CustomMessageBox::information( this,"Save", "Save system will be implemented later.");
}

void MorrisBoardWindow::updateScores(int p1,int p2) {
    ui->player1ScoreLabel->setText( QString("Score : %1").arg(p1));
    ui->player2ScoreLabel->setText( QString("Score : %1").arg(p2));
}