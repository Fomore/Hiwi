#ifndef MYVIDEOPLAYER_H
#define MYVIDEOPLAYER_H

#include <QtCore>
#include <QThread>
#include <QImage>

#include <opencv2/opencv.hpp>
#include <iostream>

class MyVideoPlayer : public QThread
{
    Q_OBJECT
private:
    bool mStop;
    bool mIsNewPosition;
    double mNewPosition;

//    std::vector<cv::String> mImagePaths;
//    int mImagePos;

    double mLastPos;
    double frameRate;
    std::string path;
    cv::VideoCapture video;
    void run();
    void showImage(cv::Mat image, bool showPos);
    QImage MatToQImage(const cv::Mat &mat);
public:
    MyVideoPlayer();
    bool setPath(QString path);
    double getPosition();

    double getVideoWidth();
    double getVideoHeight();

    double SecToFrame(int sec);

    void getFrame();
    QImage getFrame(size_t frameNr);

public slots:
    void play();
    void pause();
    void stop();

    void forward();
    void backward();
    void skipForward();
    void skipBackward();

    void setPosition(double pos, bool show);
    void setPosition(int pos);
signals:
    void legthChanged(const int &length);
    void positionChanger(const int &pos);
    void isNewImage(const QImage &image);
};

#endif // MYVIDEOPLAYER_H
