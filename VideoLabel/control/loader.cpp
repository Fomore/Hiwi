#include "loader.h"

#include <QFile>
#include <QTextStream>

Loader::Loader()
{
}

Event Loader::getEvent(int id){
    return mEvents[id];
}

QStringList Loader::getObject(int id){
    QStringList ret;
    ret.push_back(mObjects[id].getName());
    ret.push_back(QString::number(id));
    ret.push_back(mObjects[id].getDescription());
    return ret;
}

QStringList Loader::getObjectAllName(){
    QStringList ret;
    for(size_t i = 0; i < mObjects.size(); i++){
        ret.push_back(mObjects[i].getName());
    }
    return ret;
}

QStringList Loader::getEventAllName(){
    QStringList ret;
    for(size_t i = 0; i < mEvents.size(); i++){
        ret.push_back(mEvents[i].getName());
    }
    return ret;
}

void Loader::addNewObject(QString name, QString description)
{
    mObjects.push_back(Object(name, description));
}

int Loader::addObjectSave(QString name, QString description)
{
    bool exist = false;
    int ret = -1;
    for(size_t i = 0; i < mObjects.size(); i++){
        if(name == mObjects[i].getName()){
            exist = true;
            ret = i;
            break;
        }
    }
    if(!exist){
        ret = mObjects.size();
        addNewObject(name, description);
        std::cout<<"Object: "<<name.toStdString()<<std::endl;
    }
    return ret;
}

void Loader::addNewEvent(QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
    mEvents.push_back(Event(name, description, eye, activ, other, less, comm));
}

void Loader::addEventSave(QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
    bool exist = false;
    for(size_t i = 0; i < mEvents.size(); i++){
        if(name == mEvents[i].getName()){
            exist = true;
            break;
        }
    }
    if(!exist){
        addNewEvent(name, description, eye, activ, other, less, comm);
        std::cout<<"Event: "<<name.toStdString()<<std::endl;
    }
}

void Loader::addNewVerhalten(int o_ID, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm, int frame_S, int frame_E)
{
    mBehaviors.push_back(VerhaltenTime(o_ID,name, description, eye, activ, other, less, comm, frame_S, frame_E));
}

void Loader::ChangeObject(int id, QString name, QString description)
{
    mObjects[id].change(name,description);
}

void Loader::ChamgeEvent(int id, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
    mEvents[id].change(name,description, eye, activ, other, less, comm);
}

void Loader::ChamgeVerhalten(int id, int o_ID, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm, int frame_S, int frame_E)
{
    mBehaviors[id].change(o_ID,name, description, eye, activ, other, less, comm, frame_S, frame_E);
}

int Loader::getObjectSize()
{
    return mObjects.size();
}

int Loader::getEventSize()
{
    return mEvents.size();
}

int Loader::getEventID(QString name)
{
    for(size_t i = 0; i < mEvents.size(); i++){
        if(mEvents[i].getName() == name){
            return i;
        }
    }
    return -1;
}

int Loader::getObjectID(QString name)
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getName() == name){
            return i;
        }
    }
    return -1;
}

QString Loader::getObjectName(int O_id)
{
    return mObjects[O_id].getName();
}

QString Loader::getEventName(int E_id)
{
    return mEvents[E_id].getName();
}

void Loader::deleteEvent(int id)
{
    mEvents.erase(mEvents.begin() + id);
}

void Loader::deleteObject(int id)
{
    mObjects.erase(mObjects.begin() + id);
}

bool Loader::existObjectID(int id)
{
    return id >=0 && id < (int)mObjects.size();
}

bool Loader::existEventID(int id)
{
    return id >=0 && id < (int)mEvents.size();
}

void Loader::clearAll()
{
    mEvents.clear();
    mObjects.clear();
}
