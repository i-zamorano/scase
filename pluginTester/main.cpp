#include <QApplication>

#include "documentwriterplugin.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DocumentWriterPlugin *dwp = new DocumentWriterPlugin();
    QString pluginName = dwp->getName();

    return a.exec();
}
