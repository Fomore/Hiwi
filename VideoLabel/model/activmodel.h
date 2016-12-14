#ifndef ACTIVMODEL_H
#define ACTIVMODEL_H

class ActivModel
{
public:
    int mID, mEventID, mObjectID;
    int mX,mY,mW,mH;
    int mTimePos;
    ActivModel();
    ActivModel(int x, int y, int w, int h, int frame, int E_id, int O_iD, int id);
    int getFrame();
};

#endif // ACTIVMODEL_H
