# -------------------------------------------------
# Project created by QtCreator 2010-01-07T07:26:54
# -------------------------------------------------
TARGET = qOthello
TEMPLATE = app
QT += network widgets
SOURCES += main.cpp \
    mainwindow.cpp \
    pionitem.cpp \
    rollitwidget.cpp \
    pieceslist.cpp \
    rollitia.cpp \
    square.cpp
#    dialognetwork.cpp
HEADERS += mainwindow.h \
    pionitem.h \
    rollitwidget.h \
    pieceslist.h \
    rollitia.h \
    square.h
#    dialognetwork.h
FORMS += mainwindow.ui \
    dialognetwork.ui

RESOURCES += \
    qrollit.qrc
