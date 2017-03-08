#-------------------------------------------------
#
# Project created by QtCreator 2017-03-08T16:17:25
#
#-------------------------------------------------

QT       += core gui xml

QT       -= gui

TARGET = SortEvent
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += main.cpp \
    model/event.cpp \
    control/xmlloader.cpp

HEADERS += \
    model/event.h \
    control/xmlloader.h
