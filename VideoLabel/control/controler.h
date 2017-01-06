#ifndef CONTROLER_H
#define CONTROLER_H

#include <iostream>
#include <vector>
#include "model/activmodel.h"
#include <QRect>

class Controler
{
private:
    std::vector<std::vector<ActivModel>> mActivModel;

    int VideoWidth, VideoHeight;
    int DisplayWidth, DisplayHeight;
    int ShiftX, ShiftY;

    double mScall;

    void calculateParameter();

    int getObjectPosInVector(int frame, int O_id);
    int getFramePosInVector(int frame);

public:
    Controler();
    void addEvent(int x1, int y1, int x2, int y2, int frame, int E_id, int O_id);
    int addEventInFrame(int x, int y, int w, int h, int frame, int E_id, int O_id, bool man);

    void save(QString name, QString path);

    void clearAll();

    void setObject(int frame, int lastO_id, int newO_id);
    void setEvent(int frame, int O_id, int E_id);
    void setLandmarks(int pos, int O_id, double marks[5][2]);
    void setOrientation(int pos, int O_id, double ori[3]);
    void setPosition(int frame_pos, int O_id, double pos[3]);
    void setProjection(int pos, int O_id, double pro[4]);

    void setVideoSize(int w, int h);
    void setDisplaySize(int w, int h);

    bool getNextSetFrame(int &frame);
    int getEventToObject(int frame, int O_id);
    QRect getRect(int frame, int O_id, int &E_id);
    ActivModel getActivModel(int frame, int O_id);


    bool isEventUsed(int id);
    bool isObjectUsed(int id);

    void deleteEvent(int id);
    void deleteObject(int id);
    void deleteActionEvent(int O_id, int frame);

    void printAll();
};

#endif // CONTROLER_H
