#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>
#include <QAbstractButton>
#include "control/loader.h"

namespace Ui {
class EventDialog;
}

class EventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EventDialog(QWidget *parent = 0, Loader *load = 0);
    ~EventDialog();

    void clear();

    void setAttribute(int id, QString name, QString desc);
    void setID(int id);

private slots:
    void on_buttonBox_accepted();

private:
    Ui::EventDialog *ui;

    int mID;
    bool mVerhalten;

    Loader *mLoader;

    void getAttribute(bool &eye, bool &activ, bool &other, bool &less, bool &comm);
};

#endif // EVENTDIALOG_H
