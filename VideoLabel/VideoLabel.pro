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
    control/xmlloader.cpp \
    control/myvideoplayer.cpp

HEADERS  += mainwindow.h \
    control/loader.h \
    model/event.h \
    model/object.h \
    model/activmodel.h \
    mywidget.h \
    control/controler.h \
    inputevobdialog.h \
    control/xmlloader.h \
    control/myvideoplayer.h

FORMS    += mainwindow.ui \
    inputevobdialog.ui

RESOURCES += \
    Resources.qrc

PKGCONFIG += opencv

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_viz -lopencv_core
