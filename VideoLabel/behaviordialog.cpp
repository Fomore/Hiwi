#include "behaviordialog.h"
#include "ui_behaviordialog.h"

#include <iostream>

BehaviorDialog::BehaviorDialog(QWidget *parent, Loader *load) :
    QDialog(parent),
    ui(new Ui::BehaviorDialog),
    mLoader(load)
{
    ui->setupUi(this);
}

BehaviorDialog::~BehaviorDialog()
{
    delete ui;
}

void BehaviorDialog::clear()
{
    ui->lineEditName->clear();
    ui->textEdit->clear();
    ui->checkBoxActiv->setChecked(false);
    ui->checkBoxComm->setChecked(false);
    ui->checkBoxEye->setChecked(false);
    ui->checkBoxOther->setChecked(false);
    ui->checkBoxRestless->setChecked(false);
}

void BehaviorDialog::on_buttonBox_accepted()
{
    QString name = ui->lineEditName->text().simplified();
    QString desc = ui->textEdit->toPlainText().simplified();
    bool eye, activ, other, less, comm;
    getAttribute(eye, activ, other, less, comm);
    mLoader->addNewVerhalten(O_ID, name, desc, eye, activ, other, less, comm, frame_S, frame_E);
}

void BehaviorDialog::getAttribute(bool &eye, bool &activ, bool &other, bool &less, bool &comm)
{
    eye = ui->checkBoxEye->isChecked();
    activ = ui->checkBoxActiv->isChecked();
    other = ui->checkBoxOther->isChecked();
    less = ui->checkBoxRestless->isChecked();
    comm = ui->checkBoxComm->isChecked();
}

void BehaviorDialog::setAttribute(int id, QString name, QString desc, int from, int to)
{
    ui->lineEditName->setText(name);
    ui->textEdit->setText(desc);
    O_ID = id;
    frame_S = from;
    frame_E = to;
    ui->label_ID->setText("ID: "+QString::number(id));
    ui->label_Frame_Start->setText("From: "+QString::number(from));
    ui->label_Fram_Ende->setText("To: "+QString::number(to));
}
