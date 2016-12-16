#ifndef INPUTEVOBDIALOG_H
#define INPUTEVOBDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "control/loader.h"

namespace Ui {
class InputEvObDialog;
}

class InputEvObDialog : public QDialog
{
    Q_OBJECT

public:
    explicit InputEvObDialog(QWidget *parent = 0, Loader *load = 0);
    ~InputEvObDialog();
    void setUseObject();
    void setUseEvent();

    void clear();

    void setEvOb(int id, QString name, QString desc);
    void setID(int id);

private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::InputEvObDialog *ui;
    int mUsedFore;
    int mID;

    Loader *mLoader;
};

#endif // INPUTEVOBDIALOG_H
