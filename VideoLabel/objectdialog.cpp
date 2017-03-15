#include "objectdialog.h"
#include "ui_objectdialog.h"

#include <iostream>

ObjectDialog::ObjectDialog(QWidget *parent, Controler *load) :
    QDialog(parent),
    ui(new Ui::ObjectDialog),
    mControler(load)
{
    ui->setupUi(this);
}

ObjectDialog::~ObjectDialog()
{
    delete ui;
}

void ObjectDialog::setUseObject()
{
}

void ObjectDialog::setUseEvent()
{
}

void ObjectDialog::clear()
{
    ui->lineEditName->clear();
    ui->textEdit->clear();
}

void ObjectDialog::on_buttonBox_accepted()
{
    QString name = ui->lineEditName->text().simplified();
    QString desc = ui->textEdit->toPlainText().simplified();
    if(name.size() > 0){//Objekt wird angelegt
        if(mID < mControler->getObjectSize()){
            mControler->ChangeObject(mID, name, desc);
        }else{
            mControler->addNewObject(name,desc);
        }
        emit accept();
    }
}

void ObjectDialog::setAttribute(int id, QString name, QString desc)
{
    setID(id);
    ui->lineEditName->setText(name);
    ui->textEdit->setText(desc);

    std::vector<VerhaltenTime> ver = mControler->getBehaviors(id);
    ui->tableWidget->setRowCount(ver.size());
    for(size_t i = 0; i < ver.size(); i++){
        int s,e;
        ver[i].getFrame(s,e);
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(s)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(QString::number(e)));
        if(ver[i].OnTask()){
            ui->tableWidget->setItem(i,2,new QTableWidgetItem("Yes"));
        }else{
            ui->tableWidget->setItem(i,2,new QTableWidgetItem("No"));
        }
    }
}

void ObjectDialog::setID(int id)
{
    mID = id;
    ui->label_ID->setText("ID: "+QString::number(id));
}
