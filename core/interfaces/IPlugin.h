/*************

    SCASE: extendable prototype of an assistive communication tool for people with Locked-in Syndrome

    Copyright (C) 2014  Andrés Aparicio

    This file is part of SCASE.

    SCASE is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SCASE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SCASE.  If not, see <http://www.gnu.org/licenses/>.

    **********/

#ifndef IPLUGIN_H
#define IPLUGIN_H

#include <QtPlugin>
#include <QMetaObject>
#include <QWidget>
#include <QVariant>
#include <QStackedWidget>

#include "interfaces/IBrowserItem.h"
#include "interfaces/IBrowserDelegate.h"

class IPlugin {

public:
    inline void invokeAction(const QString actionName_) { invokeActionPrivate(actionName_); if (browserDelegate != NULL) browserDelegate->actionDidFinish(browserItemDelegate); }
    virtual QString getBrowserTree() = 0;
    virtual QWidget *getOutputWidget() = 0;
    virtual QString getName() = 0;
    virtual QString getPluginPath() = 0;
    virtual void setupOutputWidget() = 0;
    virtual void show(QStackedWidget *container) = 0;
    virtual void hide() = 0;
    inline void setUserPath(QString userPath_) { userPath = userPath_; }
    inline void setBrowserItemDelegate(IBrowserItem *delegate) { browserItemDelegate = delegate; setBrowserItemDelegatePrivate(delegate); }
    inline void setBrowserDelegate(IBrowserDelegate *delegate) { browserDelegate = delegate; }

protected:
    IBrowserItem *browserItemDelegate;
    IBrowserDelegate *browserDelegate;

    QString userPath;

    virtual void invokeActionPrivate(const QString actionName_) = 0;
    virtual void setBrowserItemDelegatePrivate(IBrowserItem *delegate) = 0;

};

#define IPLUGIN(x) qobject_cast<IPlugin *>(x)

#define IPLUGIN_SID "net.adapar.SCASE1.IPlugin/1.0"

Q_DECLARE_INTERFACE(IPlugin, IPLUGIN_SID)

#endif // IPLUGIN_H
