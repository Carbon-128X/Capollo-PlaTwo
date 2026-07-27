#ifndef CUSTOMMESSAGEBOX_H
#define CUSTOMMESSAGEBOX_H
#include <QWidget>

class CustomMessageBox{
public:
    static void warning(QWidget *parent,const QString &title,const QString &text);
    static void question(QWidget *parent, const QString &title, const QString &text);
    static void information(QWidget *parent, const QString &title, const QString &text);
};

#endif // CUSTOMMESSAGEBOX_H