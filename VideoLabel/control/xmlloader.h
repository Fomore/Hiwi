#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <QString>
#include <QXmlStreamReader>
#include <QtXml>

#include "control/controler.h"

class XMLLoader
{
public:
    XMLLoader(Controler *control=0);

    void read(const QString filename);
    void read(const QString filename, const QString path);
    void write(const QString filename, const QString path);
    void writeSmall(const QString filename, const QString path, int Gaze);

    bool useLabel(QString Label);
    bool mIgnorePerson = true;
    QStringList mIgnoreList;
private:
    void processName();
    void processBehavior(int oID);
    void processDataset();
    void processImages();
    void processImage(int frame);
    int processBox(int &E_id, bool &isOri, double orient[3], bool &isPos, double pos[3], bool &isPro, double proj[4], bool &isLand, double land[5][2],
                    int &gaze, bool &Use);
    void processLandmarks(double mark[5][2]);
    int processEvent();
    int processPerson(bool &use);

    QString readNextText();
    QString errorString();

    int filnameToFrame(QString file);
    void boxAttributToValue(QXmlStreamAttributes att, int &height, int &left, int &top, int &width, bool &manual, bool &ignore);

    QXmlStreamReader xml;
    Controler *mControler;

    bool isWriting;
};

#endif // XMLLOADER_H
