#include "activmodel.h"

#include <iostream>
#include <QStringList>

ActivModel::ActivModel()
{
    mObjectID = mEventID = mX = mY = mW = mH = -1;
    mManuel = false;
    setZero();
}

ActivModel::ActivModel(ActivModel *p)
{
    mObjectID = p->mObjectID;
    mEventID = p->mEventID;
    mX = p->mX;
    mY = p->mY;
    mW = p->mW;
    mH = p->mH;
    mManuel = p->mManuel;
    setLandmarks(p->mLandmarks);
    setOrientation(p->mOrienation);
    setPosition(p->mPosition);
    setProjection(p->mProjection);
    mSetLandmarks = p->mSetLandmarks;
    mSetOrienation = p->mSetOrienation;
    mSetPosition = p->mSetPosition;
    mSetProjection = p->mSetProjection;
    mGaze = p->mGaze;
}

ActivModel::ActivModel(int x, int y, int w, int h, int frame, int E_id, int O_id, bool man, int gaze)
{
    setZero();
    setAll(x,y,w,h,frame,E_id,O_id, man, gaze);
}

void ActivModel::setZero()
{
    double zero3[3] = {0,0,0};
    double zero4[4] = {0,0,0,0};
    double zero5[5][2] = {{0,0},{0,0},{0,0},{0,0},{0,0}};
    setLandmarks(zero5);
    setOrientation(zero3);
    setPosition(zero3);
    setProjection(zero4);
    mSetLandmarks = mSetOrienation = mSetPosition = mSetProjection = false;
    mGaze = 0;
}

void ActivModel::setLandmarks(double marks[5][2])
{
    mSetLandmarks = true;
    for(int i = 0; i < 5; i++){
        mLandmarks[i][0] = marks[i][0];
        mLandmarks[i][1] = marks[i][1];
    }
}

void ActivModel::setOrientation(double orien[3])
{
    mSetOrienation = true;
    mOrienation[0]=orien[0];
    mOrienation[1]=orien[1];
    mOrienation[2]=orien[2];
}

void ActivModel::setPosition(double pos[3])
{
    mSetPosition = true;
    mPosition[0]=pos[0];
    mPosition[1]=pos[1];
    mPosition[2]=pos[2];
}

void ActivModel::setProjection(double proj[4])
{
    mSetProjection = true;
    mProjection[0]=proj[0];
    mProjection[1]=proj[1];
    mProjection[2]=proj[2];
    mProjection[3]=proj[3];
}

void ActivModel::setRect(int x, int y, int w, int h)
{
    mX = x;
    mY = y;
    mW = w;
    mH = h;
}

int ActivModel::getEventID()
{
    return mEventID;
}

int ActivModel::getObjectID()
{
    return mObjectID;
}

void ActivModel::getRect(int &x, int &y, int &w, int &h)
{
    x = mX;
    y = mY;
    w = mW;
    h = mH;
}

int ActivModel::getGaze()
{
    return mGaze;
}

void ActivModel::setAll(int x, int y, int w, int h, int frame, int E_id, int O_id, bool man, int gaze)
{
    mX = x;
    mY = y;
    mW = w;
    mH = h;

    mEventID = E_id;
    mObjectID = O_id;

    mManuel = man;

    mGaze = gaze;
}

void ActivModel::setEventID(int E_id)
{
    mEventID = E_id;
}

void ActivModel::setObjectID(int O_id)
{
    mObjectID = O_id;
}

QString ActivModel::toStr(int i)
{
    return QString::number(i);
}
QString ActivModel::toStr(double i){
    return QString::number(i,'g',15);
}

QString ActivModel::printAll()
{
    QString ret = "["+toStr(mObjectID)+","+toStr(mEventID)+"] ["+toStr(mX)+", "+toStr(mY)+", "+toStr(mW)+", "+toStr(mH)+"] ";
    if(mSetOrienation){
        ret += ", Ori ["+toStr(mOrienation[0])+","+toStr(mOrienation[1])+", "+toStr(mOrienation[2])+"]";
    }
    if(mSetPosition){
        ret += ", Pos ["+toStr(mPosition[0])+", "+toStr(mPosition[1])+", "+toStr(mPosition[2])+"]";
    }
    if(mSetProjection){
        ret += ", Poj ["+toStr(mProjection[0])+", "+toStr(mProjection[1])+", "+toStr(mProjection[2])+", "+toStr(mProjection[3])+"]";
    }
    if(mSetLandmarks){
        ret += "Land: ["+toStr(mLandmarks[0][0])+", "+toStr(mLandmarks[0][1])+" | "
                +toStr(mLandmarks[1][0])+", "+toStr(mLandmarks[1][1])+" | "
                +toStr(mLandmarks[2][0])+", "+toStr(mLandmarks[2][1])+" | "
                +toStr(mLandmarks[3][0])+", "+toStr(mLandmarks[3][1])+" | "
                +toStr(mLandmarks[4][0])+", "+toStr(mLandmarks[4][1])+"]";
    }
    return ret;
}
