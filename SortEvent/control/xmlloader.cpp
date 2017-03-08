#include "xmlloader.h"
#include <QFile>
#include <QDebug>

#include <iostream>

XMLLoader::XMLLoader()
{
    mPos.clear();
    mEvents.clear();
}

void XMLLoader::read(const QString filename, QString path){
    read(path+filename+"_Label.xml");
}

void XMLLoader::read(const QString filename)
{
    QFile xmlFile(filename);
    if(xmlFile.open(QIODevice::ReadOnly)){
        xml.setDevice(&xmlFile);

        if (xml.readNextStartElement() && xml.name() == "dataset"){
            processDataset();
        }else{
            std::cout<<"Fehler bei Name dataset: "<<xml.name().toString().toStdString()<<std::endl;
        }

        if (xml.tokenType() == QXmlStreamReader::Invalid)
            xml.readNext();

        if (xml.hasError()) {
            xml.raiseError();
            qDebug() << errorString();
        }
    }else{
        qDebug()<< "Fehler in XML beim öffen von XML: "<<filename;
    }
}

void XMLLoader::write(const QString filename, const QString path)
{
    QXmlStreamWriter xmlWriter;
    if(!QDir(path).exists()){
        QDir().mkdir(path);
    }
    QFile file(path+filename+"Short.xml");
    file.open(QFile::WriteOnly);

    xmlWriter.setDevice(&file);
    xmlWriter.setAutoFormatting(true);
    std::cout<<"Write "<<filename.toStdString()<<std::endl;

    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("dataset");

    xmlWriter.writeStartElement("name");
    xmlWriter.writeCharacters ("imglab dataset");
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("comment");
    xmlWriter.writeCharacters ("Created by VideoLabel");
    xmlWriter.writeEndElement();

    std::cout<<"Schreibe Events"<<std::endl;

    for(size_t i = 0; i < mPos.size(); i++){
        if(mPos[i] < mEvents.size()){
            Event ev = mEvents[mPos[i]];
            xmlWriter.writeStartElement("event");
            xmlWriter.writeAttribute("name",ev.getName());
            xmlWriter.writeAttribute("EyeContact",QString::number(ev.getEyeVontact()));
            xmlWriter.writeAttribute("ActiveParticipation",QString::number(ev.getActiveParticipation()));
            xmlWriter.writeAttribute("OtherActivities",QString::number(ev.getOtherActivities()));
            xmlWriter.writeAttribute("Restlessness",QString::number(ev.getRestlessness()));
            xmlWriter.writeAttribute("Communication",QString::number(ev.getCommunication()));

            xmlWriter.writeStartElement("description");
            xmlWriter.writeCharacters (ev.getDescription());
            xmlWriter.writeEndElement();
            xmlWriter.writeEndElement();
        }
    }


    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    std::cout<<"Write "<<filename.toStdString()<<" Ende"<<std::endl;

}

void XMLLoader::TitelToValue(QString titel, QString &Side, int &PointNumber, int &Col, int &Row)
{
    QStringList part1 =  titel.split(" ");

    if(part1.size() == 4){
        Row = part1[1].toInt();
        Col = part1[3].toInt();
    }else{
        Row = Col = 0;
    }

    QStringList part2 =  part1[0].split("_");
    if(part2.size() == 2){
        if(part2[1][0] == '0'){
            Side = "0";
            PointNumber = 0;
        }else if(part2[1][0] == 'R'){
            Side = "R";
            PointNumber = part2[1].data()[1].digitValue();
        }else if(part2[1][0] == 'L'){
            Side = "L";
            PointNumber = part2[1].data()[1].digitValue();
        }else{
            Side = "-";
            PointNumber = -1;
        }
    }else{
        Side = "-";
        PointNumber = -1;
    }
}

bool XMLLoader::grather(size_t a, size_t b)
{
    QString side[2];
    int point[2];
    int col[2];
    int row[2];
    TitelToValue(mEvents[a].getName(),side[0],point[0],col[0],row[0]);
    TitelToValue(mEvents[b].getName(),side[1],point[1],col[1],row[1]);

    if(col[0] == col[1]){
        if(row[0] == col[1]){
            if(side[0] == side[1]){
                return point[0] > point[1];
            }else{
                return (side[0] == "0" || (side[0] == "R" && side[1] == "L"));
            }
        }else{
            return row[0] > col[1];
        }
    }else{
        return col[0] > col[1];
    }
}


void XMLLoader::processDataset()
{
    if (!xml.isStartElement() || xml.name() != "dataset")
        return;
    while (xml.readNextStartElement()) {
        if(xml.name() == "event"){
            processEvent();
            xml.skipCurrentElement();
        }else{
            std::cout<<"Überspringe in Dataset mit "<<xml.name().toString().toStdString()<<std::endl;
            xml.skipCurrentElement();
        }
    }
}


void XMLLoader::processEvent()
{
    if (!xml.isStartElement() || xml.name() != "event")
        return;
    QXmlStreamAttributes att = xml.attributes();
    QString name = att.value("name").toString();

    QString desc = "";
    bool EyeContact=att.value("EyeContact").toInt()==1;
    bool ActiveParticipation=att.value("ActiveParticipation").toInt()==1;
    bool OtherActivities=att.value("OtherActivities").toInt()==1;
    bool Restlessness=att.value("Restlessness").toInt()==1;
    bool Communication=att.value("Communication").toInt()==1;
    while(xml.readNextStartElement()){
        if(xml.name() == "description"){
            desc = readNextText();
        }else{
            xml.skipCurrentElement();
        }
    }
    mEvents.push_back(*(new Event(name, desc,
                                  EyeContact,
                                  ActiveParticipation,
                                  OtherActivities,
                                  Restlessness,
                                  Communication)));
    mPos.push_back(mPos.size());
}


QString XMLLoader::readNextText() {
#ifndef USE_READ_ELEMENT_TEXT
    xml.readNext();
    return xml.text().toString();
#else
    return xml.readElementText();
#endif
}

QString XMLLoader::errorString() {
    return QObject::tr("%1\nLine %2, column %3")
            .arg(xml.errorString())
            .arg(xml.lineNumber())
            .arg(xml.columnNumber());
}
