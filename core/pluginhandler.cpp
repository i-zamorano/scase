#include "pluginhandler.h"

#include <QDebug>
#include <QStack>

PluginHandler::PluginHandler(QObject *parent) :
    QObject(parent)
{
    directory.clear();
}

void PluginHandler::registerPlugin(const QString pluginName, IPlugin *plugin, Browser *browser, QStackedWidget *container) {
    if (directory.contains(pluginName)) {
        directory.remove(pluginName);
    }

    directory.insert(pluginName, plugin);

    plugin->setBrowserDelegate(browser);
    browser->setupPluginTree(plugin);

    QWidget *pluginWidget = plugin->getOutputWidget();
    if (pluginWidget == NULL) {
        qDebug() << "Plugin" << pluginName << "has no presentation widget";
    } else {
        container->addWidget(pluginWidget);
        pluginWidget->hide();
    }
}

bool PluginHandler::invokeActionFromPlugin(const QString pluginName, const QString actionName) {
    if (directory.contains(pluginName)) {
        return directory.value(pluginName)->invokeAction(actionName);
    }
    return false;
}

QWidget *PluginHandler::getOutputWidgetFromPlugin(const QString pluginName) {
    if (directory.contains(pluginName)) {
        return directory.value(pluginName)->getOutputWidget();
    }
    return NULL;
}
