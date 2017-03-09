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
    void write(const QString filename, const QString path);

    void sort();
    std::vector<size_t> mPos;
    std::vector<Event> mEvents;
    bool grather(size_t a, size_t b);

private:
    void TitelToValue(QString titel, QString &Side, int &PointNumber, int &Col, int &Row);

    void sort(int left, int right);

    void processName();
    void processDataset();
    void processEvent();

    QString readNextText();
    QString errorString();

    QXmlStreamReader xml;

};

#endif // XMLLOADER_H
