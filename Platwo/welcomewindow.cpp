#include "welcomewindow.h"
#include "ui_welcomewindow.h"
#include "loginwindow.h"
#include "signupwindow.h"

WelcomeWindow::WelcomeWindow(QWidget *parent) : QWidget(parent), ui(new Ui::WelcomeWindow) {
    ui->setupUi(this);

    videoBackground = new VideoBackgroundWidget(this);
    videoBackground->setGeometry(rect());
    videoBackground->lower();
    videoBackground->setVideo(":/images/images/3-4.mp4");

}

WelcomeWindow::~WelcomeWindow() {
    delete ui;
}

void WelcomeWindow::on_loginButton_clicked() {
    LoginWindow *login = new LoginWindow();
    login->show();
    this->close();
}

void WelcomeWindow::on_signupButton_clicked() {
    SignupWindow *signup = new SignupWindow();
    signup->show();
    this->close();
}

void WelcomeWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if(videoBackground)
        videoBackground->setGeometry(rect());
}