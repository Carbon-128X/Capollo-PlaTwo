#include "editprofilewindow.h"
#include "ui_editprofilewindow.h"
#include "mainwindow.h"
#include "usermanager.h"
#include "security.h"
#include "custommessagebox.h"
#include <QRegularExpression>

EditProfileWindow::EditProfileWindow(QWidget *parent) : QWidget(parent) , ui(new Ui::EditProfileWindow) {
    ui->setupUi(this);

    videoBackground = new VideoBackgroundWidget(this);
    videoBackground->setGeometry(rect());
    videoBackground->lower();
    videoBackground->setVideo(":/images/images/20.mp4");


    loadUserData();
}

EditProfileWindow::~EditProfileWindow() {
    delete ui;
}

void EditProfileWindow::loadUserData() {
    User user = UserManager::currentUser;
    ui->nameEdit->setText(user.name);
    ui->usernameEdit->setText(user.username);
    ui->phoneEdit->setText(user.phone);
    ui->emailEdit->setText(user.email);
    ui->passwordEdit->clear();
    ui->confirmPasswordEdit->clear();
}

bool EditProfileWindow::validateInput() {
    QString name = ui->nameEdit->text().trimmed();
    QString username = ui->usernameEdit->text().trimmed();
    QString phone = ui->phoneEdit->text().trimmed();
    QString email = ui->emailEdit->text().trimmed();
    QString password = ui->passwordEdit->text();
    QString confirm = ui->confirmPasswordEdit->text();

    if(name.isEmpty() || username.isEmpty() || phone.isEmpty() || email.isEmpty() || password.isEmpty() || confirm.isEmpty()) {
        CustomMessageBox::warning( this, "Error", "Please fill all fields.");
        return false;
    }

    if(password != confirm){
        CustomMessageBox::warning( this, "Error", "Passwords do not match.");
        return false;
    }

    if(password.length() < 8) {
        CustomMessageBox::warning( this, "Error", "Password must be at least 8 characters.");
        return false;
    }

    QRegularExpression emailRegex( "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    if(!emailRegex.match(email).hasMatch()) {
        CustomMessageBox::warning( this, "Error", "Invalid email.");
        return false;
    }

    QRegularExpression phoneRegex("^09\\d{9}$");
    if(!phoneRegex.match(phone).hasMatch()) {
        CustomMessageBox::warning( this, "Error", "Invalid phone number.");
        return false;
    }
    if(UserManager::usernameExistsExceptCurrent(username)){
        CustomMessageBox::warning( this, "Error", "Username already exists.");
        return false;
    }

    if(UserManager::phoneExistsExceptCurrent(phone)) {
        CustomMessageBox::warning( this, "Error", "Phone number already exists.");
        return false;
    }

    if(UserManager::emailExistsExceptCurrent(email)) {
        CustomMessageBox::warning( this, "Error", "Email already exists.");
        return false;
    }
   return true;
}

void EditProfileWindow::on_saveButton_clicked() {
    if(!validateInput()){
        return;
    }
    User updatedUser;
    updatedUser.name = ui->nameEdit->text().trimmed();
    updatedUser.username =  ui->usernameEdit->text().trimmed();
    updatedUser.phone =ui->phoneEdit->text().trimmed();
    updatedUser.email = ui->emailEdit->text().trimmed();
    updatedUser.passwordHash = Security::hashPassword( ui->passwordEdit->text());

    if(UserManager::updateUser(updatedUser)){
        CustomMessageBox::information(  this, "Success", "Profile updated successfully.");
        MainWindow *main =  new MainWindow();
        main->show();
        close();
    }
    else {
        CustomMessageBox::warning( this, "Error", "Unable to update profile.");
    }
}

void EditProfileWindow::on_backButton_clicked(){
    MainWindow *main = new MainWindow();
    main->show();
    close();
}

void EditProfileWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if(videoBackground)
        videoBackground->setGeometry(rect());
}