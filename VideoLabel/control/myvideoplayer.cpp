#include "myvideoplayer.h"

#include <time.h>
#include <QPixmap>
#include <stdlib.h>

MyVideoPlayer::MyVideoPlayer(MyWidget *wg, QLabel *parent) :
    QThread(),
    mLabel(parent), mWg(wg)
{
    video = cv::VideoCapture("/home/falko/Videos/Chor_01.mp4");
}

bool MyVideoPlayer::setPath(QString path)
{
    video = cv::VideoCapture(path.toStdString());
    if(video.isOpened()){
        emit legthChanged(video.get(CV_CAP_PROP_FRAME_COUNT));
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
    cv::Mat frame;
    if(video.read(frame)){
        showImage(frame);
    }
}

void MyVideoPlayer::backward()
{
    double pos = video.get(CV_CAP_PROP_POS_FRAMES);
    video.set(CV_CAP_PROP_POS_FRAMES,std::max(-1.0, pos-2));
    cv::Mat frame;
    if(video.read(frame)){
        showImage(frame);
    }
}

void MyVideoPlayer::skipForward()
{
    double fps = video.get(CV_CAP_PROP_FPS);
    double pos = video.get(CV_CAP_PROP_POS_FRAMES);
    double mx = video.get(CV_CAP_PROP_FRAME_COUNT);
    video.set(CV_CAP_PROP_POS_FRAMES,std::min(mx, pos+fps*10));
    cv::Mat frame;
    if(video.read(frame)){
        showImage(frame);
    }
}

void MyVideoPlayer::skipBackward()
{
    double fps = video.get(CV_CAP_PROP_FPS);
    double pos = video.get(CV_CAP_PROP_POS_FRAMES);
    video.set(CV_CAP_PROP_POS_FRAMES,std::max(-1.0, pos-fps*10-2));
    cv::Mat frame;
    if(video.read(frame)){
        showImage(frame);
    }
}

void MyVideoPlayer::setPosition(double pos)
{
    video.set(CV_CAP_PROP_POS_FRAMES,std::max(-1.0, pos));
}

void MyVideoPlayer::run()
{
    clock_t wait;
    double fps = video.get(CV_CAP_PROP_FPS);

    cv::Mat frame;
    bool run = video.read(frame);
    while (run && !mStop) {
        wait = clock();
        if(!video.read(frame)){
            break;
        }

        showImage(frame);
        usleep(std::max(0,(int)(CLOCKS_PER_SEC/fps - (clock()-wait))));
    }
}

void MyVideoPlayer::showImage(cv::Mat image)
{
    if(std::abs(video.get(CV_CAP_PROP_POS_FRAMES)-mLastPos) >= std::max(1.0,video.get(CV_CAP_PROP_FRAME_COUNT)/1000.0)){
        emit positionChanger(video.get(CV_CAP_PROP_POS_FRAMES));
    }

    QImage img = MatToQImage(image);
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
