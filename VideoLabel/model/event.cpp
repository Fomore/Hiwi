#include "event.h"

#include <QStringList>

#include <iostream>

Event::Event()
{
    mID = -1;
    mName = mBeschreibung = "Nicht definiert!";

    mEyeVontact = mActiveParticipation = mOtherActivities = mRestlessness = mCommunication = false;
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
    mEyeVontact = mActiveParticipation = mOtherActivities = mRestlessness = mCommunication = false;
}

Event::Event(int id, QString name){
    mID = id;
    mName = name;
    mBeschreibung = "";
    mEyeVontact = mActiveParticipation = mOtherActivities = mRestlessness = mCommunication = false;
}

Event::Event(int id, QString name, QString description){
    mID = id;
    mName = name;
    mBeschreibung = description;
    mEyeVontact = mActiveParticipation = mOtherActivities = mRestlessness = mCommunication = false;
}

Event::Event(int id, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
    mID = id;
    mName = name;
    mBeschreibung = description;
    mEyeVontact = eye;
    mActiveParticipation = activ;
    mOtherActivities = other;
    mRestlessness = less;
    mCommunication = comm;
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

bool Event::getEyeVontact()
{
    return mEyeVontact;
}

bool Event::getActiveParticipation()
{
    return mActiveParticipation;
}

bool Event::getOtherActivities()
{
    return mOtherActivities;
}

bool Event::getRestlessness()
{
    return mRestlessness;
}

bool Event::getCommunication()
{
    return mCommunication;
}

void Event::change(QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
    mName = name;
    mBeschreibung = description;
    mEyeVontact = eye;
    mActiveParticipation = activ;
    mOtherActivities = other;
    mRestlessness = less;
    mCommunication = comm;
}

bool Event::OnTask()
{
    int sum = mEyeVontact + mActiveParticipation + ! mOtherActivities + !mRestlessness + !mCommunication;
    return 3 <= sum;
}
