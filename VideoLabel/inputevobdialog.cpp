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
    QString name = ui->lineEditName->text();
    QString desc = ui->textEdit->toPlainText();
    if(name.size() > 0){
        if(mUsedFore == 0){//Objekt wird angelegt
            mLoader->addNewObject(name,desc);
        }else if(mUsedFore == 1){//Event wird angelegt
            mLoader->addNewEvent(name,desc);
        }
    }
}
