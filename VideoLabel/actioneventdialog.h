#ifndef ACTIONEVENTDIALOG_H
#define ACTIONEVENTDIALOG_H

#include <QDialog>
#include "control/controler.h"
#include "control/loader.h"

namespace Ui {
class ActionEventDialog;
}

class ActionEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ActionEventDialog(QWidget *parent = 0, Loader *loader=0, Controler *control=0);
    ~ActionEventDialog();

public slots:
    void show(int O_id);

private:
    Ui::ActionEventDialog *ui;
    Loader *mLoader;
    Controler *mControl;
};

#endif // ACTIONEVENTDIALOG_H
