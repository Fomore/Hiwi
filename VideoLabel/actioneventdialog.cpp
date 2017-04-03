#include "actioneventdialog.h"
#include "ui_actioneventdialog.h"

#include <QMessageBox>
#include <QInputDialog>
#include <iostream>

ActionEventDialog::ActionEventDialog(QWidget *parent, Controler *control) :
    QDialog(parent),
    ui(new Ui::ActionEventDialog), mControler(control)
{
    ui->setupUi(this);

    mMenu = new QMenu(ui->tableWidget);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint)),this, SLOT(contextMenuRequested(const QPoint)));
    mMenueAction.push_back(mMenu->addAction("Set Object"));
    mMenueAction.push_back(mMenu->addAction("GoTo Frame"));
    mMenueAction.push_back(mMenu->addAction("Zeile Löschen"));
    connect(mMenueAction[0],SIGNAL(triggered()),this,SLOT(setObject()));
    connect(mMenueAction[1],SIGNAL(triggered()),this,SLOT(gotoFrame()));
    connect(mMenueAction[2],SIGNAL(triggered()),this,SLOT(deleteActionEvent()));
}

ActionEventDialog::~ActionEventDialog()
{
    delete ui;
}

void ActionEventDialog::show(int O_id)
{
    mDeleteList.clear();
    mActivModelList.clear();

    QDialog::show();
    mObjectID = O_id;
    mActivModelList = mControler->getAllActivModel(O_id);
    ui->tableWidget->setRowCount(mActivModelList.size());
    for(size_t i = 0; i < mActivModelList.size(); i++){
        ActivModel mod = mControler->getActivModel(mActivModelList[i].x,mActivModelList[i].y);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(mod.getFrame())));
        if(mControler->existEventPos(mod.getEventID())){
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(mControler->getEventName(mod.getEventID())));
        }else{
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(mod.getEventID())));
        }
        int x,y,w,h;
        mod.getRect(x,y,w,h);
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(x)));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(y)));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(w)));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(h)));
    }
}

void ActionEventDialog::on_pushButton_Probl_clicked()
{
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        if(ui->tableWidget->item(i,1)->text() == "-1"
                || ui->tableWidget->item(i,0)->text().toInt() < 0
                || ui->tableWidget->item(i,2)->text().toInt() < 0
                || ui->tableWidget->item(i,3)->text().toInt() < 0
                || ui->tableWidget->item(i,4)->text().toInt() <= 0
                || ui->tableWidget->item(i,5)->text().toInt() <= 0){
            ui->tableWidget->selectRow(i);
            break;
        }
    }
}

void ActionEventDialog::on_pushButton_Interpolate_clicked()
{
    QList<QTableWidgetItem *> list = ui->tableWidget->selectedItems();
    if(list.size() == 2 && (list[0]->row()+1 == list[1]->row() ||
                            list[0]->row()-1 == list[1]->row())){
        int a = std::min(list[1]->row(),list[0]->row());
        int b = std::max(list[1]->row(),list[0]->row());

        double s = ui->tableWidget->item(b,0)->text().toInt() - ui->tableWidget->item(a,0)->text().toInt();
        if(s <= 0)
            return;

        QString label = ui->tableWidget->item(a,1)->text();

        int f = ui->tableWidget->item(a,0)->text().toInt();

        int x = ui->tableWidget->item(a,2)->text().toInt();
        int y = ui->tableWidget->item(a,3)->text().toInt();
        int w = ui->tableWidget->item(a,4)->text().toInt();
        int h = ui->tableWidget->item(a,5)->text().toInt();

        double sx = ui->tableWidget->item(b,2)->text().toInt() - ui->tableWidget->item(a,2)->text().toInt();
        sx /= s;
        double sy = ui->tableWidget->item(b,3)->text().toInt() - ui->tableWidget->item(a,3)->text().toInt();
        sy /= s;
        double sw = ui->tableWidget->item(b,4)->text().toInt() - ui->tableWidget->item(a,4)->text().toInt();
        sw /= s;
        double sh = ui->tableWidget->item(b,5)->text().toInt() - ui->tableWidget->item(a,5)->text().toInt();
        sh /= s;
        for(double i = 1; i < s; i++){
            mActivModelList.insert(mActivModelList.begin()+a+i,cv::Point3i(-1,-1,-1));
            ui->tableWidget->insertRow(a+i);
            ui->tableWidget->setItem(a+i,0,new QTableWidgetItem(QString::number(f+i)));
            ui->tableWidget->setItem(a+i,1,new QTableWidgetItem(label));
            ui->tableWidget->setItem(a+i,2,new QTableWidgetItem(QString::number((int)(x+sx*i+0.5))));
            ui->tableWidget->setItem(a+i,3,new QTableWidgetItem(QString::number((int)(y+sy*i+0.5))));
            ui->tableWidget->setItem(a+i,4,new QTableWidgetItem(QString::number((int)(w+sw*i+0.5))));
            ui->tableWidget->setItem(a+i,5,new QTableWidgetItem(QString::number((int)(h+sh*i+0.5))));
        }
    }else{
        QMessageBox::information(this, "Interpolation","Bei der Interpolation müssen zwei untereinander liegende Felder ausgewählt werden");
    }
}

void ActionEventDialog::on_buttonBox_accepted()
{
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        int frame = ui->tableWidget->item(i,0)->text().toInt();
        int E_id = mControler->getEventID(ui->tableWidget->item(i,1)->text());
        int x = ui->tableWidget->item(i,2)->text().toInt();
        int y = ui->tableWidget->item(i,3)->text().toInt();
        int w = ui->tableWidget->item(i,4)->text().toInt();
        int h = ui->tableWidget->item(i,5)->text().toInt();

        if(mActivModelList[i].x >=0 && mActivModelList[i].y >= 0 && mActivModelList[i].z >= 0){//Object exisiteirt bereits
//            int framePos = mControler->getFramePosInVector(mActivModelList[i].z);
            int Fp = mActivModelList[i].x;
            int Op = mActivModelList[i].y;
            std::cout<<i<<": "<<mActivModelList[i]<<std::endl;
            mControler->changeActionEventValue(Fp,Op,E_id,x,y,w,h);
            if(mActivModelList[i].z != frame){
                mControler->copyActionEvent(Fp,Op,frame);
                mDeleteList.push_back(cv::Point3i(mActivModelList[i].z,Op,-1));
            }
        }else{
            mControler->addObjectInFrame(x,y,w,h,frame,E_id,mObjectID,false);
        }
    }
    for(size_t i = 0; i < mDeleteList.size(); i++){
        if(mDeleteList[i].z >= 0){
            mControler->setObject(mDeleteList[i].x,mDeleteList[i].y,mDeleteList[i].z);
        }else{
            mControler->deleteActionEvent(mDeleteList[i].y,mDeleteList[i].x);
        }
    }
}

void ActionEventDialog::deleteActionEvent()
{
    QModelIndexList list = ui->tableWidget->selectionModel()->selectedRows();
    std::vector<int> pos;
    pos.push_back(list[0].row());
    for(int i = 1; i < list.size(); i++){
        int p = 0;
        while (p < (int)pos.size() && pos[p] > list[i].row()) {
            p++;
        }
        pos.insert(pos.begin()+p,list[i].row());
    }
    for(size_t i =0 ; i < pos.size(); i++){
        if(mActivModelList[pos[i]].x >= 0 && mActivModelList[pos[i]].y >= 0){
            addDeleteList(mActivModelList[pos[i]].x,mActivModelList[pos[i]].y,-1);
            mActivModelList.erase(mActivModelList.begin() + pos[i]);
        }else{
            mActivModelList.erase(mActivModelList.begin() + pos[i]);
        }
        ui->tableWidget->removeRow(pos[i]);
    }
}

void ActionEventDialog::gotoFrame()
{
    QList<QTableWidgetItem *> list = ui->tableWidget->selectedItems();
    int pos = list[0]->row();
    int frame = ui->tableWidget->item(pos,0)->text().toInt();
    emit gotoVideoFrame(frame);
}

void ActionEventDialog::contextMenuRequested(const QPoint &point)
{
    QModelIndex t = ui->tableWidget->indexAt(point);
    if(t.row() >= 0){
        QList<QTableWidgetItem *> list = ui->tableWidget->selectedItems();

        QItemSelectionModel *selectionModel = ui->tableWidget->selectionModel();
        QItemSelection itemSelection;
        for(int i = 0; i < list.size(); i++){
            ui->tableWidget->selectRow(list[i]->row());
            itemSelection.merge(selectionModel->selection(), QItemSelectionModel::Select);
        }
        selectionModel->clearSelection();
        selectionModel->select(itemSelection,QItemSelectionModel::Select);
        mMenu->popup(ui->tableWidget->mapToGlobal(point));
    }
}

void ActionEventDialog::setObject()
{
    QStringList items = mControler->getObjectAllName();

    bool ok;
    QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),
                                         tr("Season:"), items, 0, false, &ok);
    if (ok && !item.isEmpty())
        changeObject(item);
}

void ActionEventDialog::changeObject(const QString name)
{
    int id = mControler->getObjectID(name);
    QModelIndexList list = ui->tableWidget->selectionModel()->selectedRows();
    for(int i = 0; i < list.size(); i++){
        int pos = list[i].row();
        if(mActivModelList[pos].x >= 0 && mActivModelList[pos].y >= 0 && mActivModelList[pos].z >= 0){
            addDeleteList(mActivModelList[pos].z,mActivModelList[pos].y,id);
            mActivModelList.erase(mActivModelList.begin() + pos);
        }else{
            mActivModelList.erase(mActivModelList.begin() + pos);
            std::cout<<"Change-Fehler"<<std::endl;
        }
        ui->tableWidget->removeRow(pos);
    }
}

void ActionEventDialog::addDeleteList(int frame, int oldOpos, int newOpos)
{
    int p = 0;
    while (p < (int)mDeleteList.size() && mDeleteList[p].x > frame) {
        p++;
    }
    while(p < (int)mDeleteList.size() && mDeleteList[p].x == frame && mDeleteList[p].y > oldOpos){
        p++;
    }
    mDeleteList.insert(mDeleteList.begin()+p,cv::Point3i(frame, oldOpos, newOpos));
}
