#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QPlugin.h>
#include <QMetaObject>
#include <QWidget>
#include <QVariant>

#include "interfaces/IBrowserItem.h"
#include "interfaces/IBrowserDelegate.h"

class IPlugin {

public:
    virtual bool invokeAction(const QString actionName_) = 0;
    virtual QString getBrowserTree() = 0;
    virtual QWidget *getOutputWidget() = 0;
    virtual QString getName() = 0;
    virtual QString getPluginPath() = 0;
    virtual void setupOutputWidget() = 0;
    inline void setBrowserItemDelegate(IBrowserItem *delegate) { browserItemDelegate = delegate; }
    inline void setBrowserDelegate(IBrowserDelegate *delegate) { browserDelegate = delegate; }

protected:
    IBrowserItem *browserItemDelegate;
    IBrowserDelegate *browserDelegate;

};

#define IPLUGIN(x) qobject_cast<IPlugin *>(x)

#define IPLUGIN_SID "net.adapar.SCASE1.IPlugin/1.0"

Q_DECLARE_INTERFACE(IPlugin, IPLUGIN_SID)

#endif // IPLUGIN_H
