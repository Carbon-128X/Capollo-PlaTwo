#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "custommessagebox.h"
#include "editprofilewindow.h"
#include "usermanager.h"
#include "gamewindow.h"
#include <QApplication>
#include <QPixmap>
#include <QGraphicsDropShadowEffect>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) , ui(new Ui::MainWindow) {
    ui->setupUi(this);


    ui->boxesButton->setIcon(QIcon(":/images/images/4444.png"));
    ui->boxesButton->setIconSize(QSize(146,146));

    ui->morrisButton->setIcon(QIcon(":/images/images/3333.png"));
    ui->morrisButton->setIconSize(QSize(140,140));

    ui->fanoronaButton->setIcon(QIcon(":/images/images/2222.png"));
    ui->fanoronaButton->setIconSize(QSize(138,138));



    ui->backgroundLabel->setPixmap(QPixmap(":/images/images/1212.png"));
    ui->backgroundLabel->setScaledContents(true);
    ui->backgroundLabel->setGeometry(rect());
    ui->backgroundLabel->lower();


    selectedGame = None;

    auto glow1 = new QGraphicsDropShadowEffect(this);
    glow1->setBlurRadius(35);
    glow1->setOffset(0);
    glow1->setColor(QColor(0,220,255));
    ui->boxesButton->setGraphicsEffect(glow1);

    auto glow2 = new QGraphicsDropShadowEffect(this);
    glow2->setBlurRadius(35);
    glow2->setOffset(0);
    glow2->setColor(QColor(0,220,255));
    ui->morrisButton->setGraphicsEffect(glow2);

    auto glow3 = new QGraphicsDropShadowEffect(this);
    glow3->setBlurRadius(35);
    glow3->setOffset(0);
    glow3->setColor(QColor(0,220,255));
    ui->fanoronaButton->setGraphicsEffect(glow3);

    ui->gameTitle->setText("Welcome, " + UserManager::currentUser.name + "!");
    ui->gameDescription->setText("Choose one of the games.");
    ui->startGameButton->setEnabled(false);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::showBoxes() {
    selectedGame = Boxes;
    ui->gameTitle->setText("Boxes and Dots");
    ui->gameDescription->setText( "Connect dots to complete boxes and earn more points than your opponent.");
    ui->startGameButton->setEnabled(true);
}

void MainWindow::showMorris() {
    selectedGame = Morris;
    ui->gameTitle->setText("Nine Men's Morris");
    ui->gameDescription->setText( "Create mills and capture your opponent's pieces to win.");
    ui->startGameButton->setEnabled(true);
}

void MainWindow::showFanorona() {
    selectedGame = Fanorona;
    ui->gameTitle->setText("Fanorona");
    ui->gameDescription->setText( "Traditional strategy game from Madagascar with unique capture rules.");
    ui->startGameButton->setEnabled(true);
}

void MainWindow::on_boxesButton_clicked() {
    showBoxes();
}

void MainWindow::on_morrisButton_clicked() {
    showMorris();
}

void MainWindow::on_fanoronaButton_clicked() {
    showFanorona();
}

void MainWindow::on_startGameButton_clicked() {
    if(selectedGame == None) {
        CustomMessageBox::warning( this, "Game", "Please select a game first.");
        return;
    }
    GameWindow *gameWindow = nullptr;

    switch(selectedGame) {
    case Boxes:
        gameWindow = new GameWindow(GameWindow::Boxes);
        break;
    case Morris:
        gameWindow = new GameWindow(GameWindow::Morris);
        break;
    case Fanorona:
        gameWindow = new GameWindow(GameWindow::Fanorona);
        break;
    default:
        return;
    }
    gameWindow->show();
    close();
}

void MainWindow::on_profileButton_clicked() {
    EditProfileWindow *edit = new EditProfileWindow();
    edit->show();
    close();
}

void MainWindow::on_exitButton_clicked() {
    QApplication::quit();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    ui->backgroundLabel->setGeometry(rect());
}