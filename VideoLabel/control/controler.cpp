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
    for(int i = 0; i < mActivModel[frame].size(); i++){
        if(mActivModel[frame][i].mObjectID == O_id){
            return i;
        }
    }
    return -1;
}

void Controler::addEvent(int x1, int y1, int x2, int y2, int frame, int E_id, int O_id)
{
    if(O_id < 0 || O_id >= mActivModel.size()){
        std::cout<<"Fehler Objekt ID funktioniert nicht "<<O_id<<" - "<<mActivModel.size()<<std::endl;
        O_id=0;
    }
    std::cout<<"Aufruf mit: "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<frame<<" "<<E_id<<" "<<O_id<<std::endl;
    int x = std::min(x1,x2);
    int w = std::max(x1,x2)-x;
    int y = std::min(y1,y2);
    int h = std::max(y1,y2)-y;

    x -= ShiftX;
    y -= ShiftY;

    double scall = mScall;
    double nx = x/scall+0.5;
    double ny = y/scall+0.5;
    double nw = w/scall+0.5;
    double nh = h/scall+0.5;

    addEventInFrame(nx,ny,nw,nh,frame,E_id,O_id,true);
}

void Controler::setEvent(int frame, int O_id, int E_id)
{
    int pos = getFramePosInVector(frame);
    if(pos >= 0){
        int o_pos = getObjectPosInVector(pos,O_id);
        if(o_pos >= 0){
            mActivModel[pos][o_pos].mEventID = E_id;
        }
    }
}

int Controler::addEventInFrame(int x, int y, int w, int h, int frame, int E_id, int O_id, bool man)
{
    int pos = getFramePosInVector(frame);
    if(pos < 0){
        mActivModel.insert(mActivModel.begin(),std::vector<ActivModel>());
        mActivModel[0].clear();
        mActivModel[0].push_back(ActivModel(x,y,w,h,frame,E_id,O_id,pos, man));
        pos = 0;
    }else if(pos >= (int)mActivModel.size()){
        mActivModel.push_back(std::vector<ActivModel>());
        mActivModel[pos].clear();
        mActivModel[pos].push_back(ActivModel(x,y,w,h,frame,E_id,O_id,pos, man));
    }else if(mActivModel[pos][0].getFrame() == frame){
        int o_pos = getObjectPosInVector(pos,O_id);
        if(o_pos >= 0){
            mActivModel[pos][o_pos].setAll(x,y,w,h,frame,E_id,O_id,pos, man);
        }else{
            mActivModel[pos].push_back(ActivModel(x,y,w,h,frame,E_id,O_id,pos, man));
        }
    }else{
        pos++;
        mActivModel.insert(mActivModel.begin()+pos,std::vector<ActivModel>());
        mActivModel[pos].clear();
        mActivModel[pos].push_back(ActivModel(x,y,w,h,frame,E_id,O_id,pos, man));
    }
    return pos;
}

void Controler::setObject(int frame, int lastO_id, int newO_id)
{
    int pos = getFramePosInVector(frame);
    if(lastO_id != newO_id && pos >= 0){
        int o_pos = getObjectPosInVector(pos,lastO_id);
        if(o_pos >= 0){
            mActivModel[pos][o_pos].mObjectID = newO_id;
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
    }else if(frame >= mActivModel.size() || (mActivModel[frame].size() > 0
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
        for(int i = 0; i < mActivModel.size(); i++){
            if(mActivModel[i][0].getFrame() == frame){
                return i;
            }else if(mActivModel[i][0].getFrame() > frame){
                return i-1;
            }
        }
        return -1;
    }
}

QRect Controler::getRect(int frame, int O_id, int &E_id){
    int frame_pos = getFramePosInVector(frame);

    if(frame_pos < 0 || frame_pos >= mActivModel.size() || mActivModel[frame_pos][0].getFrame()!=frame){
        E_id = -1;
        return QRect(0,0,0,0);
    }else{
        int o_pos = getObjectPosInVector(frame_pos,O_id);
        if(o_pos >= 0){
            int x = mActivModel[frame_pos][o_pos].mX;
            int y = mActivModel[frame_pos][o_pos].mY;
            int w = mActivModel[frame_pos][o_pos].mW;
            int h = mActivModel[frame_pos][o_pos].mH;

            E_id = mActivModel[frame_pos][o_pos].mEventID;
            return QRect(x,y,w,h);
        }
        E_id = -1;
        return QRect(0,0,0,0);
    }
}

ActivModel Controler::getActivModel(int frame, int O_id)
{
    int pos = getFramePosInVector(frame);
    if(pos >= 0 && mActivModel[O_id].size()){
        int o_pos = getObjectPosInVector(pos,O_id);
        if(o_pos >= 0)
            return mActivModel[O_id][pos];
    }
    return ActivModel();
}

int Controler::getLastLabel(int O_id)
{
    for(int i = 0; i < mActivModel.size(); i++){
        for(int j = 0; j<mActivModel[i].size(); j++){
            if(mActivModel[i][j].mEventID == -1
                    && (O_id == -1 || mActivModel[i][j].mObjectID == O_id) ){
                return mActivModel[i][j].getFrame();
            }
        }
    }
}

void Controler::printAll()
{
    for(int i = 0; i < mActivModel.size(); i++){
        for(int j = 0; j<mActivModel[i].size(); j++){
            std::cout<<"["<<i<<","<<j<<"] "<<mActivModel[i][j].printAll().toStdString()<<std::endl;
        }
        std::cout<<std::endl;
    }
}

int Controler::getEventToObject(int frame, int O_id)
{
    int pos = getFramePosInVector(frame);
    if(pos >= 0){
        int o_pos = getObjectPosInVector(pos,O_id);
        if(o_pos >= 0){
            return mActivModel[pos][o_pos].mEventID;
        }
    }
    return -1;
}

void Controler::save(QString name, QString path)
{
    QFile file_object(path+name+"_activevent.txt");
    if(!file_object.open(QIODevice::WriteOnly)) {
        std::cout<<"Datei "<<(path+name+"_activevent.txt").toStdString()<<" nicht gefunden"<<std::endl;
    }else{
        QTextStream out(&file_object);
        for(int i = 0; i < mActivModel.size(); i++){
            for(int j = 0; j < mActivModel[i].size(); j++)
                out << mActivModel[i][j].getDateAll() << endl;
        }
    }
    file_object.close();
}

void Controler::clearAll()
{
    for(int i = 0; i < mActivModel.size(); i++){
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
    int lastFrame = frame;
    int pos = getFramePosInVector(frame)+1;
    if(pos < 0 || pos >= mActivModel.size())
        return false;
    frame = mActivModel[pos][0].getFrame();
    return lastFrame < frame;
}


bool Controler::isEventUsed(int id)
{
    for(int i = 0; i < mActivModel.size(); i++){
        for(int j = 0; j < mActivModel[i].size();j++){
            if(mActivModel[i][j].mEventID == id)
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
    for(int i = 0; i < mActivModel.size(); i++){
        for(int j = 0; j < mActivModel[i].size();j++){
            if(mActivModel[i][j].mEventID == id){
                mActivModel[i][j].mEventID = -1;
            }else if(mActivModel[i][j].mEventID > id){
                mActivModel[i][j].mEventID--;
            }
        }
    }
}

void Controler::deleteObject(int id)
{
    for(int i = 0; i < mActivModel.size(); i++){
        for(int j = 0; j < mActivModel[i].size();j++){
            if(mActivModel[i][j].mObjectID == id){
                mActivModel[i][j].mObjectID = -1;
            }else if(mActivModel[i][j].mObjectID > id){
                mActivModel[i][j].mObjectID--;
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
