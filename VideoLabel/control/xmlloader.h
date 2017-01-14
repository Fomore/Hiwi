#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <QString>
#include <QXmlStreamReader>
#include <QtXml>

#include "control/controler.h"
#include "control/loader.h"

class XMLLoader
{
public:
    XMLLoader(Loader *loader=0, Controler *control=0);

    void read(const QString filename);
    void read(const QString filename, const QString path);
    void write(const QString filename, const QString path);

private:
    void processName();
    void processBehavior(int oID);
    void processDataset();
    void processImages();
    void processImage(int frame);
    int processBox(int &E_id, bool &isOri, double orient[3], bool &isPos, double pos[3], bool &isPro, double proj[4], bool &isLand, double land[5][2]);
    void processLandmarks(double mark[5][2]);
    int processEvent();
    int processPerson();

    QString readNextText();
    QString errorString();

    int filnameToFrame(QString file);
    void boxAttributToValue(QXmlStreamAttributes att, int &height, int &left, int &top, int &width, bool &manual, bool &ignore);

    QXmlStreamReader xml;
    Loader *mLoader;
    Controler *mControl;
};

#endif // XMLLOADER_H
