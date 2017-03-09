#include <QCoreApplication>
#include "control/xmlloader.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    XMLLoader mLoader;
    mLoader.read("/home/falko/Uni/Hiwi/TestHead.xml");

    mLoader.sort();

    mLoader.write("Test","/home/falko/Uni/Hiwi/");

    return app.exec();
}
