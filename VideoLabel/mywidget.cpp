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

    QPainter paint(this);
    paint.setPen(pen1);
    paint.drawRect(mRectActiv);
    for(int i = 0; i < mRects.size(); i++){
        paint.setPen(QPen(mColores[i]));
        paint.drawRect(mRects[i]);
    }
    paint.end();
}

void MyWidget::addRect(const QRect rec){
    addRect(rec, QColor(Qt::green));
}

void MyWidget::addRect(const QRect rec, const QColor col)
{
    mRects.push_back(QRect(rec));
    mColores.push_back(QColor(col));
}

void MyWidget::addRect(int x, int y, int w, int h)
{
    addRect(QRect(x,y,w,h));
}

void MyWidget::setRect(const QRect rec)
{
    mRectActiv.setRect(rec.x(),rec.y(),rec.width(),rec.height());
    this->repaint();
}

void MyWidget::clearRects()
{
    mRects.clear();
    mRectActiv.setRect(0,0,0,0);
    mColores.clear();
}

