#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mylabel.h"
#include <QPainter>
#include <QFileInfo>

#include "model/event.h"

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

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(myclick_on_Slider(int)));

    ui->actionPause->setVisible(false);
    ui->actionPlay->setVisible(true);

    connect(ui->labelVideo,SIGNAL(Mouse_Released()),this,SLOT(Mouse_Released()));

    mObjectDialog = new ObjectDialog(this, &mLoader);
    mEventDialog = new EventDialog(this, &mLoader);

    connect(mObjectDialog,SIGNAL(accepted()),this,SLOT(updateView()));
    connect(mEventDialog,SIGNAL(accepted()),this,SLOT(updateView()));

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
    if(!filename.isEmpty() && filename.size() > 0){
        QFileInfo fi(filename);

        this->setWindowTitle("VideoLabel - " + fi.fileName());
        mFileName = fi.baseName();

        clearAll();

        //mLoader.loadFromFile(mFileName,"./data/");
        //mControler.loadFromFile(mFileName, "./data/");
        mXMLLoader->read(mFileName, "./data/");

        updateView();

        on_actionStop_triggered();

        mPlayer->setPath(filename);
        mPlayer->getFrame();

        mControler.setDisplaySize(ui->labelVideo->size().width(),ui->labelVideo->size().height());

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
    bool clickOnSlider = (QApplication::mouseButtons() && Qt::LeftButton) &&
            (localMousePos.x() >= 0 && localMousePos.y() >= 0 &&
             localMousePos.x() < ui->horizontalSlider->size().width() &&
             localMousePos.y() < ui->horizontalSlider->size().height());
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
    Event ev = mLoader.getEvent(id);
    QString text = "<table><tr><th align=left>Name:</th> <td>"+ev.getName()+"</td></tr>";
    text += "<tr> <th align=left>ID:</th> <td>"+QString::number(ev.getID())+"</td></tr> </table>";
    if(ev.OnTask()){
        text += "<br><b>On-Task</b>";
    }else{
        text += "<br><b>Off-Task</b>";
    }

    if(ev.getEyeVontact())
        text += "<br>+ Blickkontakt zum legitimen Sprecher oder Objekt";
    if(ev.getActiveParticipation())
        text += "<br>+ Aktive Beteiligung an der Aufgabe";
    if(ev.getOtherActivities())
        text += "<br>- Ausübung anderer Tätigkeiten";
    if(ev.getRestlessness())
        text += "<br>- Motorische Unruhe";
    if(ev.getCommunication())
        text += "<br>- Themenferne Kommunikation";

    text += "<br>"+ev.getDescription();
    ui->textBrowser->setHtml(text);
}

void MainWindow::displayObject(int id){
    QStringList data = mLoader.getObject(id);
    QString text = "<table><tr><th align=left>Name:</th> <td>"+data[0]+"</td></tr>";
    text += "<tr> <th align=left>ID:</th> <td>"+data[1]+"</td></tr> </table>";
    text += "<p>"+data[2]+ "</p>";
    ui->textBrowser->setHtml(text);
}

void MainWindow::clearAll()
{
    mLoader.clearAll();
    mControler.clearAll();
    mObjectDialog->clear();
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
        ui->listWidget_2->clearSelection();
    }
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
    }else if(event->key() == Qt::Key_D){
        on_actionStepForward_triggered();
    }else if(event->key() == Qt::Key_A){
        on_actionStepBackward_triggered();
    }else if(event->key() == Qt::Key_W){
        on_actionSkipForward_triggered();
    }else if(event->key() == Qt::Key_S){
        on_actionSkipBackward_triggered();
    }else if(event->key() == Qt::Key_N){
        on_actionAdd_Object_triggered();
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
        mControler.addEvent(mPlayer->getPosition(),ui->listWidget_1->currentRow(),ui->listWidget_2->currentRow());
        ui->listWidget_2->clearFocus();
    }
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    QMainWindow::resizeEvent(ev);
    mControler.setDisplaySize(ui->labelVideo->size().width(),ui->labelVideo->size().height());
    mPlayer->getFrame();
}


void MainWindow::Mouse_Released()
{
    ui->textBrowser->setText(QString("Released Label %1 %2 -> %3 %4").arg(ui->labelVideo->x).arg(ui->labelVideo->y).arg(ui->labelVideo->lastX).arg(ui->labelVideo->lastY));

    mControler.addEvent(ui->labelVideo->lastX,
                        ui->labelVideo->lastY,
                        ui->labelVideo->x,
                        ui->labelVideo->y,
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
    ui->labelVideo->clear();
    int newPos = mPlayer->getPosition();

    QPixmap img = QPixmap::fromImage(frame);

    QPainter paint(&img);

    for(int i = 0; i < ui->listWidget_1->count(); i++){
        int evID = -1;
        QRect rec = mControler.getRect(newPos,i, evID);
        if(i == ui->listWidget_1->currentIndex().row()){
            if(evID == ui->listWidget_2->currentIndex().row()){
                paint.setPen(QColor(Qt::blue));
            }else{
                paint.setPen(QColor(255,0,255,255));
            }
        }else{
            paint.setPen(QColor(Qt::green));
        }
        paint.drawRect(rec);
    }
    paint.end();
    QPixmap img2 = img.scaled(ui->labelVideo->size().width(),
                              ui->labelVideo->size().height(),
                              Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->labelVideo->setPixmap(img2);

    updateSelection();
}

void MainWindow::on_actionSave_triggered()
{
    mXMLLoader->write(mFileName,"./data/");
    mLoader.save(mFileName,"./data/");
    mControler.save(mFileName,"./data/");
}

void MainWindow::on_actionAddEvent_triggered()
{
    mEventDialog->setWindowTitle("Add new Event");
    mEventDialog->clear();
    mEventDialog->setID(mLoader.getEventSize());
    mEventDialog->show();

}

void MainWindow::on_actionAdd_Object_triggered()
{
    mObjectDialog->setWindowTitle("Add new Object");
    mObjectDialog->clear();
    mObjectDialog->setID(mLoader.getObjectSize());
    mObjectDialog->show();
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
    Event ev = mLoader.getEvent(id);
    mEventDialog->setWindowTitle("Change Event");
    mEventDialog->setAttribute(id, ev.getName(), ev.getDescription());
    mEventDialog->show();
}

void MainWindow::Objectchange(){
    int id = ui->listWidget_1->currentIndex().row();
    QStringList all = mLoader.getObject(id);
    mObjectDialog->setWindowTitle("Change Object");
    mObjectDialog->setAttribute(id, all[0], all[2]);
    mObjectDialog->show();
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
