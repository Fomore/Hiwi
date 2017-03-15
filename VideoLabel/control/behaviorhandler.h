#ifndef BEHAVIORHANDLER_H
#define BEHAVIORHANDLER_H

#include "model/verhaltentime.h"

class BehaviorHandler
{
protected:
    std::vector<VerhaltenTime> mBehaviors;
public:
    BehaviorHandler();

    std::vector<VerhaltenTime> getBehaviors(int oID);

    void addNewVerhalten(int o_ID, QString name, QString description,
                         bool eye, bool activ, bool other, bool less, bool comm,
                         int frame_S, int frame_E);

    void ChangeVerhalten(int id, int o_ID, QString name, QString description,
                         bool eye, bool activ, bool other, bool less, bool comm,
                         int frame_S, int frame_E);
};

#endif // BEHAVIORHANDLER_H
