#ifndef OBJECTHANDLER_H
#define OBJECTHANDLER_H

#include <QString>
#include <QStringList>

#include "model/object.h"

class ObjectHandler
{
protected:
    std::vector<Object> mObjects;
public:
    ObjectHandler();

    int getObjectSize();
    int getObjectID(QString name);
    QStringList getObjectAllName();
    QString getObjectName(int O_id);
    QStringList getObjectInfo(int id);

    void addNewObject(QString name, QString description);
    int addObjectSave(QString name, QString description);

    void ChangeObject(int id, QString name, QString description);

    void deleteObject(int id);

    bool exisitObject(QString name);
    size_t getNextAutoNameID();

    bool existObjectID(int id);
};

#endif // OBJECTHANDLER_H
