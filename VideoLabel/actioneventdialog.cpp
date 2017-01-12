#include "actioneventdialog.h"
#include "ui_actioneventdialog.h"

#include <QMessageBox>
#include <iostream>

ActionEventDialog::ActionEventDialog(QWidget *parent, Loader *loader, Controler *control) :
    QDialog(parent),
    ui(new Ui::ActionEventDialog), mLoader(loader), mControl(control)
{
    ui->setupUi(this);

    mMenu = new QMenu(ui->tableWidget);
    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableWidget, SIGNAL(customContextMenuRequested(const QPoint)),this, SLOT(contextMenuRequested(const QPoint)));
    mMenueAction.push_back(mMenu->addAction("GoTo Frame"));
    mMenueAction.push_back(mMenu->addAction("Zeile Löschen"));
    connect(mMenueAction[0],SIGNAL(triggered()),this,SLOT(gotoFrame()));
    connect(mMenueAction[1],SIGNAL(triggered()),this,SLOT(deleteActionEvent()));
}

ActionEventDialog::~ActionEventDialog()
{
    delete ui;
}

void ActionEventDialog::show(int O_id)
{
    mDeleteList.clear();
    QDialog::show();
    mObjectID = O_id;
    std::vector<ActivModel> list = mControl->getAllActivodel(O_id);
    ui->tableWidget->setRowCount(list.size());
    for(size_t i = 0; i < list.size(); i++){
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(list[i].getFrame())));
        if(mLoader->existEventID(list[i].getEventID())){
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(mLoader->getEventName(list[i].getEventID())));
        }else{
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(list[i].getEventID())));
        }
        int x,y,w,h;
        list[i].getRect(x,y,w,h);
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
    for(size_t i = 0; i < mDeleteList.size(); i++){
        mControl->deleteActionEvent(mObjectID,mDeleteList[i]);
    }
    for(int i = 0; i < ui->tableWidget->rowCount(); i++){
        int E_id = mLoader->getEventID(ui->tableWidget->item(i,1)->text());
        mControl->addEventInFrame(ui->tableWidget->item(i,2)->text().toInt(),
                                  ui->tableWidget->item(i,3)->text().toInt(),
                                  ui->tableWidget->item(i,4)->text().toInt(),
                                  ui->tableWidget->item(i,5)->text().toInt(),
                                  ui->tableWidget->item(i,0)->text().toInt(),
                                  E_id,
                                  mObjectID,
                                  false);
    }
}

void ActionEventDialog::deleteActionEvent()
{
    QList<QTableWidgetItem *> list = ui->tableWidget->selectedItems();
    int pos = list[0]->row();
    int frame = ui->tableWidget->item(pos,0)->text().toInt();
    mDeleteList.push_back(frame);
    ui->tableWidget->removeRow(pos);
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
    ui->tableWidget->clearSelection();
    if(t.row() >= 0){
        mMenu->popup(ui->tableWidget->mapToGlobal(point));
        ui->tableWidget->selectRow(t.row());
    }
}
