#ifndef IBROWSERITEM_H
#define IBROWSERITEM_H

// TODO: CLEAN UP INTERFACE (MAKE DELEGATES?)

#include <QtPlugin>

class IBrowserItem;

#include "IBrowserLevel.h"

class IBrowserItem {

public:
    virtual QString getPresentationData() = 0;
    virtual void setPresentationData(const QString presentationData_) = 0;

    virtual void setAction(const QString moduleName, const QString actionName) = 0;
    virtual QString getActionModule() = 0;
    virtual QString getActionName() = 0;

    inline IBrowserLevel *getLevelBelow() { return levelBelow_; }
    inline void setLevelBelow(IBrowserLevel *levelBelow) { levelBelow_ = levelBelow; }
    inline bool hasLevelBelow() { return (levelBelow_ != NULL); }

    inline bool isSpecial() { return isSpecial_; }
    inline void setIsSpecial(bool value) { isSpecial_ = value; }

    inline void setContainer(IBrowserLevel *level) { container = level; }
    inline IBrowserLevel *getContainer() { return container; }

protected:
    bool isSpecial_;
    IBrowserLevel *container;
    IBrowserLevel *levelBelow_;

};

#define IBROWSER_ITEM_SID "net.adapar.SCASE1.IBrowserItem/1.0"

Q_DECLARE_INTERFACE(IBrowserItem, IBROWSER_ITEM_SID)

#endif // IBROWSERITEM_H
