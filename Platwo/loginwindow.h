#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QWidget>
#include "videobackgroundwidget.h"
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent =nullptr);
    ~LoginWindow();

private slots:
    void on_backButton_clicked();
    void on_forgotPasswordButton_clicked();
    void on_loginButton_clicked();
private :
    VideoBackgroundWidget *videoBackground;

private:
    Ui::LoginWindow *ui;

    void resizeEvent(QResizeEvent *event) override;
};

#endif // LOGINWINDOW_H