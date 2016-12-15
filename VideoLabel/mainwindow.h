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

private:
    Ui::MainWindow *ui;

    QMediaPlayer* player;
    QVideoWidget* vw;

    Loader mLoader;
    Controler mControler;

    QString mFileName;

    void displayEvent(int id);
    void displayObject(int id);

};

#endif // MAINWINDOW_H
