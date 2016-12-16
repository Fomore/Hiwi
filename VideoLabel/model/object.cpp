#include "object.h"

#include <QStringList>

#include <iostream>

Object::Object()
{
    mID = -1;
    mName = mBeschreibung = "Nicht definiert!";
}

Object::Object(QString data){
    QStringList data_list = data.split("&");

    if(data_list.size() == 3){
        mID = data_list[0].toInt();
        mName = data_list[1];
        mBeschreibung = data_list[2];
    }else{
        std::cout<<"Fehler in Object-Datei: "<<data.toStdString()<<std::endl;
    }
}

Object::Object(int id, QString name){
    mID = id;
    mName = name;
    mBeschreibung = "";
}

Object::Object(int id, QString name, QString description){
    mID = id;
    mName = name;
    mBeschreibung = description;
}

QString Object::getName(){
    return mName;
}

int Object::getID(){
    return mID;
}

QString Object::getDescription(){
    return mBeschreibung;
}

QString Object::getDateAll(){
    return QString::number(mID)+"&"+mName+"&"+mBeschreibung;
}

void Object::change(QString name, QString description)
{
    mName = name;
    mBeschreibung = description;
}
