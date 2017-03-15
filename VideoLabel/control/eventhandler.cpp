#include "eventhandler.h"

EventHandler::EventHandler()
{
}

Event EventHandler::getEvent(int id){
    return mEvents[id];
}

QStringList EventHandler::getEventAllName(){
    QStringList ret;
    for(size_t i = 0; i < mEvents.size(); i++){
        ret.push_back(mEvents[i].getName());
    }
    return ret;
}

void EventHandler::addNewEvent(QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
    mEvents.push_back(*(new Event(name, description, eye, activ, other, less, comm)));
}

void EventHandler::addEventSave(QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
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
    }
}

void EventHandler::ChangeEvent(int id, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
    mEvents[id].change(name,description, eye, activ, other, less, comm);
}

int EventHandler::getEventSize()
{
    return mEvents.size();
}

int EventHandler::getEventID(QString name)
{
    for(size_t i = 0; i < mEvents.size(); i++){
        if(mEvents[i].getName() == name){
            return i;
        }
    }
    return -1;
}

QString EventHandler::getEventName(int E_id)
{
    return mEvents[E_id].getName();
}

void EventHandler::deleteEvent(int id)
{
    mEvents.erase(mEvents.begin() + id);
}

bool EventHandler::existEventID(int id)
{
    return id >=0 && id < (int)mEvents.size();
}
