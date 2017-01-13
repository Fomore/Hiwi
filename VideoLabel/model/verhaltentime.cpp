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

void VerhaltenTime::change(int Oid, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm, int start, int end)
{
    Event::change(name,description,eye,activ,other, less,comm);
    mFrameStart = start;
    mFrameEnd = end;
    mObjectID = Oid;
}

void VerhaltenTime::setStartEnd(int start, int end)
{
    mFrameStart = start;
    mFrameEnd = end;
}
