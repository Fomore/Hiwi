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
    void write(const QString filename, const QString path);

private:
    void processName();
    void processDataset();
    void processImages();
    void processImage(int frame);
    int processBox(bool &isOri, double orient[3], bool &isPos, double pos[3], bool &isPro, double proj[4], bool &isLand, double land[5][2]);
    void processLandmarks(double mark[5][2]);
    QString readNextText();
    QString errorString();

    int filnameToFrame(QString file);
    void boxAttributToValue(QXmlStreamAttributes att, int &height, int &left, int &top, int &width, bool &manual, bool &ignore);

    QXmlStreamReader xml;
    Loader *mLoader;
    Controler *mControl;
};

#endif // XMLLOADER_H
