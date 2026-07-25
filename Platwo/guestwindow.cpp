#include "guestwindow.h"
#include "ui_guestwindow.h"
#include "gamewindow.h"
#include <QIntValidator>
#include <QPainter>
#include <QPixmap>
#include <QIcon>
GuestWindow::GuestWindow(GameWindow::GameType game, QWidget *parent) : QWidget(parent), ui(new Ui::GuestWindow), currentGame(game) {
    ui->setupUi(this);

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

    ui->colorComboBox->clear();
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
    client = new NetworkClient(this);
    selectedGuestColor = availableColors[0];

    connect(ui->colorComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,[=](int index){
        if(index >= 0 && index < availableColors.size()){
            selectedGuestColor = availableColors[index];
        }
    });
}

GuestWindow::~GuestWindow( ){
    delete ui;
}

void GuestWindow::initializeWindow() {
    ui->statusLabel->hide();
    ui->ipEdit->setPlaceholderText("192.168.1.10");
    ui->portEdit->setPlaceholderText("5000");
    ui->portEdit->setValidator( new QIntValidator(1024,65535,this));
}
bool GuestWindow::validateInput() {
    if(ui->ipEdit->text().trimmed().isEmpty()) {
        ui->statusLabel->show();
        ui->statusLabel->setText("Enter server IP.");
     return false;
    }

    if(ui->portEdit->text().trimmed().isEmpty()) {
        ui->statusLabel->show();
        ui->statusLabel->setText("Enter server port.");
        return false;
    }

 return true;
}

void GuestWindow::on_joinRoomButton_clicked() {
    if(!validateInput()){
        return;
    }

    ui->statusLabel->show();
    ui->statusLabel->setText("Connecting...");
    QString ip = ui->ipEdit->text();
    quint16 port = ui->portEdit->text().toUShort();

    if(!client->connectToServer(ip, port)) {
        ui->statusLabel->setText("Connection failed.");
        return;
    }
    ui->statusLabel->setText("Connecting...");

    /*
    اینجا بعدا صفحه و قسمت هر بازی باز میشه
    */
}

void GuestWindow::on_backButton_clicked() {
    GameWindow *game = new GameWindow(currentGame);
    game->show();
    close();
}

void GuestWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    ui->backgroundLabel->setGeometry(rect());
}


void GuestWindow::removeColor(const QColor &color) {
    for(int i=0;i<availableColors.size();i++) {
        if(availableColors[i] == color) {
            availableColors.remove(i);
            ui->colorComboBox->removeItem(i);
            break;
        }
    }

    if(!availableColors.isEmpty()) {
        selectedGuestColor = availableColors[ui->colorComboBox->currentIndex()];
    }
}