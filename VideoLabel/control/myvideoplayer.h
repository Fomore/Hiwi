#ifndef MYVIDEOPLAYER_H
#define MYVIDEOPLAYER_H

#include <QtCore>
#include <QThread>
#include <QLabel>
#include <QImage>
#include "mywidget.h"

#include <opencv2/opencv.hpp>
#include <iostream>

class MyVideoPlayer : public QThread
{
    Q_OBJECT
private:
    bool mStop;
    bool mIsNewPosition;
    double mNewPosition;

    double mLastPos;
    double frameRate;
    std::string path;
    cv::VideoCapture video;
    void run();
    void showImage(cv::Mat image);
    QLabel *mLabel;
    MyWidget *mWg;
    QImage MatToQImage(const cv::Mat &mat);
public:
    MyVideoPlayer(MyWidget *wg= 0,QLabel *parent = 0);
    bool setPath(QString path);
public slots:
    void play();
    void pause();
    void stop();

    void forward();
    void backward();
    void skipForward();
    void skipBackward();

    void setPosition(double pos);
signals:
    void legthChanged(double length);
    void positionChanger(double pos);
//    void isNewImage(const QImage &image);
};

#endif // MYVIDEOPLAYER_H
