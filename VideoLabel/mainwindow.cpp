#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "mylabel.h"
#include <QPainter>
#include <QFileInfo>
#include <QMessageBox>

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
    mActionEventDialog = new ActionEventDialog(this, &mLoader, &mControler);
    mBehaviorDialog = new BehaviorDialog(this, &mLoader);

    connect(mObjectDialog,SIGNAL(accepted()),this,SLOT(updateView()));
    connect(mEventDialog,SIGNAL(accepted()),this,SLOT(updateView()));
    connect(mActionEventDialog,SIGNAL(gotoVideoFrame(int)),this,SLOT(gotoVideoFrame(int)));

    mEventMenu = new QMenu(ui->listWidget_2);
    ui->listWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_2, SIGNAL(customContextMenuRequested(const QPoint)),this,
            SLOT(contextEventMenuRequested(const QPoint)));
    mEventMenueAction.push_back(mEventMenu->addAction("Ändern"));
    mEventMenueAction.push_back(mEventMenu->addAction("Clear Focus"));
    mEventMenueAction.push_back(mEventMenu->addAction("Löschen"));
    connect(mEventMenueAction[0],SIGNAL(triggered()),this,SLOT(Eventchange()));
    connect(mEventMenueAction[1],SIGNAL(triggered()),this,SLOT(EventClearFocus()));
    connect(mEventMenueAction[2],SIGNAL(triggered()),this,SLOT(Eventdelete()));

    mObjectMenu = new QMenu(ui->listWidget_1);
    ui->listWidget_1->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->listWidget_1, SIGNAL(customContextMenuRequested(const QPoint)),this,
            SLOT(contextObjectMenuRequested(const QPoint)));
    mObjectMenueAction.push_back(mObjectMenu->addAction("GoTo No Label"));
    mObjectMenueAction.push_back(mObjectMenu->addAction("Start Behavior"));
    mObjectMenueAction.push_back(mObjectMenu->addAction("Clear Focus"));
    mObjectMenueAction.push_back(mObjectMenu->addAction("Ändern"));
    mObjectMenueAction.push_back(mObjectMenu->addAction("Show Events"));
    mObjectMenueAction.push_back(mObjectMenu->addAction("Löschen"));
    connect(mObjectMenueAction[0],SIGNAL(triggered()),this,SLOT(setNoLabelPosition()));
    connect(mObjectMenueAction[1],SIGNAL(triggered()),this,SLOT(start_behavior()));
    connect(mObjectMenueAction[2],SIGNAL(triggered()),this,SLOT(ObjectClearFocus()));
    connect(mObjectMenueAction[3],SIGNAL(triggered()),this,SLOT(Objectchange()));
    connect(mObjectMenueAction[4],SIGNAL(triggered()),this,SLOT(show_Actionenevent()));
    connect(mObjectMenueAction[5],SIGNAL(triggered()),this,SLOT(Objectdelete()));

    mXMLLoader = new XMLLoader(&mLoader,&mControler);

    QAction *strgS =new QAction("save",this);
    strgS->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    connect(strgS, SIGNAL(triggered()), this, SLOT(on_actionSave_triggered()));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(auto_Save()));
    timer->start(300000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::gotoVideoFrame(int frame)
{
    if(ui->horizontalSlider->minimum() <= frame
            && ui->horizontalSlider->maximum() >= frame){
        ui->horizontalSlider->setValue(frame);
        mPlayer->setPosition(frame);
    }
}


void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Video"), "~", tr("Video Files (*.avi *.mp4 *.wmv);; All (*.*)"));
    if(!filename.isEmpty() && filename.size() > 0){
        QFileInfo fi(filename);

        this->setWindowTitle("VideoLabel - " + fi.fileName());
        mFileName = fi.baseName();

        clearAll();

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
    text += "<tr> <th align=left>ID:</th> <td>"+QString::number(id)+"</td></tr> </table>";
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

void MainWindow::changeData(int frame, int old_oID, int old_eID, int new_oID, int new_eID)
{
    std::cout<<"Change: "<<old_oID<<" "<<old_eID<<" "<<new_oID<<" "<<new_eID<<": "<<frame<<std::endl;
    QString text = " <title>Änderung der Daten</title>";

    bool ex_Old = false;
    bool ex_New = false;

        text += "<p>Frame "+QString::number(frame)+"</p>";
    if(old_eID >= 0 && ui->listWidget_2->count() > old_eID
            && old_oID >= 0 && ui->listWidget_1->count() > old_oID){
        text += "<p>Von "+ui->listWidget_1->item(old_oID)->text()+" - "+ui->listWidget_2->item(old_eID)->text()+"</p>";
        ex_Old = true;
    }
    if(new_eID >= 0 && ui->listWidget_2->count() > old_eID
            && new_oID >= 0  && ui->listWidget_1->count() > old_oID){
        text += "<p>Von "+ui->listWidget_1->item(new_oID)->text()+" - "+ui->listWidget_2->item(new_eID)->text()+"</p>";
        ex_New = true;
    }

    if(!(ex_New && ex_Old))
        text += "<p>Attribut-Fehler</p>";

    ui->textBrowser->setHtml(text);

    mControler.setEvent(frame,old_oID,new_eID);
    mControler.setObject(frame,old_oID,new_oID);
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
    int nID = ui->listWidget_1->currentRow();
    if(ui->checkBoxEvent->isChecked()){
        QString nObj = ui->listWidget_1->item(nID)->text();
        QString oObj = ui->listWidget_1->item(lastObject)->text();
        if(lastObject != nID &&
                QMessageBox::question(this, "Object Ändern", "Soll das Object \""+nObj+"\" in  \""+oObj+"\" umbenannt werden?",
                                      QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes){
            mControler.setAllObject(nID, lastObject);
            mLoader.deleteObject(nID);
            updateView();
        }
        ui->listWidget_1->item(lastObject)->setSelected(true);
    }else{
        lastObject = nID;
    }
    updateSelection();
}

void MainWindow::on_listWidget_2_clicked(const QModelIndex &index)
{
    displayEvent(index.row());
    if(ui->checkBoxEvent->isChecked()){
        changeData(mPlayer->getPosition(),lastObject,lastEvent,ui->listWidget_1->currentRow(),ui->listWidget_2->currentRow());

        ui->listWidget_2->clearFocus();
    }
    lastEvent = ui->listWidget_2->currentRow();
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    QMainWindow::resizeEvent(ev);
    mControler.setDisplaySize(ui->labelVideo->size().width(),ui->labelVideo->size().height());
    mPlayer->getFrame();
}


void MainWindow::Mouse_Released()
{
    //    ui->textBrowser->setText(QString("Released Label %1 %2 -> %3 %4").arg(ui->labelVideo->x).arg(ui->labelVideo->y).arg(ui->labelVideo->lastX).arg(ui->labelVideo->lastY));
    if(ui->labelVideo->isRecActiv()){
        mControler.addEvent(ui->labelVideo->lastX,
                            ui->labelVideo->lastY,
                            ui->labelVideo->x,
                            ui->labelVideo->y,
                            mPlayer->getPosition(),
                            ui->listWidget_2->currentIndex().row(),
                            ui->listWidget_1->currentIndex().row());
    }else{
        int frame_pos = mControler.getFramePosInVector(mPlayer->getPosition());
        int obj_size = mControler.getObjectSizeInFramePos(frame_pos);

        if(frame_pos >= 0 && obj_size >= 0 &&
                mControler.getActivModel(frame_pos,0).getFrame() == (int)mPlayer->getPosition()){
            int x = ui->labelVideo->x;
            int y = ui->labelVideo->y;
            int w = 0;
            int h = 0;
            mControler.WindoRectToVideoRect(x,y,w,h);
            for(int i = 0 ; i < obj_size; i++){
                int Ox,Oy,Ow,Oh;
                int Oid = mControler.getActivModel(frame_pos,i).getObjectID();
                mControler.getActivModel(frame_pos,i).getRect(Ox,Oy,Ow,Oh);
                if(Ox <= x && Ox+Ow >= x && Oy <= y && Oy+Oh >= y){
                    if(lastObject >= 0 && lastObject < ui->listWidget_1->count()
                            && ui->checkBoxEvent->isChecked() && lastObject != Oid){
                        QString nObj = ui->listWidget_1->item(Oid)->text();
                        QString oObj = ui->listWidget_1->item(lastObject)->text();
                        if(QMessageBox::question(this, "Object Ändern", "Soll das Object \""+nObj+"\" in  \""+oObj+"\" umbenannt werden?",
                                                      QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes){
                            mControler.setAllObject(Oid, lastObject);
                            mLoader.deleteObject(Oid);
                            updateView();
                            ui->checkBoxEvent->setChecked(false);
                            ui->listWidget_1->setCurrentRow(lastObject);
                        }
                    }else{
                        lastObject = Oid;
                        ui->listWidget_1->item(Oid)->setSelected(true);
                        ui->listWidget_1->setCurrentRow(Oid);
                    }
                    break;
                }
            }
        }
    }
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

    QPixmap img = QPixmap::fromImage(frame);

    QPainter paint(&img);

    int frame_pos = mControler.getFramePosInVector(mPlayer->getPosition());
    int obj_size = mControler.getObjectSizeInFramePos(frame_pos);

    bool run = false;
    if(frame_pos >= 0 && obj_size >= 0 &&
            mControler.getActivModel(frame_pos,0).getFrame() == (int)mPlayer->getPosition()){
        for(int i = 0 ; i < obj_size; i++){
            int x,y,w,h;
            mControler.getActivModel(frame_pos,i).getRect(x,y,w,h);
            QRect rec(x,y,w,h);
            if(mControler.getActivModel(frame_pos,i).getObjectID() == ui->listWidget_1->currentIndex().row()){
                paint.setPen(QPen(QColor(Qt::blue), 3, Qt::SolidLine, Qt::SquareCap, Qt::BevelJoin));
                run = true;
            }else{
                paint.setPen(QColor(Qt::green));
            }
            paint.drawRect(rec);
        }
    }
    paint.end();
    QPixmap img2 = img.scaled(ui->labelVideo->size().width(),
                              ui->labelVideo->size().height(),
                              Qt::KeepAspectRatio,Qt::SmoothTransformation);
    ui->labelVideo->setPixmap(img2);

    if(ui->checkBoxEvent->isChecked() && ui->actionPause->isVisible()){
        changeData(mPlayer->getPosition(),lastObject,lastEvent,ui->listWidget_1->currentRow(),ui->listWidget_2->currentRow());
    }

    if(behaviorRun && mPlayer->getPosition() >= behaviorFrame_Last + mPlayer->SecToFrame(60)){
        int oID = ui->listWidget_1->currentRow();
        mBehaviorDialog->clear();
        mBehaviorDialog->setAttribute(oID,"","",behaviorFrame_Last,mPlayer->getPosition());
        behaviorRun = false;
        on_actionPause_triggered();
        mBehaviorDialog->show();
    }

    if(!ui->checkBoxEvent->isChecked()){
        updateSelection();
    }

    if(!run && ui->listWidget_1->selectedItems().size() > 0){
        on_actionPause_triggered();
    }
}

void MainWindow::on_actionSave_triggered()
{
    mXMLLoader->write(mFileName,"./data/");
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
}

void MainWindow::setNoLabelPosition()
{
    int id = ui->listWidget_1->currentIndex().row();
    int pos = mControler.getLastLabel(id);
    ui->horizontalSlider->setValue(pos);
    mPlayer->setPosition(pos);
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

void MainWindow::Eventdelete()
{
    int id = ui->listWidget_2->currentIndex().row();
    Event ev = mLoader.getEvent(id);
    bool dell = true;
    if(mControler.isEventUsed(id)){
        dell = QMessageBox::question(this, "Lösche Event", "Das Event \""+ev.getName()+"\" wird noch verwendet, soll es dennoch gelöscht werden?",
                                     QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes;
    }
    if(dell){
        mControler.deleteEvent(id);
        mLoader.deleteEvent(id);
        updateView();
    }
}

void MainWindow::EventClearFocus()
{
    ui->listWidget_2->clearFocus();
    ui->listWidget_2->clearSelection();
}

void MainWindow::Objectchange(){
    int id = ui->listWidget_1->currentIndex().row();
    QStringList all = mLoader.getObject(id);
    mObjectDialog->setWindowTitle("Change Object");
    mObjectDialog->setAttribute(id, all[0], all[2]);
    mObjectDialog->show();
}

void MainWindow::Objectdelete()
{
    int id = ui->listWidget_1->currentIndex().row();
    QStringList all = mLoader.getObject(id);
    bool dell = true;
    if(mControler.isEventUsed(id)){
        dell = QMessageBox::question(this, "Lösche Qbjekt", "Das Object \""+all[0]+"\" wird noch verwendet, soll es dennoch gelöscht werden?",
                QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes;
    }

    if(dell){
        mControler.deleteObject(id);
        mLoader.deleteObject(id);
        updateView();
    }
}

void MainWindow::ObjectClearFocus()
{
    ui->listWidget_1->clearFocus();
    ui->listWidget_1->clearSelection();
}

void MainWindow::start_behavior()
{
    behaviorFrame_Last = mPlayer->getPosition();
    behaviorRun = true;
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
    if(ui->checkBoxEvent->isChecked()){
        changeData(mPlayer->getPosition(),lastObject,lastEvent,ui->listWidget_1->currentRow(),ui->listWidget_2->currentRow());
    }
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

void MainWindow::on_actionGoTo_No_Label_triggered()
{
    int pos = mControler.getLastLabel(-1);
    ui->horizontalSlider->setValue(pos);
    mPlayer->setPosition(pos);
}

void MainWindow::show_Actionenevent()
{
    int id = ui->listWidget_1->currentIndex().row();
    mActionEventDialog->show(id);
}

void MainWindow::auto_Save()
{
//    on_actionPause_triggered();
    QDateTime t;
    mXMLLoader->write(mFileName+"_auto_"+t.currentDateTime().toString("yy_MM_dd_hh_mm"),"./data/");
}

void MainWindow::on_actionDrawBox_triggered(bool checked)
{
    ui->labelVideo->setAddRect(checked);
}
