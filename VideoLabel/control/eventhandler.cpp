#include "eventhandler.h"

EventHandler::EventHandler()
{
}

Event EventHandler::getEvent(int E_pos){
    return mEvents[E_pos];
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

void EventHandler::ChangeEvent(int E_pos, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
    mEvents[E_pos].change(name,description, eye, activ, other, less, comm);
}

size_t EventHandler::getEventSize()
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

QString EventHandler::getEventName(size_t E_pos)
{
    return mEvents[E_pos].getName();
}

void EventHandler::deleteEvent(size_t pos)
{
    mEvents.erase(mEvents.begin() + pos);
}

bool EventHandler::existEventPos(size_t pos)
{
    return pos < (int)mEvents.size();
}
