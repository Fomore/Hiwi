#ifndef ACTIVMODEL_H
#define ACTIVMODEL_H

#include <QString>

class ActivModel
{
private:
    QString toInt(int i);
public:
    int mID, mEventID, mObjectID;
    int mX,mY,mW,mH;
    int mTimePos;
    ActivModel();
    ActivModel(QString data);
    ActivModel(int x, int y, int w, int h, int frame, int E_id, int O_id, int id);
    int getFrame();
    void setAll(int x, int y, int w, int h, int frame, int E_id, int O_id, int id);
    QString getDateAll();
};

#endif // ACTIVMODEL_H
