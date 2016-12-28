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
    std::vector<std::vector<ActivModel>> mActionEvents;

    int VideoWidth, VideoHeight;
    int DisplayWidth, DisplayHeight;

    double mScall;

    double getScall();
    void calculateParameter();

    int getPosition(int frame, int O_id);
    void addEventVector();

public:
    Controler();
    void addEvent(int x1, int y1, int x2, int y2, int frame, int E_id, int O_iD);
    int addEventInFrame(int x, int y, int w, int h, int frame, int E_id, int O_iD, bool man);

    void loadFromFile(QString name, QString path);

    void save(QString name, QString path);

    void clearAll();

    void setLandmarks(int O_id, int E_id, double marks[5][2]);
    void setOrientation(int O_id, int E_id, double ori[3]);
    void setPosition(int O_id, int E_id, double pos[3]);
    void setProjection(int O_id, int E_id, double pro[4]);
    void setVideoSize(int w, int h);
    void setDisplaySize(int w, int h);
    void setObjectSize(int anz);

    bool getNextSetFrame(int &frame);
    int getEventToObject(int frame, int O_id);
    QRect getRect(int frame, int O_id, int &E_id);
};

#endif // CONTROLER_H
