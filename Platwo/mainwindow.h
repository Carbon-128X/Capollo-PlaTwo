#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QWidget>
#include <QPixmap>
namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent =nullptr);
    ~MainWindow();
    enum GameType
    {
        None,
        Boxes,
        Morris,
        Fanorona
    };

private slots:
    void on_boxesButton_clicked();
    void on_morrisButton_clicked();
    void on_fanoronaButton_clicked();
    void on_startGameButton_clicked();
    void on_profileButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::MainWindow *ui;

    GameType selectedGame = None;
    void showBoxes();
    void showMorris();
    void showFanorona();

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // MAINWINDOW_H