#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H
#include <QWidget>
#include "videobackgroundwidget.h"
namespace Ui {
class GameWindow;
}

class GameWindow : public QWidget {
    Q_OBJECT
public:
    enum GameType {
        Boxes,
        Morris,
        Fanorona
    };
    explicit GameWindow(GameType game, QWidget *parent = nullptr);
    ~GameWindow();

private slots:
    void on_backButton_clicked();
    void on_startGameButton_clicked();
private:
    Ui::GameWindow *ui;
    GameType currentGame;
    void loadGameInformation();
    void loadHistory();

    VideoBackgroundWidget *videoBackground;
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GAMEWINDOW_H