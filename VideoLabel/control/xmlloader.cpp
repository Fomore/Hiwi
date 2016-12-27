#include "xmlloader.h"
#include <QFile>
#include <QDebug>

#include <iostream>

XMLLoader::XMLLoader(Loader *loader, Controler *control)
{
    mLoader = loader;
    mControl = control;
}

void XMLLoader::read(const QString filename)
{
    QFile xmlFile(filename);
    if(xmlFile.open(QIODevice::ReadOnly)){
        xml.setDevice(&xmlFile);

        if (xml.readNextStartElement() && xml.name() == "dataset")
            processDataset();

        if (xml.tokenType() == QXmlStreamReader::Invalid)
            xml.readNext();

        if (xml.hasError()) {
            xml.raiseError();
            qDebug() << errorString();
        }
    }else{
        qDebug()<< "Fehler beim öffen: "<<filename;
    }
}

void XMLLoader::write(const QString filename)
{
    QXmlStreamWriter xmlWriter;
    QFile file("./data/"+filename+"_Label.xml");
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

    xmlWriter.writeStartElement("images");

    int frame = -1;
    while(mControl->getNextSetFrame(frame)){
        xmlWriter.writeStartElement("image");
        xmlWriter.writeAttribute("file",filename+"-"+QString("%1").arg(frame, 6, 10, QChar('0'))+".jpg");
        for(int i = 0; i < mControl->getObjectCount(); i++){
            int E_id = -1;
            QRect rec = mControl->getRect(frame,i,E_id);

            if(E_id >= 0){
            xmlWriter.writeStartElement("box");
            xmlWriter.writeAttribute("height",QString::number(rec.height()));
            xmlWriter.writeAttribute("left",QString::number(rec.x()));
            xmlWriter.writeAttribute("top",QString::number(rec.y()));
            xmlWriter.writeAttribute("width",QString::number(rec.width()));

            QStringList obj = mLoader->getObject(i);
            xmlWriter.writeStartElement("label");
            xmlWriter.writeCharacters (obj[0]);
            xmlWriter.writeStartElement("description");
            xmlWriter.writeCharacters (obj[2]);
            xmlWriter.writeEndElement();
            xmlWriter.writeEndElement();

            Event ev = mLoader->getEvent(E_id);
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

            xmlWriter.writeEndElement();
            }
        }
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    std::cout<<"Write "<<filename.toStdString()<<" Ende"<<std::endl;
}

void XMLLoader::processName()
{
    mLoader->addEventSave(readNextText(),"",false,false,false,false,false);
    xml.skipCurrentElement();
}

void XMLLoader::processDataset()
{
    if (!xml.isStartElement() || xml.name() != "dataset")
        return;
    while (xml.readNextStartElement()) {
        if (xml.name() == "images")
            processImages();
        else if (xml.name() == "name")
            processName();
        else
            xml.skipCurrentElement();
    }
}

void XMLLoader::processImages() {
    if (!xml.isStartElement() || xml.name() != "images")
        return;
    while (xml.readNextStartElement()) {
        if (xml.name() == "image"){
            QString file = xml.attributes().value("file").toString();
            int frame = -1;
            if(!file.isNull())
                frame = filnameToFrame(file);
            processImage(frame);
        }
        else
            xml.skipCurrentElement();
    }
}

void XMLLoader::processImage(int frame)
{
    if (!xml.isStartElement() || xml.name() != "image")
        return;
    while (xml.readNextStartElement()) {
        if (xml.name() == "box"){
            int height, left, top, width;
            bool manual, ignore;
            boxAttributToValue(xml.attributes(),height,left,top,width,manual,ignore);

            bool isOri = false;
            double orient[3];
            bool isPos = false;
            double pos[3];
            bool isPro = false;
            double proj[4];
            bool isLand = false;
            double land[5][2];

            int O_id = processBox(isOri, orient, isPos, pos, isPro, proj, isLand, land);
            if(O_id >= 0){
                mControl->setObjectSize(O_id+1);
                int E_id = mControl->addEventInFrame(left,top,width,height,frame,0,O_id,manual);//ToDo: Verknüpfung zum Event besser

                if(isLand)
                    mControl->setLandmarks(O_id, E_id,land);
                if(isOri)
                    mControl->setOrientation(O_id, E_id,orient);
                if(isPos)
                    mControl->setPosition(O_id, E_id,pos);
                if(isPro)
                    mControl->setProjection(O_id, E_id,proj);
            }else{
                std::cout<<"Fehler bei Label: "<<xml.lineNumber()<<" "<<O_id<<std::endl;
            }
        }
        else
            xml.skipCurrentElement();
    }
}

int XMLLoader::processBox(bool &isOri, double orient[3], bool &isPos, double pos[3], bool &isPro, double proj[4], bool &isLand, double land[5][2]) {
    if (!xml.isStartElement() || xml.name() != "box"){
        std::cout<<"Keine Box"<<std::endl;
        return -1;
    }

    QString label;

    while (xml.readNextStartElement()) {
        if (xml.name() == "label"){
            label = readNextText();
        }else if (xml.name() == "orientation"){
            isOri = true;
            QXmlStreamAttributes att = xml.attributes();
            orient[0] = att.value("x").toDouble();
            orient[1] = att.value("y").toDouble();
            orient[2] = att.value("z").toDouble();
        }else if (xml.name() == "position"){
            isPos = true;
            QXmlStreamAttributes att = xml.attributes();
            pos[0] = att.value("x").toDouble();
            pos[1] = att.value("y").toDouble();
            pos[2] = att.value("z").toDouble();
        }else if (xml.name() == "projection"){
            isPro = true;
            QXmlStreamAttributes att = xml.attributes();
            proj[0] = att.value("x1").toDouble();
            proj[1] = att.value("y1").toDouble();
            proj[2] = att.value("x2").toDouble();
            proj[3] = att.value("y2").toDouble();
        }else if(xml.name() == "landmarks"){
            isLand = true;
            processLandmarks(land);
        }else{
            std::cout<<"Fehler: "<<xml.lineNumber()<<" "<<xml.name().toString().toStdString()<<std::endl;
        }
#ifndef USE_READ_ELEMENT_TEXT
        xml.skipCurrentElement();
#endif
    }

    if (!label.isNull()){
        return mLoader->addObjectSave(label,"");
    }else{
        std::cout<<"Label ist Null"<<std::endl;
        return -1;
    }
}

void XMLLoader::processLandmarks(double mark[5][2])
{
    if (!xml.isStartElement() || xml.name() != "landmarks")
        return;
    for(int i = 0; i < 5 && xml.readNextStartElement(); i++){
        if (xml.name() == "point"){
            QXmlStreamAttributes att = xml.attributes();
            int id = att.value("idx").toInt();
            double x = att.value("x").toDouble();
            double y = att.value("y").toDouble();
            if(id >= 1 && id < 6){
                mark[id-1][0]=x;
                mark[id-1][1]=y;
            }
            xml.skipCurrentElement();
        }
    }
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

int XMLLoader::filnameToFrame(QString file)
{
    QStringList myStringList = file.split('-').last().split('.');
    if(myStringList.size() >=2){
        return myStringList[myStringList.size()-2].toInt();
    }
    return -1;
}

void XMLLoader::boxAttributToValue(QXmlStreamAttributes att, int &height, int &left, int &top, int &width, bool &manual, bool &ignore)
{
    height = att.value("height").toInt();
    left = att.value("left").toInt();
    top = att.value("top").toInt();
    width = att.value("width").toInt();

    QString man = att.value("manual").toString();
    manual = !man.isNull() && man.toInt() == 1;

    QString ign = att.value("ignore").toString();
    ignore = !ign.isNull() && ign.toInt() == 1;

}
