#ifndef CONTROLER_H
#define CONTROLER_H

#include <iostream>
#include <vector>
#include "model/activmodel.h"
#include <QRect>

class Controler
{
private:
     std::vector<ActivModel> mEvents;

     int VideoWidth, VideoHeight;
     int DisplayWidth, DisplayHeight;

     double getScall();

     int getLastID(int frame);
     int getNextID(int frame);
public:
    Controler();
    void addEvent(int x1, int y1, int x2, int y2, int frame, int E_id, int O_iD);
    void setVideoSize(int w, int h);
    void setDisplaySize(int w, int h);
    QRect getRect(int frame);
};

#endif // CONTROLER_H
