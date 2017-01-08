#ifndef OBJECT_H
#define OBJECT_H

#include <QString>

class Object
{
private:
    QString mName;
    QString mBeschreibung;

public:
    Object();
    Object(QString name, QString description);
    QString getName();
    QString getDescription();

    void change(QString name, QString description);
};

#endif // OBJECT_H
