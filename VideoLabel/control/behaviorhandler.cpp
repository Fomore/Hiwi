#include "behaviorhandler.h"

BehaviorHandler::BehaviorHandler()
{
}

std::vector<VerhaltenTime> BehaviorHandler::getBehaviors(int oID)
{
    std::vector<VerhaltenTime> ret;
    for(size_t i = 0; i < mBehaviors.size(); i++){
        if(mBehaviors[i].getObjectID() == oID){
            ret.push_back(mBehaviors[i]);
        }
    }
    return ret;
}

void BehaviorHandler::addNewVerhalten(int o_ID, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm, int frame_S, int frame_E)
{
    mBehaviors.push_back(VerhaltenTime(o_ID,name, description, eye, activ, other, less, comm, frame_S, frame_E));
}

void BehaviorHandler::ChangeVerhalten(int id, int o_ID, QString name, QString description, bool eye, bool activ, bool other, bool less, bool comm, int frame_S, int frame_E)
{
    mBehaviors[id].change(o_ID,name, description, eye, activ, other, less, comm, frame_S, frame_E);
}
