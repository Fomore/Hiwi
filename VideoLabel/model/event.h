#ifndef EVENT_H
#define EVENT_H

#include <QString>

class Event
{
private:
    int mID;
    QString mName;
    QString mBeschreibung;

public:
    Event();
    Event(QString data);
    Event(int id, QString name);
    Event(int id, QString name, QString description);
    QString getName();
    int getID();
    QString getDescription();
    QString getDateAll();
    void change(QString name, QString description);
};

#endif // EVENT_H
