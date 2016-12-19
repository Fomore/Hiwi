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

#include "inputevobdialog.h"

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

    void Mouse_current_Pose();
    void Mouse_Pressed();
    void Mouse_Released();

    void newVideoFrame(qint64 newPos);

    void on_actionSave_triggered();

    void on_actionAddEvent_triggered();

    void on_actionAdd_Object_triggered();

    void updateView();

    void contextEventMenuRequested(const QPoint& point);

    void contextObjectMenuRequested(const QPoint& point);

    void Eventchange();

    void Objectchange();

    void on_actionMute_triggered();

    void on_actionSound_triggered();

    void on_actionImport_XML_triggered();

    void on_actionStepForward_triggered();

    void on_actionStepBackward_triggered();

    void on_actionSkipForward_triggered();

    void on_actionSkipBackward_triggered();

private:
    Ui::MainWindow *ui;

    QMediaPlayer* player;
    QVideoWidget* vw;

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

    InputEvObDialog *mEvObDialog;

    void clearAll();

    void updateSelection();
    void updateRects();

    void keyPressEvent(QKeyEvent *);
};

#endif // MAINWINDOW_H
