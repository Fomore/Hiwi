#ifndef LOADER_H
#define LOADER_H

#include <QString>
#include <QStringList>
#include <iostream>

#include "model/event.h"
#include "model/object.h"
#include "model/verhaltentime.h"

class Loader
{
private:
    std::vector<Event> mEvents;
    std::vector<Object> mObjects;
    std::vector<VerhaltenTime> mBehaviors;
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

    void addNewVerhalten(int o_ID, QString name, QString description,
                         bool eye, bool activ, bool other, bool less, bool comm,
                         int frame_S, int frame_E);

    void ChangeObject(int id, QString name, QString description);
    void ChamgeEvent(int id, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm);
    void ChamgeVerhalten(int id, int o_ID, QString name, QString description,
                         bool eye, bool activ, bool other, bool less, bool comm,
                         int frame_S, int frame_E);

    int getObjectSize();
    int getEventSize();
    int getEventID(QString name);
    int getObjectID(QString name);
    QString getObjectName(int O_id);
    QString getEventName(int E_id);

    void deleteEvent(int id);
    void deleteObject(int id);

    bool existObjectID(int id);
    bool existEventID(int id);

    void clearAll();
};

#endif // LOADER_H
