#ifndef VERHALTENTIME_H
#define VERHALTENTIME_H

#include "event.h"

class VerhaltenTime : public Event
{
private:
    int mFrameStart, mFrameEnd;
    int mObjectID;
public:
    VerhaltenTime();
    VerhaltenTime(int Oid, QString name, QString description,
                  bool eye, bool activ, bool other, bool less, bool comm,
                  int start, int end);

    virtual void change(int O_id, QString name, QString description,
                        bool eye, bool activ, bool other, bool less, bool comm,
                        int start, int end);

    void setStartEnd(int startTime, int endTime);
    void getFrame(int &start, int &end);
    int getObjectID();
};

#endif // VERHALTENTIME_H
