#ifndef BROWSERITEM_H
#define BROWSERITEM_H

#include <QObject>
#include <QVariant>
#include <QString>

#include "interfaces/IBrowserItem.h"
#include "interfaces/IBrowserLevel.h"
#include "interfaces/IPlugin.h"

class BrowserItem;

#include "browserlevel.h"

class BrowserItem : public QObject, public IBrowserItem
{
    Q_OBJECT
    Q_INTERFACES(IBrowserItem)

public:
    explicit BrowserItem(QObject *parent = 0);

    // IBrowserItem members
    QString getPresentationData();
    void setPresentationData(const QString presentationData_);

    void setAction(const QString moduleName, const QString actionName);
    QString getActionModule();
    QString getActionName();

    // BrowserItem members
    bool hasAction();

    BrowserLevel *getLevelBelow();
    void setLevelBelow(BrowserLevel *levelBelow);

    inline IPlugin *getPluginDelegate() { return pluginDelegate; }
    inline void setPluginDelegate(IPlugin *delegate) { pluginDelegate = delegate; }
    inline bool hasPluginDelegate() { return (pluginDelegate != NULL); }

signals:

public slots:

private:
    QString presentationData;

    QString *moduleName;
    QString *actionName;

    IPlugin *pluginDelegate;

};

#endif // BROWSERITEM_H
