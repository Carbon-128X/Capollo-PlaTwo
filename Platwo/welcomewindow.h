#ifndef WELCOMEWINDOW_H
#define WELCOMEWINDOW_H
#include <QWidget>
#include "videobackgroundwidget.h"
namespace Ui {
class WelcomeWindow;
}

class WelcomeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);
    ~WelcomeWindow();

private slots:
    void on_loginButton_clicked();
    void on_signupButton_clicked();

private:
    Ui::WelcomeWindow *ui;

    VideoBackgroundWidget *videoBackground;

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // WELCOMEWINDOW_H