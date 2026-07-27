#include "widget.h"
#include <QApplication>
#include <QCoreApplication>
#include "usermanager.h"
#include <QFile>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Capollo");
    QCoreApplication::setApplicationName("Platwo");

    UserManager::load();
    Widget w;
    w.show();

    return a.exec();
}
