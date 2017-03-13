#include "frame.h"

Frame::Frame()
{
    mFrameNr = 0;
    mObjects.clear();
}

Frame::Frame(size_t frameNr)
{
    mFrameNr = frameNr;
    mObjects.clear();
}

void Frame::clear()
{
    mObjects.clear();
}

size_t Frame::getFrameNr()
{
    return mFrameNr;
}

size_t Frame::getObjectSize()
{
    return mObjects.size();
}

ActivModel Frame::getObject(size_t ob_pos)
{
    if(ob_pos < mObjects.size()){
        return mObjects[ob_pos];
    }else{
        return ActivModel();
    }
}

std::vector<size_t> Frame::SeveralTimesObjects()
{
    std::vector<size_t> ret;
    ret.clear();
    for(size_t i = 0; i < mObjects.size(); i++){
        for(size_t j = i+1; j < mObjects.size(); j++){
            if(mObjects[i].getObjectID() == mObjects[j].getObjectID()
                    && mObjects[i].getObjectID() != -1){
                ret.push_back(i);
                ret.push_back(j);
            }
        }
    }
    return ret;
}

std::vector<size_t> Frame::getObjectOnPosition(int x, int y, int w, int h, int acc)
{
    std::vector<size_t> ret;
    ret.clear();
    for(size_t i = 0; i < mObjects.size(); i++){
        int mx, my, mw, mh;
        mObjects[i].getRect(mx, my, mw, mh);
        int x1 = std::max(x,mx);
        int y1 = std::max(y,my);
        int x2 = std::min(x+w,mx+mw);
        int y2 = std::min(y+h,my+mh);
        if(x1 > x2 && y1 > y2
                && std::min(mw, w)*acc/100 > x2-x1
                && std::min(mh, h)*acc/100 > y2-y1){
            ret.push_back(i);
        }
    }
    return ret;
}

int Frame::getObjectID(size_t pos)
{
    if(pos < mObjects.size()){
    return mObjects[pos].getObjectID();
    }else{
        return -2;
    }
}

int Frame::getEventID(int O_id)
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getObjectID() == O_id){
            mObjects[i].getEventID();
        }
    }
    return -1;
}

void Frame::setEventID(int O_id, int E_id)
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getObjectID() == O_id){
            mObjects[i].setEventID(E_id);
        }
    }
}

bool Frame::existEvent(int E_id)
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getEventID() == E_id){
            return true;
        }
    }
    return false;
}

void Frame::deleteEvent(int id)
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getEventID() == id){
            mObjects[i].setEventID(-1);
        }else if(mObjects[i].getEventID() > id){
            mObjects[i].setEventID(mObjects[i].getEventID()-1);
        }
    }
}

void Frame::setLandmarks(int O_id, double marks[5][2])
{

    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getObjectID() == O_id){
            mObjects[i].setLandmarks(marks);
        }
    }
}

void Frame::setOrientation(int O_id, double orien[3])
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getObjectID() == O_id){
            mObjects[i].setOrientation(orien);
        }
    }
}

void Frame::setPosition(int O_id, double pos[3])
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getObjectID() == O_id){
            mObjects[i].setPosition(pos);
        }
    }
}

void Frame::setProjection(int O_id, double proj[4])
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getObjectID() == O_id){
            mObjects[i].setProjection(proj);
        }
    }
}

void Frame::setRect(int O_id, int x, int y, int w, int h)
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getObjectID() == O_id){
            mObjects[i].setRect(x,y,w,h);
        }
    }
}

void Frame::setObjectID(int lastO_id, int newO_id)
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getObjectID() == lastO_id){
            mObjects[i].setObjectID(newO_id);
        }
    }
}

bool Frame::existObject(int O_id)
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(mObjects[i].getObjectID() == O_id){
            return true;
        }
    }
    return false;
}

void Frame::deleteObject(int id)
{
    for(size_t i = 0; i < mObjects.size();i++){
        if(mObjects[i].getObjectID() == id){
            mObjects[i].setObjectID(-1);
        }else if(mObjects[i].getObjectID() > id){
            mObjects[i].setObjectID(mObjects[i].getObjectID()-1);
        }
    }
}

void Frame::deleteActionEvent(size_t pos)
{
    if(pos < mObjects.size()){
        mObjects.erase(mObjects.begin() + pos);
    }
}

void Frame::addObject(int x, int y, int w, int h, int frame, int E_id, int O_id, bool man)
{
    mObjects.push_back(*(new ActivModel(x,y,w,h,frame,E_id,O_id,man)));
}

void Frame::addObject(ActivModel model)
{
    mObjects.push_back(*(new ActivModel(model)));
}

QString Frame::printAll(size_t pos)
{
    if(pos < mObjects.size()){
        return mObjects[pos].printAll();
    }else{
        return "";
    }
}
