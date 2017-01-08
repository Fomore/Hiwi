#ifndef ACTIONEVENTDIALOG_H
#define ACTIONEVENTDIALOG_H

#include <QDialog>
#include <QMenu>

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

private slots:
    void on_pushButton_Probl_clicked();

    void on_pushButton_Interpolate_clicked();

    void on_buttonBox_accepted();

    void deleteActionEvent();

    void contextMenuRequested(const QPoint& point);

private:
    Ui::ActionEventDialog *ui;
    Loader *mLoader;
    Controler *mControl;

    std::vector<int> mDeleteList;
    int mObjectID;

    QMenu* mMenu;
    std::vector<QAction*> mMenueAction;
};

#endif // ACTIONEVENTDIALOG_H
