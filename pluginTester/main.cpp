#include "documentwriterplugin.h"

int main(int argc, char *argv[])
{
    DocumentWriterPlugin *dwp = new DocumentWriterPlugin();
    QString pluginName = dwp->getName();

    return 0;
}
