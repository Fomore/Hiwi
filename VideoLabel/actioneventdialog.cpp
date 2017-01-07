#include "actioneventdialog.h"
#include "ui_actioneventdialog.h"

#include <QMessageBox>
#include <iostream>

ActionEventDialog::ActionEventDialog(QWidget *parent, Loader *loader, Controler *control) :
    QDialog(parent),
    ui(new Ui::ActionEventDialog), mLoader(loader), mControl(control)
{
    ui->setupUi(this);
}

ActionEventDialog::~ActionEventDialog()
{
    delete ui;
}

void ActionEventDialog::show(int O_id)
{
    QDialog::show();
    mObjectID = O_id;
    std::vector<ActivModel> list = mControl->getAllActivodel(O_id);
    ui->tableWidget->setRowCount(list.size());
    for(int i = 0; i < list.size(); i++){
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(list[i].getFrame())));
        if(mLoader->existEventID(list[i].mEventID)){
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(mLoader->getEventName(list[i].mEventID)));
        }else{
            ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(list[i].mEventID)));
        }
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString::number(list[i].mX)));
        ui->tableWidget->setItem(i,3,new QTableWidgetItem(QString::number(list[i].mY)));
        ui->tableWidget->setItem(i,4,new QTableWidgetItem(QString::number(list[i].mW)));
        ui->tableWidget->setItem(i,5,new QTableWidgetItem(QString::number(list[i].mH)));
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
            ui->tableWidget->setItem(a+i,0,new QTableWidgetItem(QString::number(a+i)));
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
