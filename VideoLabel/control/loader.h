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

    QStringList getObject(int id);
    Event getEvent(int id);

    QStringList getObjectAllName();
    QStringList getEventAllName();

    void save(QString name, QString path);

    void addNewObject(QString name, QString description);
    int addObjectSave(QString name, QString description);

    void addNewEvent(QString name, QString description,
                     bool eye, bool activ, bool other, bool less, bool comm);
    void addEventSave(QString name, QString description,
                      bool eye, bool activ, bool other, bool less, bool comm);

    void ChangeObject(int id, QString name, QString description);
    void ChamgeEvent(int id, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm);

    int getObjectSize();
    int getEventSize();
    int getEventID(QString name);
    int getObjectID(QString name);

    void deleteEvent(int id);
    void deleteObject(int id);

    void clearAll();
};

#endif // LOADER_H
