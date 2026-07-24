#ifndef MORRISBOARDWINDOW_H
#define MORRISBOARDWINDOW_H
#include "logic/nineMensMorris.h"
#include "morrisboardwidget.h"
#include <QWidget>
#include <QColor>
#include <QTimer>
namespace Ui {
class MorrisBoardWindow;
}

class MorrisBoardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MorrisBoardWindow( bool timer, int time, const QString &p1Name,
                               const QString &p2Name, const QColor &p1Color, const QColor &p2Color, QWidget *parent = nullptr );
    ~MorrisBoardWindow();

private:
    Ui::MorrisBoardWindow *ui;
    NineMensMorris *game;
    bool timerEnabled;
    int gameTime;
    QString player1Name;
    QString player2Name;
    QColor player1Color;
    QColor player2Color;
    QTimer *turnTimer;
    int turnTime;
    int remainingTime;
    bool gameFinished = false;
    int winnerPlayer = -1;


    void initializeWindow();
    void initializePlayers();
    void initializeButtons();
    void updateTurn(int player);
    void updateTimer(int seconds);
    void startTurnTimer();
    void updateScores(int p1, int p2);

private slots:
    void onTimerTick();
    void refreshGameUI();
    void on_restartButton_clicked();
    void on_exitButton_clicked();
    void on_saveButton_clicked();
};

#endif // MORRISBOARDWINDOW_H
