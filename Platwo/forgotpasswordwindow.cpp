#include "forgotpasswordwindow.h"
#include "ui_forgotpasswordwindow.h"
#include "loginwindow.h"
#include "usermanager.h"
#include "security.h"
#include <QMessageBox>
#include <QRegularExpression>
#include "custommessagebox.h"
ForgotPasswordWindow::ForgotPasswordWindow(QWidget *parent): QWidget(parent), ui(new Ui::ForgotPasswordWindow){
    ui->setupUi(this);

    videoBackground = new VideoBackgroundWidget(this);
    videoBackground->setGeometry(rect());
    videoBackground->lower();
    videoBackground->setVideo(":/images/images/20.mp4");

}

ForgotPasswordWindow::~ForgotPasswordWindow(){
    delete ui;
}
void ForgotPasswordWindow::on_backButton_clicked() {
    LoginWindow *login = new LoginWindow();

    login->show();

    this->close();
}


void ForgotPasswordWindow::on_changePasswordButton_clicked()
{
    QString phone = ui->phoneEdit->text().trimmed();
    QString password = ui->newPasswordEdit->text();
    QString confirm = ui->confirmPasswordEdit->text();

    if(phone.isEmpty() || password.isEmpty() || confirm.isEmpty()){
        CustomMessageBox::warning(this, "Error", "Please fill all fields.");
        return;
    }

    if(password != confirm){
        CustomMessageBox::warning(this, "Error", "Passwords do not match.");
        return;
    }

    // اینجا شماره تلفن را چک میکنم
    QRegularExpression phoneRegex("^09\\d{9}$");

    if(!phoneRegex.match(phone).hasMatch()){
        CustomMessageBox::warning(this, "Error", "Invalid phone number.");
        return;
    }

    // رمز باید قوی باشد پس من هم کاراکتر ها و ععداشو چک میکنم
    QRegularExpression passwordRegex(
        "^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&]).{8,}$");

    if(!passwordRegex.match(password).hasMatch()){
        CustomMessageBox::warning(this,"Weak Password","Password must contain:\n""- At least 8 characters\n""- One uppercase letter\n"
            "- One lowercase letter\n" "- One number\n" "- One special character");
        return;
    }

    QString hash = Security::hashPassword(password);

    if(UserManager::changePassword(phone, hash)){
        CustomMessageBox::information(this, "Success", "Password changed successfully.");

        LoginWindow *login = new LoginWindow();
        login->show();

        this->close();
    }
    else{
        CustomMessageBox::warning(this, "Error", "Phone number not found.");
    }
}

void ForgotPasswordWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);

    if(videoBackground)
        videoBackground->setGeometry(rect());
}