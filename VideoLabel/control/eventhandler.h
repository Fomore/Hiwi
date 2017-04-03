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

    void ChangeEvent(int E_pos, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm);

    Event getEvent(int E_pos);
    size_t getEventSize();
    int getEventID(QString name);
    QStringList getEventAllName();
    QString getEventName(size_t E_pos);

    void deleteEvent(size_t pos);

    bool existEventPos(size_t pos);
};

#endif // EVENTHANDLER_H
