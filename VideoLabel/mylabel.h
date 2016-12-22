#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>
#include <QPainter>
#include <QtGui>
#include <QtCore>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    int x,y;
    int lastX, lastY;

    void setRect(const QRect rec);
protected:
    void paintEvent(QPaintEvent *);

    QRect mRectActiv;

signals:
    void Mouse_Pressed();
    void Mouse_Released();
    void Mouse_Move();

public slots:

};

#endif // MYLABEL_H
