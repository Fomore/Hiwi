#ifndef EVENT_H
#define EVENT_H

#include <QString>

class Event
{
private:
    QString mName;
    QString mBeschreibung;

    bool mEyeVontact, mActiveParticipation, mOtherActivities, mRestlessness, mCommunication;
public:
    Event();
    Event(QString name, QString description,
          bool eye, bool activ, bool other, bool less, bool comm);

    QString getName();
    QString getDescription();
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
