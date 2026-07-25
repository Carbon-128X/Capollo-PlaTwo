#ifndef HOSTWINDOW_H
#define HOSTWINDOW_H
#include <QWidget>
#include "gamewindow.h"
#include <QColor>
#include <QVector>
#include "networkserver.h"
#include <QHostAddress>
namespace Ui {
class HostWindow;
}

class HostWindow : public QWidget {
    Q_OBJECT

public:
    explicit HostWindow(GameWindow::GameType game, QWidget *parent = nullptr);
    ~HostWindow();
    int getBoardSize() const;
    int getGameTime() const;
    bool hasTimer() const;
    QColor hostColor() const;

private slots:
    void on_timeLimitCheck_clicked(bool checked);
    void on_createRoomButton_clicked();
    void on_backButton_clicked();


private:
    Ui::HostWindow *ui;
    GameWindow::GameType currentGame;
    void initializeWindow();
    bool validateInput();
    QString getLocalIP();

    int boardSize = 6;
    int gameTime = 0;
    bool timerEnabled = false;

    QVector<QColor> availableColors;
    QColor selectedHostColor;

    NetworkServer *server = nullptr;
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // HOSTWINDOW_H