#include "comparedialog.h"
#include "ui_comparedialog.h"

CompareDialog::CompareDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompareDialog)
{
    ui->setupUi(this);
}

CompareDialog::~CompareDialog()
{
    delete ui;
}
