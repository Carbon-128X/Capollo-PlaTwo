#include "hostwindow.h"
#include "ui_hostwindow.h"
#include "gamewindow.h"
#include <QIntValidator>
#include "boxesboardwindow.h"
#include <QPixmap>
#include <QPainter>
#include <QIcon>
#include "custommessagebox.h"
#include "morrisboardwindow.h"
#include "fanoronaboardwindow.h"
#include "usermanager.h"
HostWindow::HostWindow(GameWindow::GameType game, QWidget *parent): QWidget(parent),ui(new Ui::HostWindow),currentGame(game){
    ui->setupUi(this);
    networkGame = new NetworkGame(this);


    connect(networkGame, &NetworkGame::gameStarted, this, [=]() {
                if(currentGame == GameWindow::Boxes){
                    BoxesBoardWindow *board = new BoxesBoardWindow( boardSize,timerEnabled,gameTime,
                     QString::fromStdString(networkGame->config().host.name),
                     QString::fromStdString(networkGame->config().guest.name),
                     QColor(QString::fromStdString(networkGame->config().host.color)),
                     QColor(QString::fromStdString(networkGame->config().guest.color))
                            );
                  board->setNetworkGame(networkGame);
                 board->show();
                 close();
                }

         else if(currentGame == GameWindow::Morris){
            MorrisBoardWindow *board = new MorrisBoardWindow( timerEnabled, gameTime,
                 QString::fromStdString(networkGame->config().host.name),
                 QString::fromStdString(networkGame->config().guest.name),
                 QColor(QString::fromStdString(networkGame->config().host.color)),
                 QColor(QString::fromStdString(networkGame->config().guest.color))
                 );
         board->setNetworkGame(networkGame);
         board->show();
         close();
                }

     else{
     FanoronaBoardWindow *board = new FanoronaBoardWindow( timerEnabled, gameTime,
         QString::fromStdString(networkGame->config().host.name),
         QString::fromStdString(networkGame->config().guest.name),
         QColor(QString::fromStdString(networkGame->config().host.color)),
         QColor(QString::fromStdString(networkGame->config().guest.color))
         );
          board->setNetworkGame(networkGame);
         board->show();
         close();
     }
 });
    availableColors = {
            Qt::red,
            Qt::blue,
            Qt::green,
            Qt::yellow,
            Qt::cyan,
            Qt::magenta,
            QColor("#FF8800"),
            QColor("#8A2BE2"),
            QColor("#00AA55"),
            QColor("#FF1493")
        };

    QStringList names = {
            "Red",
            "Blue",
            "Green",
            "Yellow",
            "Cyan",
            "Magenta",
            "Orange",
            "Purple",
            "Dark Green",
            "Pink"
        };

    for(int i=0;i<availableColors.size();i++) {
        QPixmap pix(20,20);
        pix.fill(Qt::transparent);
        QPainter painter(&pix);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(availableColors[i]);
        painter.setPen(Qt::black);
        painter.drawEllipse(2,2,16,16);
        ui->colorComboBox->addItem(QIcon(pix), names[i]);
    }

    ui->backgroundLabel->setPixmap(QPixmap(":/images/images/1111.png"));
    ui->backgroundLabel->setScaledContents(true);
    ui->backgroundLabel->setGeometry(rect());
    ui->backgroundLabel->lower();

    initializeWindow();

    selectedHostColor = availableColors[0];

    connect(ui->colorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,[=](int index){
         if(index >= 0 && index < availableColors.size()){
            selectedHostColor = availableColors[index];
        }
    });
}

HostWindow::~HostWindow() {
    delete ui;
}

void HostWindow::initializeWindow() {
    ui->statusLabel->hide();
    ui->timeEdit->setEnabled(false);
    ui->timeEdit->setText("120");
    ui->portEdit->setPlaceholderText("e.g. 5000");
    ui->portEdit->setValidator(new QIntValidator(1024,65535,this));
    ui->timeEdit->setValidator( new QIntValidator(1,99999,this));
    if(currentGame != GameWindow::Boxes) {
        ui->boardSizeLabel->hide();
        ui->boardSizeCombo->hide();
    }
    ui->ipValueLabel->setText("--------");

}


bool HostWindow::validateInput() {
    if(ui->portEdit->text().trimmed().isEmpty()) {
        ui->statusLabel->show();
        ui->statusLabel->setText("Enter a valid port.");
        return false;
    }

    if(ui->timeLimitCheck->isChecked()) {
        if(ui->timeEdit->text().trimmed().isEmpty()) {
            ui->statusLabel->show();
            ui->statusLabel->setText("Enter game time.");
            return false;
        }
    }

 return true;
}

QString HostWindow::getLocalIP() {
    // بعداً با QNetworkInterface جایگزین می‌شود.
    return "127.0.0.1";
}

void HostWindow::on_timeLimitCheck_clicked(bool checked) {
    ui->timeEdit->setEnabled(checked);
}

void HostWindow::on_createRoomButton_clicked() {
    if(!validateInput()){
        return;
    }

    quint16 port = ui->portEdit->text().toUShort();

    //اندازه زمین گیم
     boardSize = 6;

    if(currentGame == GameWindow::Boxes){
        boardSize = ui->boardSizeCombo->currentText().split(" ").first().toInt();
    }

    // تایمر
     timerEnabled = ui->timeLimitCheck->isChecked();
     gameTime = 120;

    if(timerEnabled){
        gameTime = ui->timeEdit->text().toInt();
    }

    GameConfig config;

    switch(currentGame) {
    case GameWindow::Boxes:
        config.gameType = GAME_DOTS_AND_BOXES;
        config.boardSize = boardSize;
        break;

    case GameWindow::Morris:
        config.gameType = GAME_NINE_MENS_MORRIS;
        break;

    case GameWindow::Fanorona:
        config.gameType = GAME_FANORONA;
        break;
    }

    config.timerEnabled = timerEnabled;
    config.turnSeconds = gameTime;


    PlayerInfo me;
    me.name = UserManager::currentUser.username.toStdString();
    me.color = selectedHostColor.name().toStdString();

    if(!networkGame->startHost(config, me, port)) {
        CustomMessageBox::warning(this,"Server Error","Cannot start server.");
        return;
    }

    ui->statusLabel->show();
    ui->statusLabel->setText("Waiting for player...");

    ui->ipValueLabel->setText(getLocalIP());


    //-----------------------------------


}

void HostWindow::on_backButton_clicked() {
    GameWindow *game = new GameWindow(currentGame);
    game->show();
    close();
}

int HostWindow::getBoardSize() const {
    return boardSize;
}

int HostWindow::getGameTime() const {
    return gameTime;
}

bool HostWindow::hasTimer() const {
    return timerEnabled;
}

void HostWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    ui->backgroundLabel->setGeometry(rect());
}

QColor HostWindow::hostColor() const {
    return selectedHostColor;
}