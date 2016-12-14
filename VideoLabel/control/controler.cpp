#include "controler.h"

#include <math.h>

#include <iostream>

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
    std::cout<<"Aufruf mit: "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<frame<<" "<<E_id<<" "<<O_iD<<std::endl;

    double scall = getScall();
    double x = x1*scall;
    double y = y1*scall;
    double w = x2*scall-x1*scall;
    double h = y2*scall-y1*scall;

    mEvents.push_back(ActivModel(x,y,w,h,frame,E_id,O_iD,mEvents.size()));//ToDo: Position richtig setzen -> Sortiert nach Frames
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

int Controler::getLastID(int frame){
    int ret = -1;
    for(int i = 0; i < mEvents.size(); i++){ // Es muss aufsteigend sortiert sein
        if(frame >= mEvents[i].getFrame()){
            ret = i;
        }else{
            return ret;
        }
    }
    return ret;
}

int Controler::getNextID(int frame){
    int i = 0;
    while(i < (int)mEvents.size()-1 && frame < mEvents[i].getFrame()){
        i++;
    }

    return i;
}

QRect Controler::getRect(int frame){

    int lastID = getLastID(frame);
    int nextID = getNextID(frame);

    if(lastID == -1){
        return QRect(0,0,0,0);
    }else{
        double scall = getScall();
        double x = mEvents[lastID].mX*scall;
        double y = mEvents[lastID].mY*scall;
        double w = mEvents[lastID].mW*scall;
        double h = mEvents[lastID].mH*scall;

        int frame_diff = mEvents[nextID].getFrame() -mEvents[lastID].getFrame();

        if(frame_diff > 0){

            double s = (double) (frame-mEvents[lastID].getFrame())/frame_diff;

            double x_diff = (mEvents[lastID].mX -mEvents[nextID].mX)/scall*s;
            double y_diff = (mEvents[lastID].mY -mEvents[nextID].mY)/scall*s;
            double w_diff = (mEvents[lastID].mW -mEvents[nextID].mW)/scall*s;
            double h_diff = (mEvents[lastID].mH -mEvents[nextID].mH)/scall*s;

            return QRect(x+x_diff,
                         y+y_diff,
                         x+x_diff+w+w_diff,
                         y+y_diff+h+h_diff);

        }else{
            return QRect(x,y,x+w,y+h);
        }
    }
}
