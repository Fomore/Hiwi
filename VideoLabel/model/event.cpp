#include "event.h"

#include <QStringList>

#include <iostream>

Event::Event()
{
    mName = mBeschreibung = "Nicht definiert!";

    mEyeVontact = mActiveParticipation = mOtherActivities = mRestlessness = mCommunication = false;
}

Event::Event(QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
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

QString Event::getDescription(){
    return mBeschreibung;
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
