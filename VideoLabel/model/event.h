#ifndef EVENT_H
#define EVENT_H

#include <QString>

class Event
{
private:
    int mID;
    QString mName;
    QString mBeschreibung;

    bool mEyeVontact, mActiveParticipation, mOtherActivities, mRestlessness, mCommunication;
public:
    Event();
    Event(QString data);
    Event(int id, QString name);
    Event(int id, QString name, QString description);
    Event(int id, QString name, QString description,
          bool eye, bool activ, bool other, bool less, bool comm);
    QString getName();
    int getID();
    QString getDescription();
    QString getDateAll();
    bool getEyeVontact();
    bool getActiveParticipation();
    bool getOtherActivities();
    bool getRestlessness();
    bool getCommunication();

    void change(QString name, QString description,
                bool eye, bool activ, bool other, bool less, bool comm);
    bool OnTask();
};

#endif // EVENT_H
