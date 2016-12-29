#include "loader.h"

#include <QFile>
#include <QTextStream>

Loader::Loader()
{
}

void Loader::loadFromFile(QString name, QString path){
    QFile file_event(path+name+"_event.txt");
    if(!file_event.open(QIODevice::ReadOnly)) {
        std::cout<<"Datei "<<(path+name+"_event.txt").toStdString()<<" nicht gefunden"<<std::endl;
    }else{
        QTextStream in(&file_event);
        while(!in.atEnd()) {
            QString line = in.readLine();
            mEvents.push_back(Event(line));
        }
    }
    file_event.close();

    QFile file_object(path+name+"_object.txt");
    if(!file_object.open(QIODevice::ReadOnly)) {
        std::cout<<"Datei "<<(path+name+"_object.txt").toStdString()<<" nicht gefunden"<<std::endl;
    }else{
        QTextStream in(&file_object);
        while(!in.atEnd()) {
            QString line = in.readLine();
            mObjects.push_back(Object(line));
        }
    }
    file_object.close();
}

Event Loader::getEvent(int id){
    return mEvents[id];
}

QStringList Loader::getObject(int id){
    QStringList ret;
    ret.push_back(mObjects[id].getName());
    ret.push_back(QString::number(mObjects[id].getID()));
    ret.push_back(mObjects[id].getDescription());
    return ret;
}

QStringList Loader::getObjectAllName(){
    QStringList ret;
    for(int i = 0; i < mObjects.size(); i++){
        ret.push_back(mObjects[i].getName());
    }
    return ret;
}

QStringList Loader::getEventAllName(){
    QStringList ret;
    for(int i = 0; i < mEvents.size(); i++){
        ret.push_back(mEvents[i].getName());
    }
    return ret;
}

void Loader::save(QString name, QString path)
{
    QFile file_event(path+name+"_event.txt");
    if(!file_event.open(QIODevice::WriteOnly)) {
        std::cout<<"Datei "<<(path+name+"_event.txt").toStdString()<<" nicht gefunden"<<std::endl;
    }else{
        QTextStream out(&file_event);
        for(int i = 0; i < mEvents.size(); i++){
            out << mEvents[i].getDateAll() << endl;
        }
    }
    file_event.close();

    QFile file_object(path+name+"_object.txt");
    if(!file_object.open(QIODevice::WriteOnly)) {
        std::cout<<"Datei "<<(path+name+"_object.txt").toStdString()<<" nicht gefunden"<<std::endl;
    }else{
        QTextStream out(&file_object);
        for(int i = 0; i < mObjects.size(); i++){
            out << mObjects[i].getDateAll() << endl;
        }
    }
    file_object.close();
}

void Loader::addNewObject(QString name, QString description)
{
    mObjects.push_back(Object(mObjects.size(), name, description));
}

int Loader::addObjectSave(QString name, QString description)
{
    bool exist = false;
    int ret = -1;
    for(int i = 0; i < mObjects.size(); i++){
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
    mEvents.push_back(Event(mEvents.size(), name, description, eye, activ, other, less, comm));
}

void Loader::addEventSave(QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
    bool exist = false;
    for(int i = 0; i < mEvents.size(); i++){
        if(name == mEvents[i].getName()){
            exist = true;
            break;
        }
    }
    if(!exist){
        addNewEvent(name, description, eye, activ, other, less, comm);
    }
}

void Loader::ChangeObject(int id, QString name, QString description)
{
    mObjects[id].change(name,description);
}

void Loader::ChamgeEvent(int id, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm)
{
    mEvents[id].change(name,description, eye, activ, other, less, comm);
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
    for(int i = 0; i < mEvents.size(); i++){
        if(mEvents[i].getName() == name){
            return i;
        }
    }
    return -1;
}

int Loader::getObjectID(QString name)
{
    for(int i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getName() == name){
            return i;
        }
    }
    return -1;
}

void Loader::clearAll()
{
    mEvents.clear();
    mObjects.clear();
}
