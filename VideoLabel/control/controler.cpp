#include "controler.h"

#include <math.h>

#include <iostream>
#include <QFile>
#include <QTextStream>

Controler::Controler()
{
    VideoHeight = VideoWidth = 0;
    DisplayHeight = DisplayWidth = 0;
    mEvents.clear();
}

double Controler::getScall()
{
    return std::min((double)VideoHeight/(double)DisplayHeight,(double)VideoWidth/(double)DisplayWidth);
}

void Controler::addEvent(int x1, int y1, int x2, int y2, int frame, int E_id, int O_iD)
{
    if(O_iD < 0 || O_iD >= mEvents.size()){
        std::cout<<"Fehler Objekt ID funktioniert nicht "<<O_iD<<std::endl;
        O_iD=0;
    }
    std::cout<<"Aufruf mit: "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<frame<<" "<<E_id<<" "<<O_iD<<std::endl;
    int x = std::min(x1,x2);
    int w = std::max(x1,x2)-x;
    int y = std::min(y1,y2);
    int h = std::max(y1,y2)-y;
    /*
    double scall = getScall();
    double x = x1*scall;
    double y = y1*scall;
    double w = x2*scall-x1*scall;
    double h = y2*scall-y1*scall;
    */
    int pos = getPosition(frame, O_iD);
    if(pos < mEvents[O_iD].size() && mEvents[O_iD][pos].getFrame() == frame){
        std::cout<<"Re-";
        mEvents[O_iD][pos].setAll(x,y,w,h,frame,E_id,O_iD,mEvents[O_iD].size());
    }else{
        mEvents[O_iD].insert(mEvents[O_iD].begin()+pos,ActivModel(x,y,w,h,frame,E_id,O_iD,pos));
    }
}

void Controler::setVideoSize(int w, int h)
{
    VideoHeight = h;
    VideoWidth = w;
}

void Controler::setDisplaySize(int w, int h)
{
    DisplayHeight = h;
    DisplayWidth = w;
}


int Controler::getPosition(int frame, int O_id)
{
    int i = 0;
    while(i < mEvents[O_id].size() && mEvents[O_id][i].getFrame() < frame){
        i++;
    }
    return i;
}

QRect Controler::getRect(int frame, int O_id){

    int lastID = getPosition(frame, O_id)-1;

    if(lastID == -1){
        return QRect(0,0,0,0);
    }else if(lastID == mEvents[O_id].size()-1 || mEvents[O_id][lastID].getFrame()==frame){
        int x = mEvents[O_id][lastID].mX;
        int y = mEvents[O_id][lastID].mY;
        int w = mEvents[O_id][lastID].mW;
        int h = mEvents[O_id][lastID].mH;

        return QRect(x,y,w,h);
    }else{
        double scall = getScall(); // ToDO: skallieren ins Kammerasystem
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

        return QRect(x+x_diff,
                     y+y_diff,
                     w+w_diff,
                     h+h_diff);
    }
}

void Controler::addObject()
{
    mEvents.push_back(std::vector<ActivModel>());
    mEvents.back().clear();
}

void Controler::setObjectSize(int anz)
{
    for(int i = mEvents.size(); i < anz; i++){
        addObject();
    }
}

void Controler::loadFromFile(QString name){
    loadFromFile(name, "./data/");
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
                addObject();
            }
            mEvents[am.mObjectID].push_back(am);
        }
    }
    file_object.close();
}

void Controler::save(QString name){
    save(name,"./data/");
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
