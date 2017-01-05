#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QFileDialog>
#include <QProgressBar>
#include <QSlider>
#include <QDebug>

#include "control/loader.h"
#include "control/controler.h"
#include "control/xmlloader.h"
#include "control/myvideoplayer.h"

#include "objectdialog.h"
#include "eventdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionPlay_triggered();

    void on_actionPause_triggered();

    void on_actionStop_triggered();

    void myclick_on_Slider(int newPos);

    void on_listWidget_1_clicked(const QModelIndex &index);

    void on_listWidget_2_clicked(const QModelIndex &index);

    void resizeEvent(QResizeEvent *);

    void Mouse_Released();

    void VideoPositionChange(int pos);

    void VideoLengthChange(int size);

    void newVideoFrame(QImage frame);

    void on_actionSave_triggered();

    void on_actionAddEvent_triggered();

    void on_actionAdd_Object_triggered();

    void updateView();

    void contextEventMenuRequested(const QPoint& point);

    void contextObjectMenuRequested(const QPoint& point);

    void Eventchange();

    void Eventdelete();

    void Objectchange();

    void Objectdelete();

    void on_actionImport_XML_triggered();

    void on_actionStepForward_triggered();

    void on_actionStepBackward_triggered();

    void on_actionSkipForward_triggered();

    void on_actionSkipBackward_triggered();

private:
    Ui::MainWindow *ui;

    Loader mLoader;
    Controler mControler;
    XMLLoader *mXMLLoader;

    QString mFileName;

    QMenu* mEventMenu;
    std::vector<QAction*> mEventMenueAction;

    QMenu* mObjectMenu;
    std::vector<QAction*> mObjectMenueAction;

    void displayEvent(int id);
    void displayObject(int id);

    ObjectDialog *mObjectDialog;
    EventDialog *mEventDialog;

    void clearAll();

    void updateSelection();

    void keyPressEvent(QKeyEvent *);

    MyVideoPlayer* mPlayer;
};

#endif // MAINWINDOW_H
