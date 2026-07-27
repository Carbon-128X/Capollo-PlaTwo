#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "mainwindow.h"
#include "hostwindow.h"
#include "guestwindow.h"
#include "usermanager.h"
#include "gamehistory.h"
#include <QPixmap>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QMessageBox>
#include "custommessagebox.h"
GameWindow::GameWindow(GameType game, QWidget *parent): QWidget(parent), ui(new Ui::GameWindow), currentGame(game) {
    ui->setupUi(this);

    videoBackground = new VideoBackgroundWidget(this);
    videoBackground->setGeometry(rect());
    videoBackground->lower();
    videoBackground->setVideo(":/images/images/0011.mp4");


    ui->historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->historyTable->horizontalHeader()->setStretchLastSection(true);
    ui->historyTable->verticalHeader()->setVisible(false);
    ui->historyTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->historyTable->setFocusPolicy(Qt::NoFocus);
    ui->historyTable->setShowGrid(false);

    loadGameInformation();
    loadHistory();
}

GameWindow::~GameWindow() {
    delete ui;
}

void GameWindow::loadGameInformation() {
    switch(currentGame) {
    case Boxes:
        ui->gameTitleLabel->setText("Boxes and Dots");
        //ui->gameImageLabel->setPixmap( QPixmap(":/images/images/boxes.jpg").scaled( ui->gameImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        break;
     case Morris:
        ui->gameTitleLabel->setText("Nine Men's Morris");
        // ui->gameImageLabel->setPixmap( QPixmap(":/images/images/morris.jpg").scaled( ui->gameImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        break;
     case Fanorona:
        ui->gameTitleLabel->setText("Fanorona");
        //ui->gameImageLabel->setPixmap(QPixmap(":/images/images/fanorona.jpg").scaled( ui->gameImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        break;
    }
}

void GameWindow::loadHistory() {
    QString gameName;
    switch(currentGame) {
    case Boxes:
        gameName = "Boxes";
        break;

    case Morris:
        gameName = "Morris";
        break;

    case Fanorona:
        gameName = "Fanorona";
        break;
    }

    QVector<GameHistory> history = UserManager::getHistory( UserManager::currentUser.username, gameName);
    if(history.isEmpty()){
        ui->historyTable->setRowCount(1);
        ui->historyTable->setItem( 0, 0, new QTableWidgetItem("No games played yet."));
        return;
    }
    ui->historyTable->setRowCount(history.size());
    for(int i = 0; i < history.size(); i++) {
        ui->historyTable->setItem(i,0,new QTableWidgetItem(history[i].opponent));
        ui->historyTable->setItem(i,1,new QTableWidgetItem(history[i].date));
        ui->historyTable->setItem(i,2,new QTableWidgetItem(history[i].role));
        ui->historyTable->setItem(i,3,new QTableWidgetItem(history[i].winner));
        ui->historyTable->setItem(i,4,new QTableWidgetItem(history[i].score));
    }
}

void GameWindow::on_backButton_clicked(){
    MainWindow *menu = new MainWindow();
    menu->show();
    close();
}

void GameWindow::on_startGameButton_clicked()
{
    if(ui->hostRadioButton->isChecked()) {
        HostWindow *host = new HostWindow(currentGame);
        host->show();
        close();
    }
    else if(ui->guestRadioButton->isChecked()) {
        GuestWindow *guest = new GuestWindow(currentGame);
        guest->show();
        close();
    }
    else {
        CustomMessageBox::warning( this,"Role","Please choose Host or Guest first." );
    }
}

void GameWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if(videoBackground)
        videoBackground->setGeometry(rect());
}
