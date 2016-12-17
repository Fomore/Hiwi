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

private:
    void processDataset();
    void processImages();
    void processImage(int frame);
    int processBox();
    QString readNextText();
    QString errorString();

    int filnameToFrame(QString file);
    void boxAttributToValue(QXmlStreamAttributes att, int &height, int &left, int &top, int &width, bool &manual, bool &ignore);

    QXmlStreamReader xml;
    Loader *mLoader;
    Controler *mControl;
};

#endif // XMLLOADER_H
