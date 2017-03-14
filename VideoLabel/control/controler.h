#ifndef CONTROLER_H
#define CONTROLER_H

#include <iostream>
#include <vector>
#include <list>
#include "model/activmodel.h"
#include "model/frame.h"
#include <QRect>
#include <opencv2/opencv.hpp>
#include "control/myvideoplayer.h"

class Controler
{
private:
//    std::vector<std::vector<ActivModel>> mActivModel;
    std::vector<Frame> mFrames;

    int VideoWidth, VideoHeight;
    int DisplayWidth, DisplayHeight;
    int ShiftX, ShiftY;

    double mScall;

    void calculateParameter();

public:
    Controler();
    void addEvent(int x1, int y1, int x2, int y2, int frameNr, int E_id, int O_id);
    int addObjectInFrame(int x, int y, int w, int h, int frameNr, int E_id, int O_id, bool man);

    void save(QString name, QString path);

    void clearAll();
    void setObject(int frameNr, int lastO_id, int newO_id);
    void setAllObject(int lastO_id, int newO_id);
    void setEvent(int frame, int O_id, int E_id);
    void setLandmarks(int pos, int O_id, double marks[5][2]);
    void setOrientation(int pos, int O_id, double ori[3]);
    void setPosition(int frame_pos, int O_id, double pos[3]);
    void setProjection(int pos, int O_id, double pro[4]);

    void setVideoSize(int w, int h);
    void setDisplaySize(int w, int h);

    bool getNextSetFrame(int &frame);
    int getEventToObject(int frame, int O_id);
    ActivModel getActivModel(size_t frame_pos, size_t O_pos);
    int getLastLabel(int O_id);
    int getLastFrame(int O_id);
    int getObjectSizeInFramePos(int frame_pos);
    int getFramePosInVector(int frameNr);
    size_t getFrameNr(size_t frame_pos);

    std::vector<cv::Point3i> getAllActivModel(int O_id);

    bool isEventUsed(int id);
    bool isObjectUsed(int id);

    void deleteEvent(int id);
    void deleteObject(int id);
    void deleteActionEvent(size_t obj_pos, size_t frm_pos);

    void changeActionEventValue(int frmPos, int objPos, int eveID, int x, int y, int w, int h);
    int copyActionEvent(int frmPos, int objPos, int frame);

    void printAll();

    void WindoRectToVideoRect(int &x, int &y, int &w, int &h);

    void detectDataError(int obj_ID, QWidget *parent, MyVideoPlayer *player);
    int samePerson(size_t frame_l, cv::Rect box_l, size_t frame_r, cv::Rect box_r, QWidget *parent, MyVideoPlayer *player);
};

#endif // CONTROLER_H
