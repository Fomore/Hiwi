#-------------------------------------------------
#
# Project created by QtCreator 2016-12-06T13:27:06
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VideoLabel
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    model/event.cpp \
    model/object.cpp \
    model/activmodel.cpp \
    model/verhaltentime.cpp \
    control/controler.cpp \
    control/xmlloader.cpp \
    control/myvideoplayer.cpp \
    control/loader.cpp \
    mylabel.cpp \
    eventdialog.cpp \
    objectdialog.cpp \
    actioneventdialog.cpp \
    behaviordialog.cpp

HEADERS  += mainwindow.h \
    control/loader.h \
    model/event.h \
    model/object.h \
    model/activmodel.h \
    control/controler.h \
    control/xmlloader.h \
    control/myvideoplayer.h \
    mylabel.h \
    eventdialog.h \
    objectdialog.h \
    actioneventdialog.h \
    behaviordialog.h \
    model/verhaltentime.h

FORMS    += mainwindow.ui \
    eventdialog.ui \
    objectdialog.ui \
    actioneventdialog.ui \
    behaviordialog.ui

RESOURCES += \
    Resources.qrc

PKGCONFIG += opencv

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_viz -lopencv_core
