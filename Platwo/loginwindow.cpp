#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "welcomewindow.h"
#include "forgotpasswordwindow.h"
#include "usermanager.h"
#include "security.h"
#include "mainwindow.h"
#include <QMessageBox>
#include "custommessagebox.h"
LoginWindow::LoginWindow(QWidget *parent): QWidget(parent), ui(new Ui::LoginWindow){
    ui->setupUi(this);

    videoBackground = new VideoBackgroundWidget(this);
    videoBackground->setGeometry(rect());
    videoBackground->lower();
    videoBackground->setVideo(":/images/images/20.mp4");
}

LoginWindow::~LoginWindow(){
    delete ui;
}

void LoginWindow::on_backButton_clicked(){
    WelcomeWindow *welcome = new WelcomeWindow();
    welcome->show();
    this->close();
}

void LoginWindow::on_forgotPasswordButton_clicked() {
    ForgotPasswordWindow *forgot = new ForgotPasswordWindow();
    forgot->show();
    this->close();
}


void LoginWindow::on_loginButton_clicked(){
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text();

    if(username.isEmpty() || password.isEmpty()){
        CustomMessageBox::warning(this, "Error","Please fill all fields.");
        return;
    }

    QString hash = Security::hashPassword(password);

    if(UserManager::login(username, hash)) {
        CustomMessageBox::information(this,"Success","Login successful.");
        MainWindow *mainMenu = new MainWindow();

        mainMenu->show();

        this->close();

    }
    else{
        CustomMessageBox::warning(this,"Error","Username or password is incorrect.");
    }
}

void LoginWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if(videoBackground)
        videoBackground->setGeometry(rect());
}