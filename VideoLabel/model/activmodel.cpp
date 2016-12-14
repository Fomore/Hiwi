#include "activmodel.h"

#include <iostream>
#include <QStringList>

ActivModel::ActivModel()
{
}

ActivModel::ActivModel(QString data)
{
    QStringList data_list = data.split("&");

    if(data_list.size() == 8){
        mID = data_list[0].toInt();
        mObjectID = data_list[1].toInt();
        mEventID = data_list[2].toInt();
        mTimePos = data_list[3].toInt();
        mX = data_list[4].toInt();
        mY = data_list[5].toInt();
        mW = data_list[6].toInt();
        mH = data_list[7].toInt();
    }else{
        std::cout<<"Fehler in Event-Datei: "<<data.toStdString()<<std::endl;
    }
}

QString ActivModel::toInt(int i)
{
    return QString::number(i);
}

ActivModel::ActivModel(int x, int y, int w, int h, int frame, int E_id, int O_id, int id)
{
    setAll(x,y,w,h,frame,E_id,O_id,id);
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
    std::cout<<"Speichere "<<mTimePos<<": "<< mX<<" "<<mY<<" "<<mW<<" "<<mH<<std::endl;
}

QString ActivModel::getDateAll()
{
    return toInt(mID)+"&"+toInt(mObjectID)+"&"+toInt(mEventID)+"&"+toInt(mTimePos)+"&"+toInt(mX)+"&"+toInt(mY)+"&"+toInt(mW)+"&"+toInt(mH);
}
