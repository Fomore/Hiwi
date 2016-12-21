#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mywidget.h"
#include <QPainter>
#include <QFileInfo>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mPlayer = new MyVideoPlayer();

    QObject::connect(mPlayer,SIGNAL(legthChanged(int)),this,SLOT(VideoLengthChange(int)));
    QObject::connect(mPlayer,SIGNAL(positionChanger(int)),this,SLOT(VideoPositionChange(int)));
    QObject::connect(ui->horizontalSlider,SIGNAL(sliderMoved(int)),mPlayer,SLOT(setPosition(int)));
    connect(mPlayer,SIGNAL(isNewImage(QImage)),this,SLOT(newVideoFrame(QImage)));

    /*connect(player,&QMediaPlayer::durationChanged,ui->horizontalSlider,&QSlider::setMaximum);
    connect(player,&QMediaPlayer::positionChanged,ui->horizontalSlider,&QSlider::setValue);
    connect(ui->horizontalSlider,&QSlider::sliderMoved,player,&QMediaPlayer::setPosition);
*/
    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(myclick_on_Slider(int)));

    ui->actionPause->setVisible(false);
    ui->actionPlay->setVisible(true);

    connect(ui->widgetVideo,SIGNAL(Mouse_Released()),this,SLOT(Mouse_Released()));


    mEvObDialog = new InputEvObDialog(this, &mLoader);

    connect(mEvObDialog,SIGNAL(accepted()),this,SLOT(updateView()));

    mEventMenu = new QMenu(ui->listWidget_2);
    ui->listWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_2, SIGNAL(customContextMenuRequested(const QPoint)),this,
            SLOT(contextEventMenuRequested(const QPoint)));
    mEventMenueAction.push_back(mEventMenu->addAction("Ändern"));
    connect(mEventMenueAction[0],SIGNAL(triggered()),this,SLOT(Eventchange()));

    mObjectMenu = new QMenu(ui->listWidget_1);
    ui->listWidget_1->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_1, SIGNAL(customContextMenuRequested(const QPoint)),this,
            SLOT(contextObjectMenuRequested(const QPoint)));
    mObjectMenueAction.push_back(mObjectMenu->addAction("Ändern"));
    connect(mObjectMenueAction[0],SIGNAL(triggered()),this,SLOT(Objectchange()));

    mXMLLoader = new XMLLoader(&mLoader,&mControler);

    QAction *strgS =new QAction("save",this);
    strgS->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(strgS, SIGNAL(triggered()), this, SLOT(on_actionSave_triggered()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Video"), "~", tr("Video Files (*.avi *.mp4 *.wmv);; All (*.*)"));
    if(!filename.isEmpty()){
        QFileInfo fi(filename);

        this->setWindowTitle("VideoLabel - " + fi.fileName());
        mFileName = fi.baseName();

        clearAll();

        mLoader.loadFromFile(mFileName);
        mControler.loadFromFile(mFileName);

        updateView();

        on_actionStop_triggered();

        mPlayer->setPath(filename);

        mControler.setDisplaySize(ui->widgetVideo->size().width(),ui->widgetVideo->size().height());

        int w = mPlayer->getVideoWidth();
        int h = mPlayer->getVideoHeight();
        if(w > 0 && h > 0){
            mControler.setVideoSize(w,h);
        }else{
            mControler.setVideoSize(640,480);
        }
    }
}

void MainWindow::on_actionPlay_triggered()
{
    ui->statusBar->showMessage("Play");
    ui->actionPlay->setVisible(false);
    ui->actionPause->setVisible(true);
    updateRects();

    mPlayer->play();
}

void MainWindow::on_actionPause_triggered()
{
    ui->statusBar->showMessage("Pause");
    ui->actionPlay->setVisible(true);
    ui->actionPause->setVisible(false);

    mPlayer->pause();
}

void MainWindow::on_actionStop_triggered()
{
    ui->statusBar->showMessage("Stop");

    ui->actionPlay->setVisible(true);
    ui->actionPause->setVisible(false);
    mPlayer->stop();
}

void MainWindow::myclick_on_Slider(int newPos){
    QPoint localMousePos = ui->horizontalSlider->mapFromGlobal(QCursor::pos());
    bool clickOnSlider = (QApplication::mouseButtons() & Qt::LeftButton);
    if (clickOnSlider){
        // Attention! The following works only for Horizontal, Left-to-right sliders
        double posRatio = localMousePos.x() / (double)ui->horizontalSlider->size().width();
        int sliderRange = ui->horizontalSlider->maximum() - ui->horizontalSlider->minimum();
        int sliderPosUnderMouse = ui->horizontalSlider->minimum() + sliderRange * posRatio;
        if (sliderPosUnderMouse != newPos)
        {
            ui->horizontalSlider->setValue(sliderPosUnderMouse);
            mPlayer->setPosition(sliderPosUnderMouse);
            return;
        }
    }
}

void MainWindow::displayEvent(int id){
    QStringList data = mLoader.getEvent(id);
    QString text = "<table><tr><th align=left>Name:</th> <td>"+data[0]+"</td></tr>";
    text += "<tr> <th align=left>ID:</th> <td>"+data[1]+"</td></tr> </table>";
    text += data[2];
    ui->textBrowser->setHtml(text);
}

void MainWindow::displayObject(int id){
    QStringList data = mLoader.getObject(id);
    QString text = "<table><tr><th align=left>Name:</th> <td>"+data[0]+"</td></tr>";
    text += "<tr> <th align=left>ID:</th> <td>"+data[1]+"</td></tr> </table>";
    text += data[2];
    ui->textBrowser->setHtml(text);
}

void MainWindow::clearAll()
{
    mLoader.clearAll();
    mControler.clearAll();
    mEvObDialog->clear();
    ui->listWidget_2->clear();
    ui->listWidget_1->clear();

}

void MainWindow::updateSelection()
{
    int frame = mPlayer->getPosition();
    int O_id = ui->listWidget_1->currentIndex().row();
    int E_id = mControler.getEventToObject(frame,O_id);
    if(E_id >= 0 && E_id < ui->listWidget_2->count()){
        ui->listWidget_2->item(E_id)->setSelected(true);
    }else{
        ui->listWidget_2->clearFocus();
    }
}

void MainWindow::updateRects()
{
    int newPos = mPlayer->getPosition();
    ui->widgetVideo->clearRects();
    for(int i = 0; i < ui->listWidget_1->count(); i++){
        int evID = -1;
        QRect rec = mControler.getRect(newPos,i, evID);
        if(i == ui->listWidget_1->currentIndex().row()){
            if(evID == ui->listWidget_2->currentIndex().row()){
                ui->widgetVideo->addRect(rec,QColor(Qt::blue));
            }else{
                ui->widgetVideo->addRect(rec,QColor(255,0,255,255));
            }
        }else{
            ui->widgetVideo->addRect(rec);
        }
    }
    ui->widgetVideo->repaint();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{

    QMainWindow::keyPressEvent(event);
    if(event->key() == Qt::Key_Space){
        if(ui->actionPlay->isVisible()){
            on_actionPlay_triggered();
        }else if(ui->actionPause->isVisible()){
            on_actionPause_triggered();
        }
    }
}

void MainWindow::on_listWidget_1_clicked(const QModelIndex &index)
{
    displayObject(index.row());
    updateSelection();
}

void MainWindow::on_listWidget_2_clicked(const QModelIndex &index)
{
    displayEvent(index.row());
    if(ui->checkBoxEvent->isChecked()){
        std::cout<<"Ändere Event"<<std::endl;
    }
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    QMainWindow::resizeEvent(ev);
    mControler.setDisplaySize(ui->widgetVideo->size().width(),ui->widgetVideo->size().height());
    updateRects();
}

void MainWindow::Mouse_current_Pose()
{
    ui->textBrowser->setText(QString("Klick: %1/%2").arg(ui->widgetVideo->x).arg(ui->widgetVideo->y));
}

void MainWindow::Mouse_Pressed()
{
    ui->textBrowser->setText(QString("Pressed %1 %2").arg(ui->widgetVideo->x).arg(ui->widgetVideo->y));
}


void MainWindow::Mouse_Released()
{
    ui->textBrowser->setText(QString("Released %1 %2 -> %3 %4").arg(ui->widgetVideo->x).arg(ui->widgetVideo->y).arg(ui->widgetVideo->lastX).arg(ui->widgetVideo->lastY));

    mControler.addEvent(ui->widgetVideo->lastX,
                        ui->widgetVideo->lastY,
                        ui->widgetVideo->x,
                        ui->widgetVideo->y,
                        mPlayer->getPosition(),
                        ui->listWidget_2->currentIndex().row(),
                        ui->listWidget_1->currentIndex().row());
}

void MainWindow::VideoPositionChange(int pos)
{
    ui->horizontalSlider->setValue(pos);
}

void MainWindow::VideoLengthChange(int size)
{
    ui->horizontalSlider->setMaximum(size);
}

void MainWindow::newVideoFrame(QImage frame)
{
    QImage img2 = frame.scaled(ui->labelVideo->size().width(),
                               ui->labelVideo->size().height(),
                               Qt::KeepAspectRatio);
    ui->labelVideo->setPixmap(QPixmap::fromImage(img2));

    updateRects();
    updateSelection();
}

void MainWindow::on_actionSave_triggered()
{
    mLoader.save(mFileName);
    mControler.save(mFileName);
}

void MainWindow::on_actionAddEvent_triggered()
{
    mEvObDialog->setWindowTitle("Add new Event");
    mEvObDialog->setUseEvent();
    mEvObDialog->clear();
    mEvObDialog->setID(mLoader.getEventSize());
    mEvObDialog->show();

}

void MainWindow::on_actionAdd_Object_triggered()
{
    mEvObDialog->setWindowTitle("Add new Object");
    mEvObDialog->setUseObject();
    mEvObDialog->clear();
    mEvObDialog->setID(mLoader.getObjectSize());
    mEvObDialog->show();
}

void MainWindow::updateView()
{
    ui->listWidget_2->clear();
    QStringList EventList = mLoader.getEventAllName();
    for(int i = 0; i < EventList.size(); i++){
        ui->listWidget_2->addItem(EventList[i]);
    }

    ui->listWidget_1->clear();
    QStringList ObjectList = mLoader.getObjectAllName();
    for(int i = 0; i < ObjectList.size(); i++){
        ui->listWidget_1->addItem(ObjectList[i]);
    }
    mControler.setObjectSize(ObjectList.size());
}

void MainWindow::contextEventMenuRequested(const QPoint &point)
{
    QModelIndex t = ui->listWidget_2->indexAt(point);
    if(t.row() >= 0){
        mEventMenu->popup(ui->listWidget_2->mapToGlobal(point));
        ui->listWidget_2->item(t.row())->setSelected(true);
    }
}

void MainWindow::contextObjectMenuRequested(const QPoint &point)
{
    QModelIndex t = ui->listWidget_1->indexAt(point);
    if(t.row() >= 0){
        mObjectMenu->popup(ui->listWidget_1->mapToGlobal(point));
        ui->listWidget_1->item(t.row())->setSelected(true);
    }
}

void MainWindow::Eventchange()
{
    int id = ui->listWidget_2->currentIndex().row();
    QStringList all = mLoader.getEvent(id);
    mEvObDialog->setWindowTitle("Change Event");
    mEvObDialog->setUseEvent();
    mEvObDialog->setEvOb(id, all[0], all[2]);
    mEvObDialog->show();
}

void MainWindow::Objectchange(){
    int id = ui->listWidget_1->currentIndex().row();
    QStringList all = mLoader.getObject(id);
    mEvObDialog->setWindowTitle("Change Object");
    mEvObDialog->setEvOb(id, all[0], all[2]);
    mEvObDialog->setUseObject();
    mEvObDialog->show();
}

void MainWindow::on_actionImport_XML_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open XML-Datei"), "~", tr("XML (*.xml);; All (*.*)"));
    mXMLLoader->read(filename);
    updateView();
}

void MainWindow::on_actionStepForward_triggered()
{
    mPlayer->forward();
}

void MainWindow::on_actionStepBackward_triggered()
{
    mPlayer->backward();
}

void MainWindow::on_actionSkipForward_triggered()
{
    mPlayer->skipForward();
}

void MainWindow::on_actionSkipBackward_triggered()
{
    mPlayer->skipBackward();
}
