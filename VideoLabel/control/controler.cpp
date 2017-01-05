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
    mEvents.clear();
    mScall = 1.0;
}

void Controler::calculateParameter()
{
    mScall = std::min((double)DisplayHeight/(double)VideoHeight,(double)DisplayWidth/(double)VideoWidth);
    ShiftX = (DisplayWidth-VideoWidth*mScall)/2;
    ShiftY = (DisplayHeight-VideoHeight*mScall)/2;
}

void Controler::addEvent(int x1, int y1, int x2, int y2, int frame, int E_id, int O_iD)
{
    if(O_iD < 0 || O_iD >= mEvents.size()){
        std::cout<<"Fehler Objekt ID funktioniert nicht "<<O_iD<<" - "<<mEvents.size()<<std::endl;
        O_iD=0;
    }
    std::cout<<"Aufruf mit: "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<frame<<" "<<E_id<<" "<<O_iD<<std::endl;
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

    addEventInFrame(nx,ny,nw,nh,frame,E_id,O_iD,true);
}

void Controler::addEvent(int frame, int O_id, int E_id)
{
    if(O_id >= 0 && E_id >= 0 && (int) mEvents.size() > O_id){
        int pos = getPosition(frame, O_id);
        if(pos >= 0 && pos <(int)mEvents[O_id].size()){
            mEvents[O_id][pos].mEventID = E_id;
            //            std::cout<<"Ändere Event "<<frame<<"["<<O_id<<", "<<E_id<<"]"<<std::endl;
        }else{
            std::cout<<"Fehler bei Ändere Event "<<frame<<" ["<<O_id<<", "<<E_id<<"]"<<std::endl;
        }
    }
}

int Controler::addEventInFrame(int x, int y, int w, int h, int frame, int E_id, int O_iD, bool man)
{
    int pos = getPosition(frame, O_iD);
    if(O_iD >=0 && (int) mEvents.size() > O_iD){
        if(pos >= (int)mEvents[O_iD].size()){
            pos = mEvents[O_iD].size();
            mEvents[O_iD].push_back(ActivModel(x,y,w,h,frame,E_id,O_iD,pos, man));
        }else if(pos < 0){
            pos = 0;
            mEvents[O_iD].insert(mEvents[O_iD].begin(),ActivModel(x,y,w,h,frame,E_id,O_iD,pos, man));
        }else if(mEvents[O_iD][pos].getFrame() == frame){
            mEvents[O_iD][pos].setAll(x,y,w,h,frame,E_id,O_iD,mEvents[O_iD].size(), man);
        }else{
            mEvents[O_iD].insert(mEvents[O_iD].begin()+pos,ActivModel(x,y,w,h,frame,E_id,O_iD,pos, man));
        }
    }else{
        std::cout<<"Object ID Fehler "<<O_iD<<std::endl;
    }
    return pos;
}

bool Controler::changeObject(int frame, int lastO_id, int newO_id)
{
    printAll();
    std::cout<<"Change Obj "<<frame<<" ["<<lastO_id<<", "<<newO_id<<"] ";
    int lastpos = getPosition(frame, lastO_id);
    int newpos = getPosition(frame, newO_id);
    if(lastpos >= 0 && lastO_id != newO_id
            && lastpos < (int)mEvents[lastO_id].size()
            && mEvents[lastO_id][lastpos].getFrame() == frame){
        if(newpos < 0){
            mEvents[newO_id].insert(mEvents[newO_id].begin(),ActivModel(mEvents[lastO_id][lastpos]));
            mEvents[lastO_id].erase(mEvents[lastO_id].begin() + lastpos);
            mEvents[newO_id][0].mObjectID = newO_id;
        }else if(newpos >= (int)mEvents[newO_id].size()){
            mEvents[newO_id].push_back(ActivModel(mEvents[lastO_id][lastpos]));
            mEvents[lastO_id].erase(mEvents[lastO_id].begin() + lastpos);
            mEvents[newO_id][mEvents[newO_id].size()-1].mObjectID = newO_id;
        }else if(mEvents[newO_id][newpos].getFrame() == frame){
            std::cout<<"f"<<std::endl;
            printAll();
            return false;
        }else{
            mEvents[newO_id].insert(mEvents[newO_id].begin()+newpos,ActivModel(mEvents[lastO_id][lastpos]));
            mEvents[lastO_id].erase(mEvents[lastO_id].begin() + lastpos);
            mEvents[newO_id][newpos].mObjectID = newO_id;
        }
        std::cout<<"+"<<std::endl;
        printAll();
        return true;
    }else{
        std::cout<<"f"<<std::endl;
        printAll();
        return false;
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


int Controler::getPosition(int frame, int O_id)
{
    if(O_id >= 0 && O_id < mEvents.size() && mEvents[O_id].size() > 0){
        int top = mEvents[O_id].size()-1;
        int down = 0;
        if(mEvents[O_id][top].getFrame() == frame){
            return top;
        }else if(mEvents[O_id][top].getFrame() < frame){
            return top+1;
        }else if(mEvents[O_id][down].getFrame() == frame){
            return down;
        }else if(mEvents[O_id][down].getFrame() > frame){
            return -1;
        }else{
            while(top-down >1){
                int middle = (top+down)/2;
                int m_frame = mEvents[O_id][middle].getFrame();
                if(m_frame == frame){
                    return middle;
                }else if(m_frame > frame){
                    top = middle;
                }else{
                    down = middle;
                }
            }
            return down;
        }
    }
    return -1;
}

QRect Controler::getRect(int frame, int O_id, int &E_id){

    int lastID = getPosition(frame, O_id);

    if(lastID < 0 || lastID >= mEvents[O_id].size()){
        E_id = -1;
        return QRect(0,0,0,0);
    }else if(lastID == mEvents[O_id].size()-1 || mEvents[O_id][lastID].getFrame()==frame){
        int x = mEvents[O_id][lastID].mX;
        int y = mEvents[O_id][lastID].mY;
        int w = mEvents[O_id][lastID].mW;
        int h = mEvents[O_id][lastID].mH;

        E_id = mEvents[O_id][lastID].mEventID;

        return QRect(x,y,w,h);
    }else{
        double x = mEvents[O_id][lastID].mX;
        double y = mEvents[O_id][lastID].mY;
        double w = mEvents[O_id][lastID].mW;
        double h = mEvents[O_id][lastID].mH;

        int frame_diff = mEvents[O_id][lastID+1].getFrame() -mEvents[O_id][lastID].getFrame();

        double s = (double) (frame-mEvents[O_id][lastID].getFrame())/frame_diff;

        double x_diff = (mEvents[O_id][lastID+1].mX -mEvents[O_id][lastID].mX)*s;
        double y_diff = (mEvents[O_id][lastID+1].mY -mEvents[O_id][lastID].mY)*s;
        double w_diff = (mEvents[O_id][lastID+1].mW -mEvents[O_id][lastID].mW)*s;
        double h_diff = (mEvents[O_id][lastID+1].mH -mEvents[O_id][lastID].mH)*s;

        E_id = mEvents[O_id][lastID].mEventID;

        return QRect((x+x_diff), (y+y_diff), (w+w_diff), (h+h_diff));
    }
}

ActivModel Controler::getActivModel(int frame, int o_id)
{
    int pos = getPosition(frame,o_id);
    if(pos >= 0 && mEvents[o_id].size()){
        return mEvents[o_id][pos];
    }else{
        return ActivModel();
    }
}

void Controler::printAll()
{
    for(int i = 0; i < mEvents.size(); i++){
        for(int j = 0; j<mEvents[i].size(); j++){
            std::cout<<"["<<i<<","<<j<<"] "<<mEvents[i][j].printAll().toStdString()<<std::endl;
        }
        std::cout<<std::endl;
    }
}

int Controler::getEventToObject(int frame, int O_id)
{
    int i = getPosition(frame, O_id);
    if(i >= 0 && i < mEvents[O_id].size()){
        return mEvents[O_id][i].mEventID;
    }
    return -1;
}

void Controler::addEventVector()
{
    mEvents.push_back(std::vector<ActivModel>());
    mEvents.back().clear();
}

void Controler::setObjectSize(int anz)
{
    for(int i = mEvents.size(); i < anz; i++){
        addEventVector();
    }
}

void Controler::loadFromFile(QString name, QString path){
    QFile file_object(path+name+"_activevent.txt");
    if(!file_object.open(QIODevice::ReadOnly)) {
        std::cout<<"Datei "<<(path+name+"_activevent.txt").toStdString()<<" nicht gefunden"<<std::endl;
    }else{
        QTextStream in(&file_object);
        while(!in.atEnd()) {
            QString line = in.readLine();
            ActivModel am(line);
            while(am.mObjectID >= mEvents.size()){
                addEventVector();
            }
            mEvents[am.mObjectID].push_back(am);
        }
    }
    file_object.close();
}

void Controler::save(QString name, QString path)
{
    QFile file_object(path+name+"_activevent.txt");
    if(!file_object.open(QIODevice::WriteOnly)) {
        std::cout<<"Datei "<<(path+name+"_activevent.txt").toStdString()<<" nicht gefunden"<<std::endl;
    }else{
        QTextStream out(&file_object);
        for(int i = 0; i < mEvents.size(); i++){
            for(int j = 0; j < mEvents[i].size(); j++)
                out << mEvents[i][j].getDateAll() << endl;
        }
    }
    file_object.close();
}

void Controler::clearAll()
{
    for(int i = 0; i < mEvents.size(); i++){
        mEvents[i].clear();
    }
    mEvents.clear();
}

void Controler::setLandmarks(int O_id, int pos, double marks[5][2])
{
    if(O_id >= 0 && O_id < mEvents.size()
            && pos >= 0 && pos < mEvents[O_id].size()){
        mEvents[O_id][pos].setLandmarks(marks);
    }
}

void Controler::setOrientation(int O_id, int E_id, double ori[3])
{
    if(O_id >= 0 && O_id < mEvents.size()
            && E_id >= 0 && E_id < mEvents[O_id].size())
        mEvents[O_id][E_id].setOrientation(ori);
}

void Controler::setPosition(int O_id, int E_id, double pos[3])
{
    if(O_id >= 0 && O_id < mEvents.size()
            && E_id >= 0 && E_id < mEvents[O_id].size())
        mEvents[O_id][E_id].setPosition(pos);
}

void Controler::setProjection(int O_id, int E_id, double pro[4])
{
    if(O_id >= 0 && O_id < mEvents.size()
            && E_id >= 0 && E_id < mEvents[O_id].size())
        mEvents[O_id][E_id].setProjection(pro);
}

bool Controler::getNextSetFrame(int &frame)
{
    std::vector<int> next;
    next.clear();

    for(int i = 0; i < mEvents.size(); i++){
        int tmp = getPosition(frame,i)+1;
        if(tmp < mEvents[i].size()){
            next.push_back(mEvents[i][tmp].getFrame());
        }
    }
    if(next.size() > 0){
        int nframe = *std::min_element(next.begin(), next.end());
        //        std::cout<<"Next "<<frame<<" -> "<<nframe<<std::endl;
        bool ret = frame < nframe;
        frame = nframe;
        return ret && frame >=0;
    }else{
        return false;
    }
}


bool Controler::getEventUsed(int id)
{
    for(int i = 0; i < mEvents.size(); i++){
        for(int j = 0; j < mEvents[i].size();j++){
            if(mEvents[i][j].mEventID == id)
                return true;
        }
    }
    return false;
}

bool Controler::getObjectUsed(int id)
{
    return mEvents[id].size() == 0;
}

void Controler::deleteEvent(int id)
{
    for(int i = 0; i < mEvents.size(); i++){
        for(int j = 0; j < mEvents[i].size();j++){
            if(mEvents[i][j].mEventID == id){
                mEvents[i][j].mEventID = -1;
            }else if(mEvents[i][j].mEventID > id){
                mEvents[i][j].mEventID--;
            }
        }
    }
}

void Controler::deleteObject(int id)
{
    for(int i = 0; i < mEvents.size(); i++){
        for(int j = 0; j < mEvents[i].size();j++){
            if(mEvents[i][j].mObjectID == id){
                mEvents[i][j].mObjectID = -1;
            }else if(mEvents[i][j].mObjectID > id){
                mEvents[i][j].mObjectID--;
            }
        }
    }
}

void Controler::deleteActionEvent(int O_id, int frame)
{
    for(int j = 0; j < mEvents[O_id].size();j++){
        if(mEvents[O_id][j].getFrame() == frame){
            mEvents[O_id].erase(mEvents[O_id].begin() + j);
            j--;
        }
    }
}
