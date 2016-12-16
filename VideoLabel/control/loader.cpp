#include "loader.h"

#include <QFile>
#include <QTextStream>

Loader::Loader()
{
}

void Loader::loadFromFile(QString name){
    loadFromFile(name,"./data/");
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

QStringList Loader::getEvent(int id){
    QStringList ret;
    ret.push_back(mEvents[id].getName());
    ret.push_back(QString::number(mEvents[id].getID()));
    ret.push_back(mEvents[id].getDescription());
    return ret;
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

void Loader::save(QString name){
    save("./data/");
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

void Loader::addNewEvent(QString name, QString description)
{
    mEvents.push_back(Event(mEvents.size(), name, description));
}

void Loader::ChangeObject(int id, QString name, QString description)
{
    mObjects[id].change(name,description);
}

void Loader::ChamgeEvent(int id, QString name, QString description)
{
    mEvents[id].change(name,description);
}

int Loader::getObjectSize()
{
    return mObjects.size();
}

int Loader::getEventSize()
{
    return mEvents.size();
}
