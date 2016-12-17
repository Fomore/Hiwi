#-------------------------------------------------
#
# Project created by QtCreator 2016-12-06T13:27:06
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoLabel
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    control/loader.cpp \
    model/event.cpp \
    model/object.cpp \
    model/activmodel.cpp \
    mywidget.cpp \
    control/controler.cpp \
    inputevobdialog.cpp \
    control/xmlloader.cpp

HEADERS  += mainwindow.h \
    control/loader.h \
    model/event.h \
    model/object.h \
    model/activmodel.h \
    mywidget.h \
    control/controler.h \
    inputevobdialog.h \
    control/xmlloader.h

FORMS    += mainwindow.ui \
    inputevobdialog.ui

RESOURCES += \
    Resources.qrc

