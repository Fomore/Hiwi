#include "myvideoplayer.h"

#include <time.h>
#include <QPixmap>
#include <stdlib.h>

MyVideoPlayer::MyVideoPlayer(MyWidget *wg, QLabel *parent) :
    QThread(),
    mLabel(parent), mWg(wg)
{
    video = cv::VideoCapture("/home/falko/Videos/Chor_01.mp4");
    frameRate = video.get(CV_CAP_PROP_FPS);
    emit legthChanged((int)video.get(CV_CAP_PROP_FRAME_COUNT));

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
    }
    return video.isOpened();
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
    setPosition(video.get(CV_CAP_PROP_POS_FRAMES));
}

void MyVideoPlayer::backward()
{
    double pos = video.get(CV_CAP_PROP_POS_FRAMES);
    setPosition(std::max(-1.0, pos-2));
}

void MyVideoPlayer::skipForward()
{
    double pos = video.get(CV_CAP_PROP_POS_FRAMES);
    double mx = video.get(CV_CAP_PROP_FRAME_COUNT);
    setPosition(std::min(mx, pos+frameRate*10.0));
}

void MyVideoPlayer::skipBackward()
{
    double pos = video.get(CV_CAP_PROP_POS_FRAMES);
    setPosition(std::max(-1.0, pos-frameRate*10.0-2));
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
    QImage img2 = img.scaled(mLabel->size().width(),mLabel->size().height(),Qt::KeepAspectRatio);
    mLabel->setPixmap(QPixmap::fromImage(img2));
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
