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
    Event getEvent(int id);

    QStringList getObjectAllName();
    QStringList getEventAllName();

    void save(QString name);
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

    int getID(QString name);

    void clearAll();

    void setLandmarks(int O_id, double marks[5][2]);
    void setOrientation(int O_id, double ori[3]);
    void setPosition(int O_id, double pos[3]);
    void setProjection(int O_id, double pro[4]);
};

#endif // LOADER_H
