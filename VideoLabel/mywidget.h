#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QEvent>
#include <QDebug>
#include <QPainter>
#include <QtGui>
#include <QtCore>

class MyWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyWidget(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    int x,y;
    int lastX, lastY;

    void addRect(const QRect);
    void addRect(const QRect, const QColor);
    void addRect(int x, int y, int w, int h);

    void setRect(const QRect rec);
    void clearRects();

protected:
    void paintEvent(QPaintEvent *);

    QRect mRectActiv;
    std::vector<QRect> mRects;
    std::vector<QColor> mColores;

signals:
    void Mouse_Pressed();
    void Mouse_Released();
    void Mouse_Pose();
public slots:
};

#endif // MYWIDGET_H
