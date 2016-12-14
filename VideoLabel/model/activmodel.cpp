#include "activmodel.h"

ActivModel::ActivModel()
{
}

ActivModel::ActivModel(int x, int y, int w, int h, int frame, int E_id, int O_iD, int id)
{
    mX = x;
    mY = y;
    mW = w;
    mH = h;

    mTimePos = frame;

    mEventID = E_id;
    mObjectID = O_iD;
    mID = id;
}

int ActivModel::getFrame(){
    return mTimePos;
}
