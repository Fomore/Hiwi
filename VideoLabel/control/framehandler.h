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

    int addObjectInFrame(int x, int y, int w, int h, size_t frameNr, int E_id, int O_id, bool man);

    void setObject(size_t frameNr, int lastO_id, int newO_id);
    void setAllObject(int lastO_id, int newO_id);
    void setEvent(size_t frameNr, int O_pos, int E_id);
    void setLandmarks(size_t pos, int O_id, double marks[5][2]);
    void setOrientation(size_t pos, int O_id, double ori[3]);
    void setPosition(size_t frame_pos, int O_id, double pos[3]);
    void setProjection(size_t pos, int O_id, double pro[4]);

    int getEventToObject(size_t frameNr, int O_id);
    ActivModel getActivModel(size_t frame_pos, size_t O_pos);
    size_t getLastLabel(int O_id);
    int getLastFrame(int O_id);
    int getObjectSizeInFramePos(size_t frame_pos);
    int getFramePosInVector(size_t frameNr);
    size_t getFrameNr(size_t frame_pos);
    size_t getFrameSize();

    std::vector<cv::Point3i> getAllActivModel(int O_id);

    bool isEventUsed(int id);
    bool isObjectUsed(int id);

    void deleteEventID(int id);
    void deleteObjectID(int id);
    void deleteActionEvent(size_t frm_pos, size_t obj_pos);

    void changeActionEventValue(size_t frmPos, size_t objPos, int eveID, int x, int y, int w, int h);
    int copyActionEvent(size_t frmPos, size_t objPos, size_t frameNr);

};

#endif // FRAMEHANDLER_H
