QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chesspiece.cpp \
    connectserverwindow.cpp \
    constants.cpp \
    createconnectionwindow.cpp \
    endgame.cpp \
    game.cpp \
    gameconnection.cpp \
    gamelogic.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    chesspiece.h \
    connectserverwindow.h \
    constants.h \
    createconnectionwindow.h \
    endgame.h \
    game.h \
    gameconnection.h \
    gamelogic.h \
    mainwindow.h

FORMS += \
    connectserverwindow.ui \
    createconnectionwindow.ui \
    endgame.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
