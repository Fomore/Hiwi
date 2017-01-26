#ifndef ACTIVMODEL_H
#define ACTIVMODEL_H

#include <QString>

class ActivModel
{
private:
    int mEventID, mObjectID;
    int mX,mY,mW,mH;
    int mTimePos;

    QString toStr(int i);
    QString toStr(double i);
    virtual void setZero();

public:
    double mLandmarks[5][2];
    double mOrienation[3];
    double mPosition[3];
    double mProjection[4];
    bool mManuel, mSetLandmarks, mSetOrienation, mSetPosition, mSetProjection;

    ActivModel();
    ActivModel(ActivModel *p);
    ActivModel(int x, int y, int w, int h, int frame, int E_id, int O_id, bool man);

    void setAll(int x, int y, int w, int h, int frame, int E_id, int O_id, bool man);
    void setEventID(int E_id);
    void setObjectID(int O_id);
    virtual void setLandmarks(double marks[5][2]);
    virtual void setOrientation(double orien[3]);
    virtual void setPosition(double pos[3]);
    virtual void setProjection(double proj[4]);
    virtual void setRect(int x, int y, int w, int h);

    int getFrame();
    int getEventID();
    int getObjectID();
    void getRect(int &x, int &y, int &w, int &h);

    QString printAll();
};

#endif // ACTIVMODEL_H
