#ifndef FRAMEHANDLER_H
#define FRAMEHANDLER_H

#include <opencv2/opencv.hpp>

#include "model/frame.h"

class FrameHandler
{
protected:
    std::vector<Frame> mFrames;
public:
    FrameHandler();

    int addObjectInFrame(int x, int y, int w, int h, int frameNr, int E_id, int O_id, bool man);

    void setObject(int frameNr, int lastO_id, int newO_id);
    void setAllObject(int lastO_id, int newO_id);
    void setEvent(int frame, int O_id, int E_id);
    void setLandmarks(int pos, int O_id, double marks[5][2]);
    void setOrientation(int pos, int O_id, double ori[3]);
    void setPosition(int frame_pos, int O_id, double pos[3]);
    void setProjection(int pos, int O_id, double pro[4]);

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

    void deleteEventID(int id);
    void deleteObjectID(int id);
    void deleteActionEvent(size_t obj_pos, size_t frm_pos);

    void changeActionEventValue(int frmPos, int objPos, int eveID, int x, int y, int w, int h);
    int copyActionEvent(int frmPos, int objPos, int frame);

};

#endif // FRAMEHANDLER_H
