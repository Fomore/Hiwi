#ifndef OBJECT_H
#define OBJECT_H

#include <QString>

class Object
{
private:
    int mID;
    QString mName;
    QString mBeschreibung;

public:
    Object();
    Object(QString data);
    Object(int id, QString name);
    Object(int id, QString name, QString description);
    QString getName();
    int getID();
    QString getDescription();
    QString getDateAll();
};

#endif // OBJECT_H
