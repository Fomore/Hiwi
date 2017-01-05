#ifndef ACTIVMODEL_H
#define ACTIVMODEL_H

#include <QString>

class ActivModel
{
private:
    QString toStr(int i);
    QString toStr(double i);
    virtual void setZero();

public:
    int mID, mEventID, mObjectID;
    int mX,mY,mW,mH;
    int mTimePos;
    double mLandmarks[5][2];
    double mOrienation[3];
    double mPosition[3];
    double mProjection[4];
    bool mManuel, mSetLandmarks, mSetOrienation, mSetPosition, mSetProjection;
    ActivModel();
    ActivModel(QString data);
    ActivModel(ActivModel *p);
    ActivModel(int x, int y, int w, int h, int frame, int E_id, int O_id, int id, bool man);

    virtual void setLandmarks(double marks[5][2]);
    virtual void setOrientation(double orien[3]);
    virtual void setPosition(double pos[3]);
    virtual void setProjection(double proj[4]);
    void setAll(int x, int y, int w, int h, int frame, int E_id, int O_id, int id, bool man);

    QString getDateAll();
    QString printAll();
    int getFrame();
};

#endif // ACTIVMODEL_H
