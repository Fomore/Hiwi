#ifndef ACTIVMODEL_H
#define ACTIVMODEL_H

#include <QString>

class ActivModel
{
private:
    QString toStr(int i);
    QString toStr(double i);
    void setZero();
public:
    int mID, mEventID, mObjectID;
    int mX,mY,mW,mH;
    int mTimePos;
    double mLandmarks[5][2];
    double mOrienation[3];
    double mPosition[3];
    double mProjection[4];
    ActivModel();
    ActivModel(QString data);
    ActivModel(int x, int y, int w, int h, int frame, int E_id, int O_id, int id);
    void setLandmarks(double marks[5][2]);
    void setOrientation(double orien[3]);
    void setPosition(double pos[3]);
    void setProjection(double proj[4]);
    int getFrame();
    void setAll(int x, int y, int w, int h, int frame, int E_id, int O_id, int id);
    QString getDateAll();
};

#endif // ACTIVMODEL_H
