#include "controler.h"

#include <math.h>

#include <iostream>
#include <QFile>
#include <QTextStream>
#include <algorithm>
#include <QMessageBox>
#include <QPainter>

Controler::Controler()
{
    VideoHeight = VideoWidth = 0;
    DisplayHeight = DisplayWidth = 0;
    ShiftX = ShiftY = 0;
    mFrames.clear();
    mScall = 1.0;
}

void Controler::addEvent(int x1, int y1, int x2, int y2, int frameNr, int E_id, int O_id)
{
    int x = std::min(x1,x2);
    int w = std::max(x1,x2)-x;
    int y = std::min(y1,y2);
    int h = std::max(y1,y2)-y;

    WindoRectToVideoRect(x,y,w,h);

    std::cout<<"Aufruf mit: "<<x<<" "<<y<<" "<<w<<" "<<h<<" "<<frameNr<<" "<<E_id<<" "<<O_id<<std::endl;

    addObjectInFrame(x,y,w,h,frameNr,E_id,O_id,true);
}

void Controler::calculateParameter()
{
    mScall = std::min((double)DisplayHeight/(double)VideoHeight,(double)DisplayWidth/(double)VideoWidth);
    ShiftX = (DisplayWidth-VideoWidth*mScall)/2;
    ShiftY = (DisplayHeight-VideoHeight*mScall)/2;
    std::cout<<"Parameter: "<<mScall<<" "<<ShiftX<<" "<<ShiftY<<std::endl;
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

void Controler::printAll()
{
    for(size_t i = 0; i < mFrames.size(); i++){
        for(size_t j = 0; j<mFrames[i].getObjectSize(); j++){
            std::cout<<"["<<i<<","<<j<<"] "<<mFrames[i].printAll(j).toStdString()<<std::endl;
        }
        std::cout<<std::endl;
    }
}

void Controler::clearAll()
{
    for(size_t i = 0; i < mFrames.size(); i++){
        mFrames[i].clear();
    }
    mFrames.clear();

    mEvents.clear();
    mBehaviors.clear();
    mObjects.clear();
}

void Controler::WindoRectToVideoRect(int &x, int &y, int &w, int &h)
{
    x = (x-ShiftX)/mScall+0.5;
    y = (y-ShiftY)/mScall+0.5;
    w = w/mScall+0.5;
    h = h/mScall+0.5;
}


void Controler::detectJumpBox(int obj_ID, QWidget *parent, MyVideoPlayer *player){
    size_t i = 0;
    //Überspringe bis erstes mal gefunden
    std::cout<<"Suche erstes Element von "<<obj_ID<<std::endl;
    while (i < mFrames.size() && !mFrames[i].existObject(obj_ID)) {
        i++;
    }
    size_t frame_l = mFrames[i].getFrameNr();
    int x,y,w,h;
    mFrames[i].getRect(obj_ID,x,y,w,h);
    i++;

    while (i < mFrames.size()) {
        size_t objPos;
        if(mFrames[i].existObject(obj_ID,objPos)){
            if(mFrames[i].samePositionPos(objPos,x,y,w,h,30)){
                mFrames[i].getRect(obj_ID,x,y,w,h);
                frame_l = mFrames[i].getFrameNr();
            }else{
                int xn,yn,wn,hn;
                mFrames[i].getRect(obj_ID,xn,yn,wn,hn);
                size_t frame_n = mFrames[i].getFrameNr();
                int work = samePerson(frame_l,cv::Rect(x,y,w,h),frame_n,cv::Rect(xn,yn,wn,hn), parent,player);
                if(work == 1){
                    x = xn; y = yn; w = wn; h = hn;
                    frame_l = frame_n;
                }else if(work == 0){
                    size_t newID = getNextAutoNameID();
                    mFrames[i].setObjectID(obj_ID, newID);
                    i++;
                    while (i < mFrames.size() && mFrames[i].existObject(obj_ID,objPos)) {
                        if(mFrames[i].samePositionPos(objPos,xn,yn,wn,hn,30)){
                            mFrames[i].getRect(obj_ID,xn,yn,wn,hn);
                            mFrames[i].setObjectID(obj_ID, newID);
                        }else{
                            i--;
                            break;
                        }
                        i++;
                    }
                }else{
                    return;
                }
            }
        }
        i++;
    }
}

void Controler::detectMultibleObject()
{
    bool IDused = true;
    size_t newID;
    for(size_t i = 0; i < mFrames.size(); i++){
        for(size_t j = 0; j < mFrames[i].getObjectSize(); j++){
            std::vector<size_t> doppelt = mFrames[i].SeveralTimesObject(j);
            if(doppelt.size() >= 1){
                doppelt.push_back(j);

                int obj_ID = mFrames[i].getObjectID(j);
                int x,y,w,h;
                //Auswahl der korrekten Box
                if(i >= 1 && mFrames[i-1].existObject(obj_ID)){
                    mFrames[i-1].getRect(obj_ID,x,y,w,h);
                }else{
                    mFrames[i].getRectPos(j,x,y,w,h);
                }

                bool orginal = true;
                for(size_t pos = 0; pos < doppelt.size(); pos++){
                    if(orginal && mFrames[i].samePositionPos(doppelt[pos],x,y,w,h,30)){
                        orginal = false;
                    }else{
                        if(IDused){
                            newID = getNextAutoNameID();
                            IDused = false;
                        }
                        size_t objNr;
                        int xn,yn,wn,hn;
                        mFrames[i].getRectPos(doppelt[pos],xn,yn,wn,hn);
                        for(size_t run = i; run < mFrames.size(); run++){
                            if(mFrames[run].samePosition(obj_ID,xn,yn,wn,hn,30,objNr)){
                                mFrames[run].setObjectIDPos(objNr, newID);
                                mFrames[run].getRectPos(objNr,xn,yn,wn,hn);
                                IDused = true;
                            }else{
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Controler::mergeObject(int obj_ID, QWidget *parent, MyVideoPlayer *player)
{
    int i = 0;
    //Überspringe bis erstes mal gefunden
    std::cout<<"Suche erstes Element "<<obj_ID<<std::endl;
    while (i < mFrames.size() && !mFrames[i].existObject(obj_ID)) {
        i++;
    }
    int x_start,y_start,w_start,h_start;
    mFrames[i].getRect(obj_ID,x_start,y_start,w_start,h_start);
    size_t frame_l = mFrames[i].getFrameNr();

    std::vector<int> ban;

    //Suche nach passenen Objecten
    for(int step = -1; step <= 1; step+=2){
        int x,y,w,h;
        std::cout<<"Merge: "<<step<<std::endl;
        for(int j = i; j >= 0 && j < mFrames.size(); j+=step){
            if(mFrames[j].existObject(obj_ID)){
                mFrames[j].getRect(obj_ID,x,y,w,h);
                frame_l = mFrames[j].getFrameNr();
            }else{
                std::vector<int> posible = mFrames[j].getObjectOnPosition(x, y, w, h, 30);
                for(size_t id = 0; id < posible.size(); id++){
                    bool ok = true;
                    for(size_t r = 0; r < ban.size(); r++){
                        if(posible[id] == ban[r]){
                            ok = false;
                            break;
                        }
                    }
                    if(ok){
                        int xn,yn,wn,hn;
                        mFrames[j].getRect(posible[id],xn,yn,wn,hn);
                        size_t frame_n = mFrames[j].getFrameNr();
                        int work = samePerson(frame_l,cv::Rect(x,y,w,h),frame_n,cv::Rect(xn,yn,wn,hn), parent,player);
                        if(work == 1){ //Gleich
                            setAllObject(posible[id], obj_ID);
                            deleteObject(posible[id]);
                            deleteObjectID(posible[id]);
                            break;
                        }else if(work == 0){ // abbruch
                            ban.push_back(posible[id]);
                        }else{
                            return;
                        }
                    }
                }
            }
        }
    }
    std::cout<<"Ende Meage"<<std::endl;
}

void Controler::fixGesData(QWidget *parent, MyVideoPlayer *player)
{
    size_t tmp = mObjects.size();
    std::cout<<"Fix Data - Splitt"<<std::endl;
    detectMultibleObject();
    std::cout<<"Fix Data - Jump ("<<mObjects.size()-tmp<<" neu)"<<std::endl;
    for(size_t i = 0; i < mObjects.size(); i++){
        detectJumpBox(i, parent, player);
    }
    std::cout<<"Fix Data - Merge"<<std::endl;
    if(QMessageBox::question(parent,"Quit","Wollen sie Abbrechen?",QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes){
        return;
    }
    size_t anz = 0;
    while(anz != mObjects.size()){
        anz = mObjects.size();
        for(size_t i = 0; i < mObjects.size(); i++){
            if(i%50 == 0 && QMessageBox::question(parent,"Quit","Wollen sie Abbrechen?",QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes){
                return;
            }
            mergeObject(i, parent, player);
        }
    }
    std::cout<<"Fix Data - Ende"<<std::endl;
}

void Controler::deleatEmptyObject()
{
    for(size_t i = 0; i < mObjects.size(); i++){
        if(!isObjectUsed(i)){
            deleteObject(i);
            i--;
        }
    }
}

int Controler::samePerson(size_t frame_l, cv::Rect box_l, size_t frame_r, cv::Rect box_r, QWidget *parent, MyVideoPlayer *player)
{
    QImage img_l = player->getFrame(frame_l);
    QImage img_r = player->getFrame(frame_r);

    QImage copy_l = img_l.copy(getPrintBox(img_l.width(), img_l.height(), box_l.x, box_l.y, box_l.height, box_l.width)).scaled(300,400,Qt::KeepAspectRatio);
    QImage copy_r = img_r.copy(getPrintBox(img_r.width(), img_r.height(), box_r.x, box_r.y, box_r.height, box_r.width)).scaled(300,400,Qt::KeepAspectRatio);

    QPixmap result(600,400);
    result.fill(Qt::transparent);
    QPainter painter(&result);
    painter.drawPixmap(0, 0, QPixmap::fromImage(copy_l));
    painter.drawPixmap(300,0, QPixmap::fromImage(copy_r));
    painter.end();

    QMessageBox about_box(parent);
    about_box.setWindowTitle("Handelt es sich um die selbe Person? ["+QString::number(frame_l)+" "+QString::number(frame_r)+"]");
    about_box.setIconPixmap(result);
    about_box.setStandardButtons(QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    about_box.clearFocus();

    int ret = about_box.exec();

    if(ret == QMessageBox::Yes){
        return 1;
    }else if(ret == QMessageBox::No){
        return 0;
    }else{
        return -1;
    }
}

QRect Controler::getPrintBox(int Img_width, int Img_height, int X, int Y, int H, int W)
{
    QRect ret;
    ret.setX(std::max(0,X-W/4));
    ret.setY(std::max(0,Y-H/4));
    ret.setWidth(W*1.5);
    ret.setHeight(H*1.5);
    if(Img_width < ret.x()+ret.width()){
        ret.setWidth(Img_width-ret.x());
    }
    if(Img_height < ret.y()+ret.height()){
        ret.setHeight(Img_height-ret.y());
    }
    return ret;
}
