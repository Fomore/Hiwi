#include <QCoreApplication>
#include "control/xmlloader.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    XMLLoader mLoader;
    mLoader.read("/home/falko/Uni/Hiwi/headpose_Lable_erstellen.xml");

    mLoader.sort();

    mLoader.write("Position_","/home/falko/Uni/Hiwi/");

    return app.exec();
}
