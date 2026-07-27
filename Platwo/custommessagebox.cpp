#include "custommessagebox.h"
#include <QMessageBox>

void CustomMessageBox::warning(QWidget *parent,const QString &title, const QString &text){
    QMessageBox msg(parent);
    msg.setIcon(QMessageBox::Warning);
    msg.setWindowTitle(title);
    msg.setText(text);
    msg.setStyleSheet(R"(

QMessageBox{
    background-color:#000000;
}

QLabel{
    background:transparent;
    font-family:"Kristen ITC";
}

/* عنوان پنجره */
QLabel#qt_msgbox_label{
    color:#61D8FF;
    font-size:15pt;
    font-weight:bold;
}

/* متن پیام */
QLabel#qt_msgboxex_icon_label{
    background:transparent;
}

QPushButton{
    background-color:#3A86FF;
    color:white;
    border:2px solid #61D8FF;
    border-radius:10px;
    padding:6px;
    min-width:90px;
    font-family:"Kristen ITC";
    font-size:11pt;
}

QPushButton:hover{
    background-color:#5A9CFF;
    border:2px solid #8EEBFF;
}

QPushButton:pressed{
    background-color:#008BFF;
}

)");

    msg.exec();
}

void CustomMessageBox::information(QWidget *parent, const QString &title, const QString &text){
    QMessageBox msg(parent);
    msg.setIcon(QMessageBox::Information);
    msg.setWindowTitle(title);
    msg.setText(text);
    msg.setStyleSheet(R"(

QMessageBox{
    background-color:#000000;
}

QLabel{
    background:transparent;
    font-family:"Kristen ITC";
}

/* عنوان پنجره */
QLabel#qt_msgbox_label{
    color:#61D8FF;
    font-size:15pt;
    font-weight:bold;
}

/* متن پیام */
QLabel#qt_msgboxex_icon_label{
    background:transparent;
}

QPushButton{
    background-color:#3A86FF;
    color:white;
    border:2px solid #61D8FF;
    border-radius:10px;
    padding:6px;
    min-width:90px;
    font-family:"Kristen ITC";
    font-size:11pt;
}

QPushButton:hover{
    background-color:#5A9CFF;
    border:2px solid #8EEBFF;
}

QPushButton:pressed{
    background-color:#008BFF;
}

)");
    msg.exec();
}

void CustomMessageBox::question(QWidget *parent, const QString &title, const QString &text){
    QMessageBox msg(parent);
    msg.setIcon(QMessageBox::Question);
    msg.setWindowTitle(title);
    msg.setText(text);
    msg.setStyleSheet(R"(

QMessageBox{
    background-color:#000000;
}

QLabel{
    background:transparent;
    font-family:"Kristen ITC";
}

/* عنوان پنجره */
QLabel#qt_msgbox_label{
    color:#61D8FF;
    font-size:15pt;
    font-weight:bold;
}

/* متن پیام */
QLabel#qt_msgboxex_icon_label{
    background:transparent;
}

QPushButton{
    background-color:#3A86FF;
    color:white;
    border:2px solid #61D8FF;
    border-radius:10px;
    padding:6px;
    min-width:90px;
    font-family:"Kristen ITC";
    font-size:11pt;
}

QPushButton:hover{
    background-color:#5A9CFF;
    border:2px solid #8EEBFF;
}

QPushButton:pressed{
    background-color:#008BFF;
}

)");
    msg.exec();
}