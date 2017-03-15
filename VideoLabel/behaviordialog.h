#ifndef BEHAVIORDIALOG_H
#define BEHAVIORDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "control/controler.h"

namespace Ui {
class BehaviorDialog;
}

class BehaviorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BehaviorDialog(QWidget *parent = 0, Controler *load = 0);
    ~BehaviorDialog();

    void clear();

    void setAttribute(int id, QString name, QString desc, int from, int to);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::BehaviorDialog *ui;

    int O_ID, frame_S, frame_E;

    Controler *mControler;

    void getAttribute(bool &eye, bool &activ, bool &other, bool &less, bool &comm);
};

#endif // BEHAVIORDIALOG_H
