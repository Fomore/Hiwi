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

    int toVideoX(int x);
    int toVideoY(int y);
    int toDisplayX(int x);
    int toDisplayY(int y);
public:
    Controler();
    void addEvent(int x1, int y1, int x2, int y2, int frame, int E_id, int O_iD);
    void addEventInFrame(int x, int y, int w, int h, int frame, int E_id, int O_iD);
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
};

#endif // CONTROLER_H
