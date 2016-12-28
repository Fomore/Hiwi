#include "eventdialog.h"
#include "ui_eventdialog.h"

#include <iostream>

EventDialog::EventDialog(QWidget *parent, Loader *load) :
    QDialog(parent),
    ui(new Ui::EventDialog),
    mLoader(load)
{
    ui->setupUi(this);
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
    if(name.size() > 0){//Event wird angelegt
        if(mID < mLoader->getEventSize()){
            mLoader->ChamgeEvent(mID, name, desc,
                                 ui->checkBoxEye->isChecked(),
                                 ui->checkBoxActiv->isChecked(),
                                 ui->checkBoxOther->isChecked(),
                                 ui->checkBoxRestless->isChecked(),
                                 ui->checkBoxComm->isChecked());
        }else{
            mLoader->addNewEvent(name,desc,
                                 ui->checkBoxEye->isChecked(),
                                 ui->checkBoxActiv->isChecked(),
                                 ui->checkBoxOther->isChecked(),
                                 ui->checkBoxRestless->isChecked(),
                                 ui->checkBoxComm->isChecked());
        }
        emit accept();
    }
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
