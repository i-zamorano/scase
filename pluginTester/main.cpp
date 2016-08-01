#include "documentwriterplugin.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DocumentWriterPlugin *dwp = new DocumentWriterPlugin();
    QString pluginName = dwp->getName();

    return 0;
}
