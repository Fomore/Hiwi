#include "object.h"

#include <QStringList>

#include <iostream>

Object::Object()
{
    mName = mBeschreibung = "Nicht definiert!";
}

Object::Object(QString name, QString description){
    mName = name;
    mBeschreibung = description;
}

QString Object::getName(){
    return mName;
}

QString Object::getDescription(){
    return mBeschreibung;
}

void Object::change(QString name, QString description)
{
    mName = name;
    mBeschreibung = description;
}
