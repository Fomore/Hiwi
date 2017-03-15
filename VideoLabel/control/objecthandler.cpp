#include "objecthandler.h"

ObjectHandler::ObjectHandler()
{
}


QStringList ObjectHandler::getObjectInfo(int id){
    QStringList ret;
    ret.push_back(mObjects[id].getName());
    ret.push_back(QString::number(id));
    ret.push_back(mObjects[id].getDescription());
    return ret;
}

QStringList ObjectHandler::getObjectAllName(){
    QStringList ret;
    for(size_t i = 0; i < mObjects.size(); i++){
        ret.push_back(mObjects[i].getName());
    }
    return ret;
}

void ObjectHandler::addNewObject(QString name, QString description)
{
    mObjects.push_back(*(new Object(name, description)));
}

int ObjectHandler::addObjectSave(QString name, QString description)
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
    }
    return ret;
}

void ObjectHandler::ChangeObject(int id, QString name, QString description)
{
    mObjects[id].change(name,description);
}

int ObjectHandler::getObjectSize()
{
    return mObjects.size();
}

int ObjectHandler::getObjectID(QString name)
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getName() == name){
            return i;
        }
    }
    return -1;
}

QString ObjectHandler::getObjectName(int O_id)
{
    return mObjects[O_id].getName();
}

void ObjectHandler::deleteObject(int id)
{
    mObjects.erase(mObjects.begin() + id);
}

bool ObjectHandler::exisitObject(QString name)
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getName() == name){
            return true;
        }
    }
    return false;
}

size_t ObjectHandler::getNextAutoNameID()
{
    size_t i = 0;
    QString name;
    do {
        name = "name_auto_"+QString::number(i);
        i++;
    } while (exisitObject(name));
        mObjects.push_back(*(new Object(name, "Automatisch erstelles Objekt")));
    return mObjects.size()-1;
}

bool ObjectHandler::existObjectID(int id)
{
    return id >=0 && id < (int)mObjects.size();
}
