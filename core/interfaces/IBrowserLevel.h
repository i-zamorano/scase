#ifndef IBROWSERLEVEL_H
#define IBROWSERLEVEL_H

// TODO: CLEAN UP INTERFACE (MAKE DELEGATES?)

#include <QObject>
#include <QPlugin.h>

class IBrowserLevel;

#include "IBrowserItem.h"

class IBrowserLevel {

public:
    virtual void insertItem(IBrowserItem *item, int pos) = 0;
    virtual void insertItem(IBrowserItem *item) = 0;
    virtual void removeItemAtPos(int pos) = 0;

    inline void setContainer(IBrowserItem *item) { container = item; }
    inline IBrowserItem *getContainer() { return container; }

protected:
    IBrowserItem *container;

};

#define IBROWSER_LEVEL_SID "net.adapar.SCASE1.IBrowserLevel/1.0"

Q_DECLARE_INTERFACE(IBrowserLevel, IBROWSER_LEVEL_SID)

#endif // IBROWSERLEVEL_H
