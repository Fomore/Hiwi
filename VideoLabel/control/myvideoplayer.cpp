#include "myvideoplayer.h"

#include <time.h>
#include <QPixmap>
#include <stdlib.h>

MyVideoPlayer::MyVideoPlayer() :
    QThread()
{
    mStop = true;
    mIsNewPosition = false;
}

bool MyVideoPlayer::setPath(QString path)
{
    video.open(path.toStdString());
    if(video.isOpened()){
        frameRate = video.get(CV_CAP_PROP_FPS);
        emit legthChanged((int)video.get(CV_CAP_PROP_FRAME_COUNT));
        mLastPos = 0;
        return true;
    }else{
        qDebug()<< "Fehler in Video beim öffen von XML: "<<path;
        return false;
    }
}

void MyVideoPlayer::play()
{
    if(video.isOpened()){
        mStop = false;
        start();
    }
}

void MyVideoPlayer::pause()
{
    mStop = true;
}

void MyVideoPlayer::stop()
{
    mStop = true;
    video.set(CV_CAP_PROP_POS_FRAMES,0.0);

}

void MyVideoPlayer::forward()
{
    setPosition(video.get(CV_CAP_PROP_POS_FRAMES),true);
}

void MyVideoPlayer::backward()
{
    double pos = video.get(CV_CAP_PROP_POS_FRAMES);
    setPosition(std::max(-1.0, pos-2),true);
}

void MyVideoPlayer::skipForward()
{
    double pos = video.get(CV_CAP_PROP_POS_FRAMES);
    double mx = video.get(CV_CAP_PROP_FRAME_COUNT);
    setPosition(std::min(mx, pos+frameRate*10.0),true);
}

void MyVideoPlayer::skipBackward()
{
    double pos = video.get(CV_CAP_PROP_POS_FRAMES);
    setPosition(std::max(-1.0, pos-frameRate*10.0-2),true);
}

void MyVideoPlayer::setPosition(double pos, bool show)
{
    pos = std::max(-1.0, std::min(pos,video.get(CV_CAP_PROP_FRAME_COUNT)));
    if(mStop){
        video.set(CV_CAP_PROP_POS_FRAMES,pos);
        cv::Mat frame;
        if(video.read(frame)){
            showImage(frame, show);
        }
    }else{
        mIsNewPosition = true;
        mNewPosition = pos;
    }
}

void MyVideoPlayer::setPosition(int pos)
{
    setPosition((double)pos, false);
}

double MyVideoPlayer::getPosition()
{
    return video.get(CV_CAP_PROP_POS_FRAMES);
}

double MyVideoPlayer::getVideoWidth()
{
    return video.get(CV_CAP_PROP_FRAME_WIDTH);
}

double MyVideoPlayer::getVideoHeight()
{
    return video.get(CV_CAP_PROP_FRAME_HEIGHT);
}

void MyVideoPlayer::getFrame()
{
    cv::Mat frame;
    if(mStop){
        video.read(frame);
        if(!frame.empty()){
            showImage(frame, false);
            video.set(CV_CAP_PROP_POS_FRAMES,video.get(CV_CAP_PROP_POS_FRAMES)-1.0);
        }
    }
}

void MyVideoPlayer::run()
{
    clock_t last = clock();
    cv::Mat frame;
    while (!mStop && video.read(frame)) {
        showImage(frame, true);
        usleep(std::max(0,(int)(CLOCKS_PER_SEC/frameRate - (clock()-last))));
        last=clock();
        if(mIsNewPosition){
            video.set(CV_CAP_PROP_POS_FRAMES,mNewPosition);
            mIsNewPosition = false;
        }
    }
}

void MyVideoPlayer::showImage(cv::Mat image, bool showPos)
{
    if(showPos)
        emit positionChanger((int) video.get(CV_CAP_PROP_POS_FRAMES));

    QImage img = MatToQImage(image);
    emit isNewImage(img);
}

// Diese Methode stammt von http://www.qtcentre.org/threads/56482-efficient-way-to-display-opencv-image-into-Qt
QImage MyVideoPlayer::MatToQImage(const cv::Mat& mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }else if(mat.type()==CV_8UC3){    // 8-bits unsigned, NO. OF CHANNELS=3
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }else{
        //        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
