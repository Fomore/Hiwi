#include "xmlloader.h"
#include <QFile>
#include <QDebug>

#include <iostream>

XMLLoader::XMLLoader(Loader *loader, Controler *control)
{
    mLoader = loader;
    mControl = control;
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
        qDebug()<< "Fehler beim öffen: "<<filename;
    }
}

void XMLLoader::write(const QString filename, const QString path)
{
    QXmlStreamWriter xmlWriter;
    QFile file(path+filename+"_Label.xml");
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

    for(int i = 0; i < mLoader->getObjectSize(); i++){
        QStringList obj = mLoader->getObject(i);
        xmlWriter.writeStartElement("person");
        xmlWriter.writeCharacters (obj[0]);
        xmlWriter.writeStartElement("description");
        xmlWriter.writeCharacters (obj[2]);
        xmlWriter.writeEndElement();
        xmlWriter.writeEndElement();
    }
    for(int i = 0; i < mLoader->getEventSize(); i++){
        Event ev = mLoader->getEvent(i);
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

    xmlWriter.writeStartElement("images");

    int frame = -1;
    while(mControl->getNextSetFrame(frame)){
        xmlWriter.writeStartElement("image");
        xmlWriter.writeAttribute("file",filename+"-"+QString("%1").arg(frame+1, 6, 10, QChar('0'))+".jpg");
        for(int i = 0; i < mLoader->getObjectSize(); i++){
            ActivModel mod = mControl->getActivModel(frame,i);

            if(mod.getFrame() == frame){
                xmlWriter.writeStartElement("box");
                xmlWriter.writeAttribute("height",QString::number(mod.mH));
                xmlWriter.writeAttribute("left",QString::number(mod.mX));
                xmlWriter.writeAttribute("top",QString::number(mod.mY));
                xmlWriter.writeAttribute("width",QString::number(mod.mW));

                if(mod.mObjectID >= 0){
                    QStringList obj = mLoader->getObject(mod.mObjectID);
                    xmlWriter.writeStartElement("label");
                    xmlWriter.writeCharacters (obj[0]);
                    xmlWriter.writeEndElement();
                }else{
                    std::cout<<"Kein Object "<<i<<std::endl;
                }

                if(mod.mEventID >= 0){
                    Event ev = mLoader->getEvent(mod.mEventID);
                    xmlWriter.writeStartElement("event");
                    xmlWriter.writeAttribute("name",ev.getName());
                    xmlWriter.writeEndElement();
                }

                if(mod.mSetOrienation){
                    xmlWriter.writeStartElement("orientation");
                    xmlWriter.writeAttribute("x",QString("%1").arg(mod.mOrienation[0]));
                    xmlWriter.writeAttribute("y",QString("%1").arg(mod.mOrienation[1]));
                    xmlWriter.writeAttribute("z",QString("%1").arg(mod.mOrienation[2]));
                    xmlWriter.writeEndElement();
                }
                if(mod.mSetPosition){
                    xmlWriter.writeStartElement("position");
                    xmlWriter.writeAttribute("x",QString("%1").arg(mod.mPosition[0]));
                    xmlWriter.writeAttribute("y",QString("%1").arg(mod.mPosition[1]));
                    xmlWriter.writeAttribute("z",QString("%1").arg(mod.mPosition[2]));
                    xmlWriter.writeEndElement();
                }
                if(mod.mSetProjection){
                    xmlWriter.writeStartElement("projection");
                    xmlWriter.writeAttribute("x1",QString("%1").arg(mod.mProjection[0]));
                    xmlWriter.writeAttribute("y1",QString("%1").arg(mod.mProjection[1]));
                    xmlWriter.writeAttribute("x2",QString("%1").arg(mod.mProjection[2]));
                    xmlWriter.writeAttribute("y2",QString("%1").arg(mod.mProjection[3]));
                    xmlWriter.writeEndElement();
                }
                if(mod.mSetLandmarks){
                    xmlWriter.writeStartElement("landmarks");
                    for(int j = 0; j < 5; j++){
                        xmlWriter.writeStartElement("point");
                        xmlWriter.writeAttribute("idx",QString("%1").arg(j));
                        xmlWriter.writeAttribute("x",QString("%1").arg(mod.mLandmarks[j][0]));
                        xmlWriter.writeAttribute("y",QString("%1").arg(mod.mLandmarks[j][1]));
                        xmlWriter.writeEndElement();
                    }
                    xmlWriter.writeEndElement();
                }
                xmlWriter.writeEndElement();
            }
        }
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    std::cout<<"Write "<<filename.toStdString()<<" Ende"<<std::endl;
}

void XMLLoader::processDataset()
{
    if (!xml.isStartElement() || xml.name() != "dataset")
        return;
    while (xml.readNextStartElement()) {
        if (xml.name() == "images"){
            processImages();
        }else if(xml.name() == "event"){
            processEvent();
            xml.skipCurrentElement();
        }else if(xml.name() == "person"){
            processPerson();
        }else{
            std::cout<<"Überspringe in Dataset mit "<<xml.name().toString().toStdString()<<std::endl;
            xml.skipCurrentElement();
        }
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
        }else{
            std::cout<<"Überspringe in Images mit "<<xml.name().toString().toStdString()<<std::endl;
            xml.skipCurrentElement();
        }
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

            int E_id = -1;
            int O_id = processBox(E_id, isOri, orient, isPos, pos, isPro, proj, isLand, land);

            if(O_id >= 0){
                int FrameID = mControl->addEventInFrame(left,top,width,height,frame,E_id,O_id,manual);
                if(isLand){
                    mControl->setLandmarks(FrameID ,O_id, land);
                }if(isOri){
                    mControl->setOrientation(FrameID ,O_id, orient);
                }if(isPos){
                    mControl->setPosition(FrameID ,O_id, pos);
                }if(isPro){
                    mControl->setProjection(FrameID ,O_id, proj);
                }
            }else{
                std::cout<<"Fehler bei Label: "<<xml.lineNumber()<<" "<<O_id<<std::endl;
            }
        }
        else{
            std::cout<<"Überspringe in Image mit "<<xml.name().toString().toStdString()<<" "<<xml.lineNumber()<<std::endl;
            xml.skipCurrentElement();
        }
    }
}

int XMLLoader::processBox(int &E_id, bool &isOri, double orient[3], bool &isPos, double pos[3], bool &isPro, double proj[4], bool &isLand, double land[5][2]) {
    if (!xml.isStartElement() || xml.name() != "box"){
        std::cout<<"Keine Box"<<std::endl;
        return -1;
    }

    int O_id = -1;

    while (xml.readNextStartElement()) {
        if (xml.name() == "label"){
            O_id = processPerson();
        }else if (xml.name() == "orientation"){
            isOri = true;
            QXmlStreamAttributes att = xml.attributes();
            orient[0] = att.value("x").toDouble();
            orient[1] = att.value("y").toDouble();
            orient[2] = att.value("z").toDouble();
            xml.skipCurrentElement();
        }else if (xml.name() == "position"){
            isPos = true;
            QXmlStreamAttributes att = xml.attributes();
            pos[0] = att.value("x").toDouble();
            pos[1] = att.value("y").toDouble();
            pos[2] = att.value("z").toDouble();
            xml.skipCurrentElement();
        }else if (xml.name() == "projection"){
            isPro = true;
            QXmlStreamAttributes att = xml.attributes();
            proj[0] = att.value("x1").toDouble();
            proj[1] = att.value("y1").toDouble();
            proj[2] = att.value("x2").toDouble();
            proj[3] = att.value("y2").toDouble();
            xml.skipCurrentElement();
        }else if(xml.name() == "landmarks"){
            isLand = true;
            processLandmarks(land);
        }else if(xml.name() == "event"){
            E_id = processEvent();
            xml.skipCurrentElement();
        }else{
            std::cout<<"Fehler in Box, Zeile "<<xml.lineNumber()<<": "<<xml.name().toString().toStdString()<<std::endl;
            xml.skipCurrentElement();
        }
    }
    return O_id;
}

void XMLLoader::processLandmarks(double mark[5][2])
{
    if (!xml.isStartElement() || xml.name() != "landmarks")
        return;
    while(xml.readNextStartElement()){
        if (xml.name() == "point"){
            QXmlStreamAttributes att = xml.attributes();
            int id = att.value("idx").toInt();
            double x = att.value("x").toDouble();
            double y = att.value("y").toDouble();
            if(id >= 1 && id < 6){
                mark[id-1][0]=x;
                mark[id-1][1]=y;
            }
        }
        xml.skipCurrentElement();
    }
}

int XMLLoader::processEvent()
{
    if (!xml.isStartElement() || xml.name() != "event")
        return -1;
    QXmlStreamAttributes att = xml.attributes();
    QString name = att.value("name").toString();

    int pos = mLoader->getEventID(name);
    if(pos >= 0){
        return pos;
    }else{
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
        mLoader->addEventSave(name,desc,EyeContact,ActiveParticipation,OtherActivities,Restlessness,Communication);
        return mLoader->getEventID(name);
    }
}

int XMLLoader::processPerson()
{
    QString label = readNextText();
    if (label.isNull()){
        std::cout<<"Label ist Null"<<std::endl;
        return -1;
    }else if(mLoader->getObjectID(label) == -1){
        QString desc;
        while(xml.readNextStartElement()){
            if(xml.name() == "description"){
                desc = readNextText();
            }else{
                xml.skipCurrentElement();
            }
        }
        if(!desc.isEmpty())
            xml.skipCurrentElement();
        return mLoader->addObjectSave(label,desc);
    }else{
        xml.skipCurrentElement();
        return mLoader->getObjectID(label);
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
        return myStringList[myStringList.size()-2].toInt()-1;
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
