#ifndef GUESTWINDOW_H
#define GUESTWINDOW_H
#include <QWidget>
#include "gamewindow.h"

#include <QColor>
#include <QVector>
#include "networkclient.h"
namespace Ui {
class GuestWindow;
}

class GuestWindow : public QWidget {
    Q_OBJECT

public:
    explicit GuestWindow(GameWindow::GameType game, QWidget *parent = nullptr);
    ~GuestWindow();
    void removeColor(const QColor &color);

private slots:
    void on_joinRoomButton_clicked();
    void on_backButton_clicked();

private:
    Ui::GuestWindow *ui;
    GameWindow::GameType currentGame;
    void initializeWindow();
    bool validateInput();

    QVector<QColor> availableColors;
    QColor selectedGuestColor;

    NetworkClient *client = nullptr;

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GUESTWINDOW_H