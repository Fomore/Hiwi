#ifndef ACTIONEVENTDIALOG_H
#define ACTIONEVENTDIALOG_H

#include <QDialog>
#include <QMenu>

#include "control/controler.h"

namespace Ui {
class ActionEventDialog;
}

class ActionEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ActionEventDialog(QWidget *parent = 0, Controler *control=0);
    ~ActionEventDialog();

signals:
    void gotoVideoFrame(const int frame);

public slots:
    void show(int O_id);

private slots:
    void on_pushButton_Probl_clicked();

    void on_pushButton_Interpolate_clicked();

    void on_buttonBox_accepted();

    void deleteActionEvent();

    void gotoFrame();

    void contextMenuRequested(const QPoint& point);

    void setObject();

private:
    void addDeleteList(int frame, int oldOpos, int newOpos);

    void changeObject(const QString name);

    Ui::ActionEventDialog *ui;
    Controler *mControler;

    int mObjectID;
    std::vector<cv::Point3i> mDeleteList;
    std::vector<cv::Point3i> mActivModelList;

    QMenu* mMenu;
    std::vector<QAction*> mMenueAction;
};

#endif // ACTIONEVENTDIALOG_H
