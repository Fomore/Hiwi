#include "mywidget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
}

void MyWidget::mousePressEvent(QMouseEvent *ev)
{
    x = lastX = ev->x();
    y = lastY = ev->y();
    emit Mouse_Pose();
}

void MyWidget::mouseMoveEvent(QMouseEvent *ev)
{
    x = ev->x();
    y = ev->y();
    emit Mouse_Pressed();

    mRectActiv.setRect(x,y,lastX-x,lastY-y);
    this->repaint();
}

void MyWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    x = ev->x();
    y = ev->y();
    emit Mouse_Released();

    mRectActiv.setRect(x,y,lastX-x,lastY-y);
    this->repaint();
}

void MyWidget::paintEvent(QPaintEvent *){
    QPen pen1(Qt::red);
    QPen pen2(Qt::green);

    QPainter paint(this);
    paint.setPen(pen1);
    paint.drawRect(mRectActiv);
    paint.setPen(pen2);
    for(int i = 0; i < mRects.size(); i++){
        paint.drawRect(mRects[i]);
    }
    paint.end();
}

void MyWidget::addRect(const QRect rec){
    mRects.push_back(QRect(rec));
}

void MyWidget::addRect(int x, int y, int w, int h)
{
    addRect(QRect(x,y,w,h));
}

void MyWidget::setRect(const QRect rec)
{
    mRectActiv.setRect(rec.x(),rec.y(),rec.width(),rec.height());
}
