#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent) :
    QLabel(parent)
{
    mAddRect = false;
}

void MyLabel::mousePressEvent(QMouseEvent *ev)
{
    x = lastX = ev->x();
    y = lastY = ev->y();
    emit Mouse_Pressed();
}

void MyLabel::mouseMoveEvent(QMouseEvent *ev)
{
    x = ev->x();
    y = ev->y();
    emit Mouse_Move();
    if(mAddRect){
        mRectActiv.setRect(x,y,lastX-x,lastY-y);
        this->repaint();
    }
}

void MyLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    x = ev->x();
    y = ev->y();
    emit Mouse_Released();
    if(mAddRect){
    mRectActiv.setRect(x,y,lastX-x,lastY-y);
    this->repaint();
    mRectActiv.setRect(0,0,0,0);
    }
}

void MyLabel::paintEvent(QPaintEvent *ev){
    QLabel::paintEvent(ev);

    QPainter paint(this);
    paint.setPen(QColor(Qt::red));
    paint.drawRect(mRectActiv);
    paint.end();
}

void MyLabel::setRect(const QRect rec)
{
    mRectActiv.setRect(rec.x(),rec.y(),rec.width(),rec.height());
    this->repaint();
}

bool MyLabel::isRecActiv()
{
    return mAddRect;
}

void MyLabel::setAddRect(bool rec)
{
    mAddRect = rec;
}
