#include "activmodel.h"

#include <iostream>
#include <QStringList>

ActivModel::ActivModel()
{
}

ActivModel::ActivModel(QString data)
{
    QStringList data_list = data.split("&");

    if(data_list.size() >= 8){
        mID = data_list[0].toInt();
        mObjectID = data_list[1].toInt();
        mEventID = data_list[2].toInt();
        mTimePos = data_list[3].toInt();
        mX = data_list[4].toInt();
        mY = data_list[5].toInt();
        mW = data_list[6].toInt();
        mH = data_list[7].toInt();
        setZero();
        if(data_list.size() == 8){

        }else
            setZero();
    }else{
        std::cout<<"Fehler in Event-Datei: "<<data.toStdString()<<std::endl;
    }
}

QString ActivModel::toStr(int i)
{
    return QString::number(i);
}
QString ActivModel::toStr(double i){
    return QString::number(i,'g',15);
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
}

ActivModel::ActivModel(int x, int y, int w, int h, int frame, int E_id, int O_id, int id)
{
    setAll(x,y,w,h,frame,E_id,O_id,id);
    setZero();
}

void ActivModel::setLandmarks(double marks[5][2])
{
    for(int i = 0; i < 5; i++){
        mLandmarks[i][0] = marks[i][0];
        mLandmarks[i][1] = marks[i][1];
    }
}

void ActivModel::setOrientation(double orien[3])
{
    mOrienation[0]=orien[0];
    mOrienation[1]=orien[1];
    mOrienation[2]=orien[2];
}

void ActivModel::setPosition(double pos[3])
{
    mPosition[0]=pos[0];
    mPosition[1]=pos[1];
    mPosition[2]=pos[2];
}

void ActivModel::setProjection(double proj[4])
{
    mProjection[0]=proj[0];
    mProjection[1]=proj[1];
    mProjection[2]=proj[2];
    mProjection[3]=proj[3];
}

int ActivModel::getFrame(){
    return mTimePos;
}

void ActivModel::setAll(int x, int y, int w, int h, int frame, int E_id, int O_id, int id)
{
    mX = x;
    mY = y;
    mW = w;
    mH = h;

    mTimePos = frame;

    mEventID = E_id;
    mObjectID = O_id;
    mID = id;
}

QString ActivModel::getDateAll()
{
    return toStr(mID)+"&"+toStr(mObjectID)+"&"+toStr(mEventID)+"&"+toStr(mTimePos)+"&"+toStr(mX)+"&"+toStr(mY)+"&"+toStr(mW)+"&"+toStr(mH)+"&"
            +toStr(mLandmarks[0][0])+"&"+toStr(mLandmarks[0][1])+"&"+toStr(mLandmarks[1][0])+"&"+toStr(mLandmarks[1][1])+"&"
            +toStr(mLandmarks[2][0])+"&"+toStr(mLandmarks[2][1])+"&"+toStr(mLandmarks[3][0])+"&"+toStr(mLandmarks[3][1])+"&"+toStr(mLandmarks[4][0])+"&"+toStr(mLandmarks[4][1])+"&"
            +toStr(mOrienation[0])+"&"+toStr(mOrienation[1])+"&"+toStr(mOrienation[2])+"&"
            +toStr(mPosition[0])+"&"+toStr(mPosition[1])+"&"+toStr(mPosition[2])+"&"
            +toStr(mProjection[0])+"&"+toStr(mProjection[1])+"&"+toStr(mProjection[2])+"&"+toStr(mProjection[3]);
}
