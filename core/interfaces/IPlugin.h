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
#include "interfaces/IBrowserPresentationDelegate.h"

class IPlugin {

public:
    inline void invokeAction(const QString actionName_) { invokeMethodPrivate(actionName_); if (browserDelegate != NULL) browserDelegate->actionDidFinish(browserItemDelegate); }
    inline void invokeFeedback(const QString feedbackName_) { invokeMethodPrivate(feedbackName_); }
    inline void invokeService(const QString serviceName_, const QString command_, QVariant payload) { invokeServicePrivate(serviceName_, command_, payload); }
    inline bool isService() { return isService_; }
    inline QString getServiceName() { return serviceName; }

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
    inline void setBrowserPresentationDelegate(IBrowserPresentationDelegate *delegate) { browserPresentationDelegate = delegate; }

signals:
    virtual void requestService(const QString serviceName, const QString command, QVariant payload) = 0;
    virtual void requestTransition(const QString serviceName) = 0;

protected:
    IBrowserItem *browserItemDelegate;
    IBrowserDelegate *browserDelegate;
    IBrowserPresentationDelegate *browserPresentationDelegate;

    QString userPath;

    bool isService_;
    QString serviceName;

    virtual void invokeMethodPrivate(const QString actionName_) = 0;
    virtual void invokeServicePrivate(const QString serviceName_, const QString command_, QVariant payload) = 0;
    virtual void setBrowserItemDelegatePrivate(IBrowserItem *delegate) = 0;

};

#define IPLUGIN(x) qobject_cast<IPlugin *>(x)

#define IPLUGIN_SID "net.adapar.SCASE1.IPlugin/1.0"

Q_DECLARE_INTERFACE(IPlugin, IPLUGIN_SID)

#endif // IPLUGIN_H
