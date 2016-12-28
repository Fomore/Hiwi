#include "objectdialog.h"
#include "ui_objectdialog.h"

#include <iostream>

ObjectDialog::ObjectDialog(QWidget *parent, Loader *load) :
    QDialog(parent),
    ui(new Ui::ObjectDialog),
    mLoader(load)
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
        if(mID < mLoader->getObjectSize()){
            mLoader->ChangeObject(mID, name, desc);
        }else{
            mLoader->addNewObject(name,desc);
        }
        emit accept();
    }
}

void ObjectDialog::setAttribute(int id, QString name, QString desc)
{
    setID(id);
    ui->lineEditName->setText(name);
    ui->textEdit->setText(desc);
}

void ObjectDialog::setID(int id)
{
    mID = id;
    ui->label_ID->setText("ID: "+QString::number(id));
}
