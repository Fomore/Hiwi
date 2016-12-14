#include "loader.h"

#include <QFile>
#include <QTextStream>

Loader::Loader()
{
}

void Loader::loadFromFile(){
    loadFromFile("./data/");
}

void Loader::loadFromFile(QString path){
    QFile file_event(path+"event.txt");
    if(!file_event.open(QIODevice::ReadOnly)) {
        std::cout<<"Datei "<<(path+"event.txt").toStdString()<<" nicht gefunden"<<std::endl;
    }else{
        QTextStream in(&file_event);
        while(!in.atEnd()) {
            QString line = in.readLine();
            mEvents.push_back(Event(line));
        }
    }
    file_event.close();

    QFile file_object(path+"object.txt");
    if(!file_object.open(QIODevice::ReadOnly)) {
        std::cout<<"Datei "<<(path+"object.txt").toStdString()<<" nicht gefunden"<<std::endl;
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
