#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <QString>
#include <QStringList>

#include "model/event.h"

class EventHandler
{
protected:
    std::vector<Event> mEvents;
public:
    EventHandler();

    void addNewEvent(QString name, QString description,
                     bool eye, bool activ, bool other, bool less, bool comm);
    void addEventSave(QString name, QString description,
                      bool eye, bool activ, bool other, bool less, bool comm);

    void ChangeEvent(int id, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm);

    Event getEvent(int id);
    int getEventSize();
    int getEventID(QString name);
    QStringList getEventAllName();
    QString getEventName(int E_id);

    void deleteEvent(int id);

    bool existEventID(int id);
};

#endif // EVENTHANDLER_H
