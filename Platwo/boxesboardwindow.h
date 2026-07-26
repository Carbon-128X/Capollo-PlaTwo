#ifndef BOXESBOARDWINDOW_H
#define BOXESBOARDWINDOW_H
#include <QTimer>
#include <QWidget>
#include "logic/dotsAndBoxes.h"
#include "networkGame.h"
namespace Ui {
class BoxesBoardWindow;
}

class BoxesBoardWindow : public QWidget {
    Q_OBJECT

public:
    explicit BoxesBoardWindow( int boardSize, bool timerEnabled, int gameTime,
        const QString &player1Name,
        const QString &player2Name,
        const QColor &player1Color,
        const QColor &player2Color,
        QWidget *parent = nullptr);
    void setNetworkGame(NetworkGame *net);
    ~BoxesBoardWindow();


private:

    Ui::BoxesBoardWindow *ui;
    DotsAndBoxes *game = nullptr;
    NetworkGame *networkGame = nullptr;
    void initializeWindow();
    void initializePlayers();
    void initializeButtons();

    void updateTurn(int player);
    void updateScores(int p1,int p2);
    void updateTimer(int seconds);
    int boardSize;
    bool timerEnabled;
    int gameTime;

    QTimer *turnTimer = nullptr;
    int turnTime = 0;
    int remainingTime = 0;
    void startTurnTimer();

    QString player1Name;
    QString player2Name;
    QColor player1Color;
    QColor player2Color;
    bool gameFinished = false;
    int winnerPlayer = -1;


private slots:
    void on_exitButton_clicked();
    void on_restartButton_clicked();
    void on_saveButton_clicked();
    void refreshGameUI();
    void onTimerTick();
};

#endif