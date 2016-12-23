#ifndef CONTROLER_H
#define CONTROLER_H

#include <iostream>
#include <vector>
#include "model/activmodel.h"
#include <QRect>

class Controler
{
private:
    std::vector<std::vector<ActivModel>> mEvents;

    int VideoWidth, VideoHeight;
    int DisplayWidth, DisplayHeight;

    double mScall;
    int mShiftX, mShiftY;

    double getScall();
    void calculateParameter();

    int getPosition(int frame, int O_id);
    void addEvent();

public:
    Controler();
    void addEvent(int x1, int y1, int x2, int y2, int frame, int E_id, int O_iD);
    int addEventInFrame(int x, int y, int w, int h, int frame, int E_id, int O_iD);
    void setVideoSize(int w, int h);
    void setDisplaySize(int w, int h);
    QRect getRect(int frame, int O_id, int &E_id);
    int getEventToObject(int frame, int O_id);
    void setObjectSize(int anz);
    void loadFromFile(QString name);
    void loadFromFile(QString name, QString path);
    void save(QString name);
    void save(QString name, QString path);
    void clearAll();
    void changeEvent(int frame, int O_id, int E_id);

    void setLandmarks(int O_id, int E_id, double marks[5][2]);
    void setOrientation(int O_id, int E_id, double ori[3]);
    void setPosition(int O_id, int E_id, double pos[3]);
    void setProjection(int O_id, int E_id, double pro[4]);
};

#endif // CONTROLER_H
