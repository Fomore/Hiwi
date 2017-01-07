#include "actioneventdialog.h"
#include "ui_actioneventdialog.h"

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
    std::cout<<"Object: "<<O_id<<std::endl;
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
    std::cout<<"Es werden "<<list.size()<<" Eingräge angezeigt"<<std::endl;
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
