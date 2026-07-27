#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "welcomewindow.h"
#include "loginwindow.h"
#include "usermanager.h"
#include "security.h"
#include <QMessageBox>
#include <QRegularExpression>
#include "custommessagebox.h"
SignupWindow::SignupWindow(QWidget *parent) : QWidget(parent) , ui(new Ui::SignupWindow) {
    ui->setupUi(this);

    videoBackground = new VideoBackgroundWidget(this);
    videoBackground->setGeometry(rect());
    videoBackground->lower();
    videoBackground->setVideo(":/images/images/20.mp4");
}

SignupWindow::~SignupWindow() {
    delete ui;
}

void SignupWindow::on_backButton_clicked() {
    WelcomeWindow *welcome = new WelcomeWindow();
    welcome->show();
    this->close();
}

void SignupWindow::on_loginButton_clicked() {
    LoginWindow *login = new LoginWindow();
    login->show();
    this->close();
}

void SignupWindow::on_signupButton_clicked() {
    QString name = ui->nameEdit->text().trimmed();
    QString username = ui->usernameEdit->text().trimmed();
    QString phone = ui->phoneEdit->text().trimmed();
    QString email = ui->emailEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    QString confirm = ui->confirmPasswordEdit->text();


    if(name.isEmpty() || username.isEmpty() || phone.isEmpty() || email.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
        CustomMessageBox::warning(this , "Error", "Please fill all fields.");
        return;
    }

    if(password != confirm)
    {
        CustomMessageBox::warning(this, "Error", "Passwords do not match.");
        return;
    }

    QRegularExpression emailRegex("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");

    if(!emailRegex.match(email).hasMatch()){
        CustomMessageBox::warning(this, "Error", "Invalid email address.");
        return;
    }

    QRegularExpression phoneRegex("^09\\d{9}$");

    if(!phoneRegex.match(phone).hasMatch()) {
        CustomMessageBox::warning(this,"Error","Phone number must be in the format:\n09XXXXXXXXX");
        return;
    }

    QRegularExpression passwordRegex( "^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&]).{8,}$");

    if(!passwordRegex.match(password).hasMatch()){
        CustomMessageBox::warning(this,"Weak Password","Password must contain:\n""• At least 8 characters\n""• One uppercase letter\n"
        "• One lowercase letter\n""• One number\n""• One special character (@$!%*?&)");
        return;
    }

    User user;
    user.name = name;
    user.username = username;
    user.phone = phone;
    user.email = email;
    user.passwordHash = Security::hashPassword(password);

    if(UserManager::registerUser(user)) {
        CustomMessageBox::information(this, "Success", "Registration completed successfully.");
        LoginWindow *login = new LoginWindow();
        login->show();
        this->close();
    }
    else{
        CustomMessageBox::warning(this,"Error", "Username or phone number already exists.");
    }
}

void SignupWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if(videoBackground)
        videoBackground->setGeometry(rect());
}