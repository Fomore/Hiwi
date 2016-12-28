#ifndef OBJECTDIALOG_H
#define OBJECTDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "control/loader.h"

namespace Ui {
class ObjectDialog;
}

class ObjectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectDialog(QWidget *parent = 0, Loader *load = 0);
    ~ObjectDialog();
    void setUseObject();
    void setUseEvent();

    void clear();

    void setAttribute(int id, QString name, QString desc);
    void setID(int id);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ObjectDialog *ui;
    int mID;

    Loader *mLoader;
};

#endif // ObjectDialog_H
