#include "objecthandler.h"

ObjectHandler::ObjectHandler()
{
}


QStringList ObjectHandler::getObjectInfo(size_t O_pos){
    QStringList ret;
    if(O_pos < mObjects.size()){
    ret.push_back(mObjects[O_pos].getName());
    ret.push_back(QString::number(O_pos));
    ret.push_back(mObjects[O_pos].getDescription());
    }
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

void ObjectHandler::ChangeObject(size_t O_pos, QString name, QString description)
{
    if(O_pos < mObjects.size()){
    mObjects[O_pos].change(name,description);
    }
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

QString ObjectHandler::getObjectName(size_t O_pos)
{
    if(O_pos < mObjects.size()){
    return mObjects[O_pos].getName();
    }else{
        return "";
    }
}

void ObjectHandler::deleteObject(size_t pos)
{
    if(pos < mObjects.size()){
    mObjects.erase(mObjects.begin() + pos);
    }
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
