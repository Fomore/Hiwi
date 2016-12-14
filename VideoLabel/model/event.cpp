#include "event.h"

#include <QStringList>

#include <iostream>

Event::Event()
{
    mID = -1;
    mName = mBeschreibung = "Nicht definiert!";
}

Event::Event(QString data){
    QStringList data_list = data.split("&");

    if(data_list.size() == 3){
        mID = data_list[0].toInt();
        mName = data_list[1];
        mBeschreibung = data_list[2];
    }else{
        std::cout<<"Fehler in Event-Datei: "<<data.toStdString()<<std::endl;
    }
}

Event::Event(int id, QString name){
    mID = id;
    mName = name;
    mBeschreibung = "";
}

Event::Event(int id, QString name, QString description){
    mID = id;
    mName = name;
    mBeschreibung = description;
}

QString Event::getName(){
    return mName;
}

int Event::getID(){
    return mID;
}

QString Event::getDescription(){
    return mBeschreibung;
}

QString Event::getDateAll(){
    return QString::number(mID)+"&"+mName+"&"+mBeschreibung;
}
