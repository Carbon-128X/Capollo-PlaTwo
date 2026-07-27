QT += widgets
QT += multimedia multimediawidgets
QT += network
CONFIG += c++17
CONFIG += resources_big

INCLUDEPATH += $$PWD/network
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boardwidget.cpp \
    boxesboardwindow.cpp \
    custommessagebox.cpp \
    editprofilewindow.cpp \
    fanoronaboardwidget.cpp \
    fanoronaboardwindow.cpp \
    filemanager.cpp \
    forgotpasswordwindow.cpp \
    network/gameSession.cpp \
    gamehistory.cpp \
    gamewindow.cpp \
    guestwindow.cpp \
    hostwindow.cpp \
    logic/dotsAndBoxes.cpp \
    logic/nineMensMorris.cpp \
    loginwindow.cpp \
    logic/fanorona.cpp \
    main.cpp \
    mainwindow.cpp \
    morrisboardwidget.cpp \
    morrisboardwindow.cpp \
    network/networkGame.cpp \
    networkclient.cpp \
    networkserver.cpp \
    network/protocol.cpp \
    security.cpp \
    signupwindow.cpp \
    user.cpp \
    usermanager.cpp \
    videobackgroundwidget.cpp \
    welcomewindow.cpp \
    widget.cpp \


HEADERS += \
    boardwidget.h \
    boxesboardwindow.h \
    custommessagebox.h \
    editprofilewindow.h \
    fanoronaboardwidget.h \
    fanoronaboardwindow.h \
    filemanager.h \
    forgotpasswordwindow.h \
    network/gameSession.h \
    gamehistory.h \
    gamewindow.h \
    guestwindow.h \
    hostwindow.h \
    logic/dotsAndBoxes.h \
    logic/gameMediator.h \
    logic/move.h \
    logic/nineMensMorris.h \
    loginwindow.h \
    mainwindow.h \
    morrisboardwidget.h \
    morrisboardwindow.h \
    network/networkGame.h \
    networkclient.h \
    networkserver.h \
    network/protocol.h \
    security.h \
    signupwindow.h \
    user.h \
    usermanager.h \
    videobackgroundwidget.h \
    welcomewindow.h \
    widget.h \
    logic/fanorona.h \

FORMS += \
    boxesboardwindow.ui \
    editprofilewindow.ui \
    fanoronaboardwindow.ui \
    forgotpasswordwindow.ui \
    gamewindow.ui \
    guestwindow.ui \
    hostwindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    morrisboardwindow.ui \
    signupwindow.ui \
    welcomewindow.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
