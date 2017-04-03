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
    QString getObjectName(size_t O_pos);
    QStringList getObjectInfo(size_t O_pos);

    void addNewObject(QString name, QString description);
    int addObjectSave(QString name, QString description);

    void ChangeObject(size_t O_pos, QString name, QString description);

    void deleteObject(size_t pos);

    bool exisitObject(QString name);
    size_t getNextAutoNameID();
};

#endif // OBJECTHANDLER_H
