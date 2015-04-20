#ifndef PLUGINHANDLER_H
#define PLUGINHANDLER_H

#include <QObject>
#include <QHash>
#include <QWidget>
#include <QStackedWidget>

#include "interfaces/IPlugin.h"

#include "browser.h"

class PluginHandler : public QObject
{
    Q_OBJECT

public:
    explicit PluginHandler(QObject *parent = 0);

    void registerPlugin(const QString pluginName, IPlugin *plugin, Browser *browser, QStackedWidget *container);
    QWidget *getOutputWidgetFromPlugin(const QString pluginName);

signals:

public slots:
    bool invokeActionFromPlugin(const QString pluginName, const QString actionName);

private:
    QHash<QString, IPlugin *> directory;

};

#endif // PLUGINHANDLER_H
