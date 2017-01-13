#include "eventdialog.h"
#include "ui_eventdialog.h"

#include <iostream>

EventDialog::EventDialog(QWidget *parent, Loader *load) :
    QDialog(parent),
    ui(new Ui::EventDialog),
    mLoader(load)
{
    ui->setupUi(this);
    mVerhalten = false;
}

EventDialog::~EventDialog()
{
    delete ui;
}

void EventDialog::clear()
{
    ui->lineEditName->clear();
    ui->textEdit->clear();
}

void EventDialog::on_buttonBox_accepted()
{
    QString name = ui->lineEditName->text().simplified();
    QString desc = ui->textEdit->toPlainText().simplified();
    bool eye, activ, other, less, comm;
    getAttribute(eye, activ, other, less, comm);
    if(!mVerhalten){
    if(name.size() > 0){//Event wird angelegt
        if(mID < mLoader->getEventSize()){
            mLoader->ChamgeEvent(mID, name, desc, eye, activ, other, less, comm);
        }else{
            mLoader->addNewEvent(name,desc, eye, activ, other, less, comm);
        }
        emit accept();
    }
    }else{
        mLoader->addNewVerhalten(mID,name,desc, eye, activ, other, less, comm, -1,-1);
    }
}

void EventDialog::getAttribute(bool &eye, bool &activ, bool &other, bool &less, bool &comm)
{
    eye = ui->checkBoxEye->isChecked();
    activ = ui->checkBoxActiv->isChecked();
    other = ui->checkBoxOther->isChecked();
    less = ui->checkBoxRestless->isChecked();
    comm = ui->checkBoxComm->isChecked();
}

void EventDialog::setAttribute(int id, QString name, QString desc)
{
    setID(id);
    ui->lineEditName->setText(name);
    ui->textEdit->setText(desc);
}

void EventDialog::setID(int id)
{
    mID = id;
    ui->label_ID->setText("ID: "+QString::number(id));
}
