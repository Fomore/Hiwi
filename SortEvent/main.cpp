#include <QCoreApplication>
#include "control/xmlloader.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    XMLLoader mLoader;
    QString ret;
    int a,b,c;
    mLoader.TitelToValue("Img_R1 2 - 4",ret,a,b,c);
    std::cout<<ret.toStdString()<<" "<<a<<" "<<b<<" "<<c<<std::endl;

    return app.exec();
}
