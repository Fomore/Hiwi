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
    std::cout<<"Aufruf mit: "<<x1<<" "<<y1<<" "<<x2<<" "<<y2<<" "<<frameNr<<" "<<E_id<<" "<<O_id<<std::endl;
    int x = std::min(x1,x2);
    int w = std::max(x1,x2)-x;
    int y = std::min(y1,y2);
    int h = std::max(y1,y2)-y;

    WindoRectToVideoRect(x,y,w,h);

    addObjectInFrame(x,y,w,h,frameNr,E_id,O_id,true);
}

void Controler::calculateParameter()
{
    mScall = std::min((double)DisplayHeight/(double)VideoHeight,(double)DisplayWidth/(double)VideoWidth);
    ShiftX = (DisplayWidth-VideoWidth*mScall)/2;
    ShiftY = (DisplayHeight-VideoHeight*mScall)/2;
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


void Controler::detectDataError(int obj_ID, QWidget *parent, MyVideoPlayer *player){
    std::cout<<"Suche nach Sprüngen im Tracking"<<std::endl;

    size_t i = 0;
    //Überspringe bis erstes mal gefunden
    std::cout<<"Suche erstes Element"<<std::endl;
    while (i < mFrames.size() && !mFrames[i].existObject(obj_ID)) {
        i++;
    }
    size_t frame_l = mFrames[i].getFrameNr();
    int x,y,w,h;
    mFrames[i].getRect(obj_ID,x,y,w,h);
    i++;

    while (i < mFrames.size()) {
        size_t objNr;
        if(mFrames[i].existObject(obj_ID,objNr)){
            if(mFrames[i].samePosition(objNr,x,y,w,h,30)){
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
                    std::cout<<"Ändere OBjekt-Name "<<frame_n<<std::endl;
                }else{
                    return;
                }
            }
        }
        i++;
    }
}

void Controler::detectDataError2(int obj_ID, QWidget *parent, MyVideoPlayer *player)
{
    /*
    size_t frame = 3253;
    cv::Rect rec_l(170, 533, 47, 64);
    cv::Rect rec_r(1190, 454, 58, 78);
    */

    size_t i = 0;
    //Überspringe bis erstes mal gefunden
    std::cout<<"Suche erstes Element"<<std::endl;
    while (i < mFrames.size() && !mFrames[i].existObject(obj_ID)) {
        i++;
    }
    std::cout<<"Suche letztes Element ab "<<i<<std::endl;
    //Durchlaufe alle aufeinanderfolgenden vorhandenen
    while (i < mFrames.size() && mFrames[i].existObject(obj_ID)) {
        i++;
    }

    std::cout<<"Suche verknüpfung "<<i<<std::endl;
    if(i < mFrames.size()){
        size_t frame_l = mFrames[i-1].getFrameNr();
        cv::Rect rec_l;
        mFrames[i-1].getRect(obj_ID,rec_l.x,rec_l.y, rec_l.width, rec_l.height);
        std::vector<int> list;

        do {
            list.clear();
            list = mFrames[i].getObjectOnPosition(rec_l.x,rec_l.y, rec_l.width, rec_l.height, 30);
            i++;
        } while (i < mFrames.size() && list.size() > 0);
        std::cout<<"Neues Element: "<<i<<std::endl;
        for(size_t j = 0; j < list.size(); j++){
            size_t frame_n = mFrames[i-1].getFrameNr();
            cv::Rect rec_n;
            mFrames[i-1].getRect(list[j],rec_n.x,rec_n.y, rec_n.width, rec_n.height);
            int work = samePerson(frame_l,rec_l,frame_n,rec_n, parent,player);
            if(work == 1){
                std::cout<<"Verknüpfen"<<std::endl;
            }else if(work == 0){
                           std::cout<<"Tennen"<<std::endl;
            }else{
                std::cout<<"Nichts"<<std::endl;
            }
        }
    }
    std::cout<<"Fertig"<<std::endl;
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
    about_box.setWindowTitle("Handelt es sich um die selbe Person?");
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
