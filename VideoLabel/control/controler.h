#ifndef CONTROLER_H
#define CONTROLER_H

#include <iostream>
#include <vector>
#include <list>
#include <QRect>
#include <opencv2/opencv.hpp>
#include "control/myvideoplayer.h"
#include "control/framehandler.h"
#include "control/objecthandler.h"
#include "control/eventhandler.h"
#include "control/behaviorhandler.h"

class Controler : public FrameHandler, public ObjectHandler, public EventHandler, public BehaviorHandler
{
private:
    int VideoWidth, VideoHeight;
    int DisplayWidth, DisplayHeight;
    int ShiftX, ShiftY;

    double mScall;

    void calculateParameter();

public:
    Controler();

    void save(QString name, QString path);

    void setVideoSize(int w, int h);
    void setDisplaySize(int w, int h);

    void addEvent(int x1, int y1, int x2, int y2, int frameNr, int E_id, int O_id);

    void clearAll();

    void printAll();

    void WindoRectToVideoRect(int &x, int &y, int &w, int &h);

    void detectDataError(int obj_ID, QWidget *parent, MyVideoPlayer *player);
    void detectDataError2(int obj_ID, QWidget *parent, MyVideoPlayer *player);
    int samePerson(size_t frame_l, cv::Rect box_l, size_t frame_r, cv::Rect box_r, QWidget *parent, MyVideoPlayer *player);
    QRect getPrintBox(int Img_width, int Img_height, int X, int Y, int H, int W);
};

#endif // CONTROLER_H
