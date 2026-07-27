#include "widget.h"
#include <QApplication>
#include "usermanager.h"
#include <QFile>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UserManager::load();
    Widget w;
    w.show();

    return a.exec();
}
