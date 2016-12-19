#include "inputevobdialog.h"
#include "ui_inputevobdialog.h"

#include <iostream>

InputEvObDialog::InputEvObDialog(QWidget *parent, Loader *load) :
    QDialog(parent),
    ui(new Ui::InputEvObDialog),
    mLoader(load)
{
    ui->setupUi(this);
    mUsedFore = 0;
}

InputEvObDialog::~InputEvObDialog()
{
    delete ui;
}

void InputEvObDialog::setUseObject()
{
    mUsedFore = 0;
}

void InputEvObDialog::setUseEvent()
{
    mUsedFore = 1;
}

void InputEvObDialog::clear()
{
    ui->lineEditName->clear();
    ui->textEdit->clear();
}

void InputEvObDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    QString name = ui->lineEditName->text().simplified();
    QString desc = ui->textEdit->toPlainText().simplified();
    if(name.size() > 0){
        if(mUsedFore == 0){//Objekt wird angelegt
            if(mID < mLoader->getObjectSize()){
                mLoader->ChangeObject(mID, name, desc);
            }else{
            mLoader->addNewObject(name,desc);
            }
        }else if(mUsedFore == 1){//Event wird angelegt
            if(mID < mLoader->getEventSize()){
                mLoader->ChamgeEvent(mID, name, desc);
            }else{
                mLoader->addNewEvent(name,desc);
            }
        }
    }
}

void InputEvObDialog::setEvOb(int id, QString name, QString desc)
{
    setID(id);
    ui->lineEditName->setText(name);
    ui->textEdit->setText(desc);
}

void InputEvObDialog::setID(int id)
{
    mID = id;
    ui->label_ID->setText("ID: "+QString::number(id));
}
