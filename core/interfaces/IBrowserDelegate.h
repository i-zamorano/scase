#ifndef IBROWSERDELEGATE_H
#define IBROWSERDELEGATE_H

#include <QString>

#include "interfaces/IBrowserLevel.h"

class IBrowserDelegate {

public:
    virtual void addItemToLevel(IBrowserLevel *level, int pos, QString name, QString moduleName, QString actionName, bool isSpecial) = 0;
    virtual void goToLevel(IBrowserLevel *level) = 0;

};

#define IBROWSER_DELEGATE_SID "net.adapar.SCASE1.IBrowserDelegate/1.0"

Q_DECLARE_INTERFACE(IBrowserDelegate, IBROWSER_DELEGATE_SID)

#endif // IBROWSERDELEGATE_H
