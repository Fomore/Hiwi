#include "verhaltentime.h"

VerhaltenTime::VerhaltenTime()
{
    mName = mBeschreibung = "Nicht definiert!";

    mEyeVontact = mActiveParticipation = mOtherActivities = mRestlessness = mCommunication = false;

    mFrameStart = mFrameEnd = mObjectID = -1;
}

VerhaltenTime::VerhaltenTime(int Oid, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm, int start, int end)
{
    change(Oid, name,description, eye, activ, other, less, comm, start, end);
}

void VerhaltenTime::change(int O_id, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm, int start, int end)
{
    Event::change(name,description,eye,activ,other, less,comm);
    mFrameStart = start;
    mFrameEnd = end;
    mObjectID = O_id;
}

void VerhaltenTime::setStartEnd(int startTime, int endTime)
{
    mFrameStart = startTime;
    mFrameEnd = endTime;
}

void VerhaltenTime::getFrame(int &start, int &end)
{
    start = mFrameStart;
    end = mFrameEnd;
}

int VerhaltenTime::getObjectID()
{
    return mObjectID;
}
