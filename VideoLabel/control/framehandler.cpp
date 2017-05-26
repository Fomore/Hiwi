#include "framehandler.h"

FrameHandler::FrameHandler()
{
    mFrames.clear();
}

void FrameHandler::setEvent(size_t frameNr, int O_pos, int E_id)
{
    int pos = getFramePosInVector(frameNr);
    if(pos >= 0){
        mFrames[pos].setEventID(O_pos, E_id);
    }
}

int FrameHandler::addObjectInFrame(int x, int y, int w, int h, size_t frameNr, int E_id, int O_id, bool man, int gaze)
{
    int pos = getFramePosInVector(frameNr);
    if(pos < 0){
        mFrames.insert(mFrames.begin(),*(new Frame(frameNr)));
        mFrames[0].addObject(x,y,w,h,frameNr,E_id,O_id,man,gaze);
        pos = 0;
    }else if(pos >= (int)mFrames.size()){
        mFrames.push_back(*(new Frame(frameNr)));
        pos = mFrames.size()-1;
        mFrames[pos].addObject(x,y,w,h,frameNr,E_id,O_id,man,gaze);
    }else if(mFrames[pos].getFrameNr() == frameNr){
        mFrames[pos].addObject(x,y,w,h,frameNr,E_id,O_id,man,gaze);
    }else{
        pos++;
        mFrames.insert(mFrames.begin()+pos,*(new Frame(frameNr)));
        mFrames[pos].addObject(x,y,w,h,frameNr,E_id,O_id,man,gaze);
    }
    return pos;
}

void FrameHandler::setAllObject(int lastO_id, int newO_id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        mFrames[i].setObjectID(lastO_id, newO_id);
    }
}

void FrameHandler::setObject(size_t frameNr, int lastO_id, int newO_id)
{
    int pos = getFramePosInVector(frameNr);
    if(lastO_id != newO_id && pos >= 0 && mFrames[pos].getFrameNr() == frameNr){
        mFrames[pos].setObjectID(lastO_id, newO_id);
    }
}

int FrameHandler::getFramePosInVector(size_t frameNr)
{
    if(frameNr >= mFrames.size() || mFrames[frameNr].getFrameNr() <= frameNr){
        for(size_t i = std::min(frameNr,mFrames.size()-1); i < mFrames.size(); i--){
            if(mFrames[i].getFrameNr() <= frameNr){
                return i;
            }
        }
        return -1;
    }else{
        for(size_t i = 0; i < mFrames.size(); i++){
            if(mFrames[i].getFrameNr() == frameNr){
                return i;
            }else if(mFrames[i].getFrameNr() > frameNr){
                return i-1;
            }
        }
        return -1;
    }
}

size_t FrameHandler::getFrameNr(size_t frame_pos)
{
    return mFrames[frame_pos].getFrameNr();
}

size_t FrameHandler::getFrameSize()
{
    return mFrames.size();
}

ActivModel FrameHandler::getActivModel(size_t frame_pos, size_t O_pos)
{
    if(frame_pos < mFrames.size()){
        return mFrames[frame_pos].getObject(O_pos);
    }else{
        return ActivModel();
    }
}

size_t FrameHandler::getLastLabel(int O_id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        if(mFrames[i].getEventID(O_id) == -1){
            return mFrames[i].getFrameNr();
        }
    }
    return -1;
}

int FrameHandler::getLastFrame(int O_id)
{
    for(size_t i = mFrames.size()-1; i < mFrames.size(); i--){
        if(mFrames[i].existObject(O_id)){
            return mFrames[i].getFrameNr();
        }
    }
    return -1;
}

int FrameHandler::getObjectSizeInFramePos(size_t frame_pos)
{
    if(frame_pos < mFrames.size()){
        return mFrames[frame_pos].getObjectSize();
    }else{
        return -1;
    }
}

std::vector<cv::Point3i> FrameHandler::getAllActivModel(int O_id)
{
    std::vector<cv::Point3i> list;
    for(size_t i = 0; i < mFrames.size(); i++){
        for(size_t j = 0; j<mFrames[i].getObjectSize(); j++){
            if(mFrames[i].getObjectID(j) == O_id){
                list.push_back(cv::Point3i(i,j,mFrames[i].getFrameNr()));
            }
        }
    }
    return list;
}

int FrameHandler::getEventToObject(size_t frameNr, int O_id)
{
    int pos = getFramePosInVector(frameNr);
    if(pos >= 0){
        return mFrames[pos].getEventID(O_id);
    }
    return -1;
}

void FrameHandler::setLandmarks(size_t pos, int O_id, double marks[5][2])
{
    if(pos < mFrames.size()){
        mFrames[pos].setLandmarks(O_id,marks);
    }
}

void FrameHandler::setOrientation(size_t pos, int O_id, double ori[3])
{
    if(pos < mFrames.size()){
        mFrames[pos].setOrientation(O_id,ori);
    }
}

void FrameHandler::setPosition(size_t frame_pos, int O_id, double pos[3])
{
    if(frame_pos < mFrames.size()){
        mFrames[frame_pos].setPosition(O_id,pos);
    }
}

void FrameHandler::setProjection(size_t pos, int O_id, double pro[4])
{
    if(pos < mFrames.size()){
        mFrames[pos].setProjection(O_id,pro);
    }
}

bool FrameHandler::isEventUsed(int id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        if(mFrames[i].existEvent(id)){
            return true;
        }
    }
    return false;
}

bool FrameHandler::isObjectUsed(int id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        if(mFrames[i].existObject(id)){
            return true;
        }
    }
    return false;
}

void FrameHandler::deleteEventID(int id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        mFrames[i].deleteEvent(id);
    }
}

void FrameHandler::deleteObjectID(int id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        mFrames[i].deleteObject(id);
    }
}

void FrameHandler::deleteActionEvent(size_t frm_pos, size_t obj_pos)
{
    std::cout<<"Lösche: "<<frm_pos<<" - "<<obj_pos<<std::endl;
    if(frm_pos < mFrames.size()){
        mFrames[frm_pos].deleteActionEvent(obj_pos);
        if(mFrames[frm_pos].getObjectSize() == 0){
            mFrames.erase(mFrames.begin() + frm_pos);
        }
    }
}

void FrameHandler::changeActionEventValue(size_t frmPos, size_t objPos, int eveID, int x, int y, int w, int h)
{
    mFrames[frmPos].setEventID(objPos,eveID);
    mFrames[frmPos].setRect(objPos,x,y,w,h);
}

int FrameHandler::copyActionEvent(size_t frmPos, size_t objPos, size_t frameNr)
{
    int pos = getFramePosInVector(frameNr);
    std::cout<<"Pos: "<<pos<<" "<<frmPos<<" "<<objPos<<" "<<frameNr<<std::endl;
    if(pos < 0){
        pos = 0;
        mFrames.insert(mFrames.begin(),*(new Frame(frameNr)));
        mFrames[0].addObject(mFrames[frmPos+1].getObject(objPos));
    }else if(pos >= (int)mFrames.size()){
        pos = mFrames.size();
        mFrames.push_back(*(new Frame(frameNr)));
        mFrames[pos].addObject(mFrames[frmPos].getObject(objPos));
    }else if(mFrames[pos].getFrameNr() == frameNr){
        mFrames[pos].addObject(mFrames[frmPos].getObject(objPos));
    }else{
        pos++;
        mFrames.insert(mFrames.begin()+pos,*(new Frame(frameNr)));
        mFrames[pos].addObject(mFrames[frmPos].getObject(objPos));
    }
    mFrames[frmPos].deleteActionEvent(objPos);
    return pos;
}
