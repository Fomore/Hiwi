#ifndef XMLLOADER_H
#define XMLLOADER_H

#include <QString>
#include <QXmlStreamReader>
#include <QtXml>
#include "model/event.h"

class XMLLoader
{
public:
    XMLLoader();

    void read(const QString filename);
    void read(const QString filename, const QString path);
    void write(const QString filename, const QString path);

    void sort();

    bool grather(size_t a, size_t b);

private:
    void TitelToValue(QString titel, QString &Side, int &PointNumber, int &Col, int &Row);

    std::vector<Event> mEvents;
    std::vector<size_t> mPos;

    void processName();
    void processDataset();
    void processEvent();

    QString readNextText();
    QString errorString();

    QXmlStreamReader xml;

};

#endif // XMLLOADER_H
