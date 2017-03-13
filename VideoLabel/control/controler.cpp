#include "controler.h"

#include <math.h>

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <algorithm>

Controler::Controler()
{
    VideoHeight = VideoWidth = 0;
    DisplayHeight = DisplayWidth = 0;
    ShiftX = ShiftY = 0;
    mFrames.clear();
    mScall = 1.0;
}

void Controler::calculateParameter()
{
    mScall = std::min((double)DisplayHeight/(double)VideoHeight,(double)DisplayWidth/(double)VideoWidth);
    ShiftX = (DisplayWidth-VideoWidth*mScall)/2;
    ShiftY = (DisplayHeight-VideoHeight*mScall)/2;
}

void Controler::addEvent(int x1, int y1, int x2, int y2, int frameNr, int E_id, int O_id)
{
    std::cout<<"Aufruf mit: "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<frameNr<<" "<<E_id<<" "<<O_id<<std::endl;
    int x = std::min(x1,x2);
    int w = std::max(x1,x2)-x;
    int y = std::min(y1,y2);
    int h = std::max(y1,y2)-y;

    WindoRectToVideoRect(x,y,w,h);

    addObjectInFrame(x,y,w,h,frameNr,E_id,O_id,true);
}

void Controler::setEvent(int frame, int O_id, int E_id)
{
    int pos = getFramePosInVector(frame);
    if(pos >= 0){
        mFrames[pos].setEventID(O_id, E_id);
    }
}

int Controler::addObjectInFrame(int x, int y, int w, int h, int frameNr, int E_id, int O_id, bool man)
{
    int pos = getFramePosInVector(frameNr);
    if(pos < 0){
        mFrames.insert(mFrames.begin(),*(new Frame(frameNr)));
        mFrames[0].addObject(x,y,w,h,frameNr,E_id,O_id,man);
        pos = 0;
    }else if(pos >= (int)mFrames.size()){
        mFrames.push_back(*(new Frame(frameNr)));
        pos = mFrames.size()-1;
        mFrames[pos].addObject(x,y,w,h,frameNr,E_id,O_id,man);
    }else if(mFrames[pos].getFrameNr() == frameNr){
        mFrames[pos].addObject(x,y,w,h,frameNr,E_id,O_id,man);
    }else{
        pos++;
        mFrames.insert(mFrames.begin()+pos,*(new Frame(frameNr)));
        mFrames[pos].addObject(x,y,w,h,frameNr,E_id,O_id,man);
    }
    return pos;
}

void Controler::setAllObject(int lastO_id, int newO_id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        mFrames[i].setObjectID(lastO_id, newO_id);
    }
}

void Controler::setObject(int frameNr, int lastO_id, int newO_id)
{
    int pos = getFramePosInVector(frameNr);
    if(lastO_id != newO_id && pos >= 0 && mFrames[pos].getFrameNr() == frameNr){
        mFrames[pos].setObjectID(lastO_id, newO_id);
    }
}

void Controler::setVideoSize(int w, int h)
{
    VideoHeight = h;
    VideoWidth = w;
    calculateParameter();
}

void Controler::setDisplaySize(int w, int h)
{
    DisplayHeight = h;
    DisplayWidth = w;
    calculateParameter();
}


int Controler::getFramePosInVector(int frameNr)
{
    if(frameNr < 0 || mFrames.size() == 0){
        return -1;
    }else if((size_t)frameNr >= mFrames.size() || (mFrames[frameNr].getObjectSize() > 0
                                             && mFrames[frameNr].getFrameNr() <= frameNr)){
        for(size_t i = std::min((size_t)frameNr,mFrames.size()-1); i >= 0; i--){
            if(mFrames[i].getFrameNr() == frameNr || mFrames[i].getFrameNr() < frameNr){
                return i;
            }
        }
        return -1;
    }else{
        for(size_t i = 0; i < mFrames.size(); i++){
            if(mFrames[i].getFrameNr() == frameNr || mFrames[i].getFrameNr() > frameNr){
                return i-1;
            }
        }
        return -1;
    }
}

ActivModel Controler::getActivModel(size_t frame_pos, int O_pos)
{
    if(frame_pos < mFrames.size()
            && O_pos >= 0 && (size_t) O_pos < mFrames[frame_pos].getObjectSize()){
            return mFrames[frame_pos].getObject(O_pos);
    }
    return ActivModel();
}

int Controler::getLastLabel(int O_id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        if(mFrames[i].getEventID(O_id) == -1){
            return mFrames[i].getFrameNr();
        }
    }
    return -1;
}

int Controler::getLastFrame(int O_id)
{
    for(int i = (int)mFrames.size()-1; i >= 0; i--){
        if(mFrames[i].existObject(O_id)){
            return mFrames[i].getFrameNr();
        }
    }
    return -1;
}

int Controler::getObjectSizeInFramePos(int frame_pos)
{
    if(frame_pos >= 0 && frame_pos < (int) mFrames.size()){
        return mFrames[frame_pos].getObjectSize();
    }else{
        return -1;
    }
}

std::vector<cv::Point3i> Controler::getAllActivModel(int O_id)
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

void Controler::printAll()
{
    for(size_t i = 0; i < mFrames.size(); i++){
        for(size_t j = 0; j<mFrames[i].getObjectSize(); j++){
            std::cout<<"["<<i<<","<<j<<"] "<<mFrames[i].printAll(j).toStdString()<<std::endl;
        }
        std::cout<<std::endl;
    }
}

void Controler::WindoRectToVideoRect(int &x, int &y, int &w, int &h)
{
    x = (x-ShiftX)/mScall+0.5;
    y = (y-ShiftY)/mScall+0.5;
    w = w/mScall+0.5;
    h = h/mScall+0.5;
}

int Controler::getEventToObject(int frame, int O_id)
{
    int pos = getFramePosInVector(frame);
    if(pos >= 0){
            return mFrames[pos].getEventID(O_id);
    }
    return -1;
}

void Controler::clearAll()
{
    for(size_t i = 0; i < mFrames.size(); i++){
        mFrames[i].clear();
    }
    mFrames.clear();
}

void Controler::setLandmarks(int pos, int O_id, double marks[5][2])
{
    if(pos >= 0){
            mFrames[pos].setLandmarks(O_id,marks);
    }
}

void Controler::setOrientation(int pos, int O_id, double ori[3])
{
    if(pos >= 0){
            mFrames[pos].setOrientation(O_id,ori);
    }
}

void Controler::setPosition(int frame_pos, int O_id, double pos[3])
{
    if(frame_pos >= 0){
            mFrames[frame_pos].setPosition(O_id,pos);
        }
}

void Controler::setProjection(int pos, int O_id, double pro[4])
{
    if(pos >= 0){
            mFrames[pos].setProjection(O_id,pro);
        }
}

bool Controler::getNextSetFrame(int &frame)
{
    do {
        frame++;
    } while (frame >= 0
             && (size_t) frame < mFrames.size()
             && mFrames[frame].getObjectSize() == 0);
    return frame >= 0 && (size_t) frame < mFrames.size();
}


bool Controler::isEventUsed(int id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        if(mFrames[i].existEvent(id)){
                return true;
        }
    }
    return false;
}

bool Controler::isObjectUsed(int id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        if(mFrames[i].existObject(id)){
                return true;
        }
    }
    return false;
}

void Controler::deleteEvent(int id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        mFrames[i].deleteEvent(id);
    }
}

void Controler::deleteObject(int id)
{
    for(size_t i = 0; i < mFrames.size(); i++){
        mFrames[i].deleteObject(id);
    }
}

void Controler::deleteActionEvent(size_t obj_pos, size_t frm_pos)
{
    std::cout<<"Lösche: "<<frm_pos<<" - "<<obj_pos<<std::endl;
    if(frm_pos < mFrames.size()){
        mFrames[frm_pos].deleteActionEvent(obj_pos);
        if(mFrames[frm_pos].getObjectSize() == 0){
            mFrames.erase(mFrames.begin() + frm_pos);
        }
    }
}

void Controler::changeActionEventValue(int frmPos, int objPos, int eveID, int x, int y, int w, int h)
{
    mFrames[frmPos].setEventID(objPos,eveID);
    mFrames[frmPos].setRect(objPos,x,y,w,h);
}

int Controler::copyActionEvent(int frmPos, int objPos, int frame)
{
    int pos = getFramePosInVector(frame);
    if(pos < 0){
        pos = 0;
        mFrames.insert(mFrames.begin(),*(new Frame(frame)));
        mFrames[0].addObject(mFrames[frmPos].getObject(objPos));
    }else if(pos >= (int)mFrames.size()){
        pos = mFrames.size();
        mFrames.push_back(*(new Frame(frame)));
        mFrames[pos].addObject(mFrames[frmPos].getObject(objPos));
    }else if(mFrames[pos].getFrameNr() == frame){
        mFrames[pos].addObject(mFrames[frmPos].getObject(objPos));
    }else{
        pos++;
        mFrames.insert(mFrames.begin()+pos,*(new Frame(frame)));
        mFrames[pos].addObject(mFrames[frmPos].getObject(objPos));
    }
    mFrames[frmPos].deleteActionEvent(objPos);
    return pos;
}
