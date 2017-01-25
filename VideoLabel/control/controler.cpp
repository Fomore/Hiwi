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
    mActivModel.clear();
    mScall = 1.0;
}

void Controler::calculateParameter()
{
    mScall = std::min((double)DisplayHeight/(double)VideoHeight,(double)DisplayWidth/(double)VideoWidth);
    ShiftX = (DisplayWidth-VideoWidth*mScall)/2;
    ShiftY = (DisplayHeight-VideoHeight*mScall)/2;
}

int Controler::getObjectPosInVector(int frame, int O_id)
{
    for(size_t i = 0; i < mActivModel[frame].size(); i++){
        if(mActivModel[frame][i].getObjectID() == O_id){
            return i;
        }
    }
    return -1;
}

void Controler::addEvent(int x1, int y1, int x2, int y2, int frame, int E_id, int O_id)
{
    if(O_id < 0 || (size_t)O_id >= mActivModel.size()){
        std::cout<<"Fehler Objekt ID funktioniert nicht "<<O_id<<" - "<<mActivModel.size()<<std::endl;
        O_id=0;
    }
    std::cout<<"Aufruf mit: "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<frame<<" "<<E_id<<" "<<O_id<<std::endl;
    int x = std::min(x1,x2);
    int w = std::max(x1,x2)-x;
    int y = std::min(y1,y2);
    int h = std::max(y1,y2)-y;

    WindoRectToVideoRect(x,y,w,h);

    addEventInFrame(x,y,w,h,frame,E_id,O_id,true);
}

void Controler::setEvent(int frame, int O_id, int E_id)
{
    int pos = getFramePosInVector(frame);
    if(pos >= 0){
        int o_pos = getObjectPosInVector(pos,O_id);
        if(o_pos >= 0){
            mActivModel[pos][o_pos].setEventID(E_id);
        }
    }
}

int Controler::addEventInFrame(int x, int y, int w, int h, int frame, int E_id, int O_id, bool man)
{
    int pos = getFramePosInVector(frame);
    if(pos < 0){
        mActivModel.insert(mActivModel.begin(),std::vector<ActivModel>());
        mActivModel[0].clear();
        mActivModel[0].push_back(ActivModel(x,y,w,h,frame,E_id,O_id,man));
        pos = 0;
    }else if(pos >= (int)mActivModel.size()){
        mActivModel.push_back(std::vector<ActivModel>());
        mActivModel[pos].clear();
        mActivModel[pos].push_back(ActivModel(x,y,w,h,frame,E_id,O_id,man));
    }else if(mActivModel[pos][0].getFrame() == frame){
        int o_pos = getObjectPosInVector(pos,O_id);
        if(o_pos >= 0){
            std::cout<<"Object "<<O_id<<" in Frame "<<frame<<" schon vorhenden."<<std::endl;
        }
        mActivModel[pos].push_back(ActivModel(x,y,w,h,frame,E_id,O_id,man));
    }else{
        pos++;
        mActivModel.insert(mActivModel.begin()+pos,std::vector<ActivModel>());
        mActivModel[pos].clear();
        mActivModel[pos].push_back(ActivModel(x,y,w,h,frame,E_id,O_id,man));
    }
    return pos;
}

void Controler::setAllObject(int lastO_id, int newO_id)
{
    for(size_t i = 0; i < mActivModel.size(); i++){
        for(size_t j = 0; j < mActivModel[i].size(); j++){
            if(mActivModel[i][j].getObjectID() == lastO_id){
                mActivModel[i][j].setObjectID(newO_id);
            }
        }
    }
}

void Controler::setObject(int frame, int lastO_id, int newO_id)
{
    int pos = getFramePosInVector(frame);
    if(lastO_id != newO_id && pos >= 0){
        int o_pos = getObjectPosInVector(pos,lastO_id);
        if(o_pos >= 0){
            mActivModel[pos][o_pos].setObjectID(newO_id);
        }
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


int Controler::getFramePosInVector(int frame)
{
    if(frame < 0 || mActivModel.size() == 0){
        return -1;
    }else if((size_t)frame >= mActivModel.size() || (mActivModel[frame].size() > 0
                                             && mActivModel[frame][0].getFrame() <= frame)){
        for(int i = std::min(frame,(int)mActivModel.size()-1); i >= 0; i--){
            if(mActivModel[i][0].getFrame() == frame){
                return i;
            }else if(mActivModel[i][0].getFrame() < frame){
                return i;
            }
        }
        return -1;
    }else{
        for(size_t i = 0; i < mActivModel.size(); i++){
            if(mActivModel[i][0].getFrame() == frame){
                return i;
            }else if(mActivModel[i][0].getFrame() > frame){
                return i-1;
            }
        }
        return -1;
    }
}

ActivModel Controler::getActivModel(int frame_pos, int O_pos)
{
    if(frame_pos >= 0 && (size_t) frame_pos < mActivModel.size()
            && O_pos >= 0 && (size_t) O_pos < mActivModel[frame_pos].size()){
            return mActivModel[frame_pos][O_pos];
    }
    return ActivModel();
}

int Controler::getLastLabel(int O_id)
{
    for(size_t i = 0; i < mActivModel.size(); i++){
        for(size_t j = 0; j<mActivModel[i].size(); j++){
            if(mActivModel[i][j].getEventID() == -1
                    && (O_id == -1 || mActivModel[i][j].getObjectID() == O_id) ){
                return mActivModel[i][j].getFrame();
            }
        }
    }
    return -1;
}

int Controler::getObjectSizeInFramePos(int frame_pos)
{
    if(frame_pos >= 0 && frame_pos < (int) mActivModel.size()){
        return mActivModel[frame_pos].size();
    }else{
        return -1;
    }
}

std::vector<ActivModel> Controler::getAllActivodel(int O_id)
{
    std::vector<ActivModel> list;
    for(size_t i = 0; i < mActivModel.size(); i++){
        for(size_t j = 0; j<mActivModel[i].size(); j++){
            if(mActivModel[i][j].getObjectID() == O_id){
                list.push_back(mActivModel[i][j]);
            }
        }
    }
    return list;
}

void Controler::printAll()
{
    for(size_t i = 0; i < mActivModel.size(); i++){
        for(size_t j = 0; j<mActivModel[i].size(); j++){
            std::cout<<"["<<i<<","<<j<<"] "<<mActivModel[i][j].printAll().toStdString()<<std::endl;
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
        int o_pos = getObjectPosInVector(pos,O_id);
        if(o_pos >= 0){
            return mActivModel[pos][o_pos].getEventID();
        }
    }
    return -1;
}

void Controler::clearAll()
{
    for(size_t i = 0; i < mActivModel.size(); i++){
        mActivModel[i].clear();
    }
    mActivModel.clear();
}

void Controler::setLandmarks(int pos, int O_id, double marks[5][2])
{
    if(pos >= 0){
        int o_pos = getObjectPosInVector(pos,O_id);
        if(o_pos >= 0){
            mActivModel[pos][o_pos].setLandmarks(marks);
        }
    }
}

void Controler::setOrientation(int pos, int O_id, double ori[3])
{
    if(pos >= 0){
        int o_pos = getObjectPosInVector(pos,O_id);
        if(o_pos >= 0){
            mActivModel[pos][o_pos].setOrientation(ori);
        }
    }
}

void Controler::setPosition(int frame_pos, int O_id, double pos[3])
{
    if(frame_pos >= 0){
        int o_pos = getObjectPosInVector(frame_pos,O_id);
        if(o_pos >= 0){
            mActivModel[frame_pos][o_pos].setPosition(pos);
        }
    }
}

void Controler::setProjection(int pos, int O_id, double pro[4])
{
    if(pos >= 0){
        int o_pos = getObjectPosInVector(pos,O_id);
        if(o_pos >= 0){
            mActivModel[pos][o_pos].setProjection(pro);
        }
    }
}

bool Controler::getNextSetFrame(int &frame)
{
    do {
        frame++;
    } while (frame >= 0
             && (size_t) frame < mActivModel.size()
             && mActivModel[frame].size() == 0);
    return frame >= 0 && (size_t) frame < mActivModel.size();
}


bool Controler::isEventUsed(int id)
{
    for(size_t i = 0; i < mActivModel.size(); i++){
        for(size_t j = 0; j < mActivModel[i].size();j++){
            if(mActivModel[i][j].getEventID() == id)
                return true;
        }
    }
    return false;
}

bool Controler::isObjectUsed(int id)
{
    return mActivModel[id].size() == 0;
}

void Controler::deleteEvent(int id)
{
    for(size_t i = 0; i < mActivModel.size(); i++){
        for(size_t j = 0; j < mActivModel[i].size();j++){
            if(mActivModel[i][j].getEventID() == id){
                mActivModel[i][j].setEventID(-1);
            }else if(mActivModel[i][j].getEventID() > id){
                mActivModel[i][j].setEventID(mActivModel[i][j].getEventID()-1);
            }
        }
    }
}

void Controler::deleteObject(int id)
{
    for(size_t i = 0; i < mActivModel.size(); i++){
        for(size_t j = 0; j < mActivModel[i].size();j++){
            if(mActivModel[i][j].getObjectID() == id){
                mActivModel[i][j].setObjectID(-1);
            }else if(mActivModel[i][j].getObjectID() > id){
                mActivModel[i][j].setObjectID(mActivModel[i][j].getObjectID()-1);
            }
        }
    }
}

void Controler::deleteActionEvent(int O_id, int frame)
{
    int pos = getFramePosInVector(frame);
    if(pos >= 0){
        int o_pos = getObjectPosInVector(pos, O_id);
        mActivModel[pos].erase(mActivModel[pos].begin() + o_pos);
        if(mActivModel[pos].size() == 0){
            mActivModel.erase(mActivModel.begin() + pos);
        }
    }
}
