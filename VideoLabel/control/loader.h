#ifndef LOADER_H
#define LOADER_H

#include <QString>
#include <QStringList>
#include <iostream>

#include "model/event.h"
#include "model/object.h"

class Loader
{
private:
    std::vector<Event> mEvents;
    std::vector<Object> mObjects;
public:
    Loader();
    void loadFromFile(QString name);
    void loadFromFile(QString name, QString path);

    QStringList getObject(int id);
    QStringList getEvent(int id);

    QStringList getObjectAllName();
    QStringList getEventAllName();

    void save(QString name);
    void save(QString name, QString path);
};

#endif // LOADER_H
