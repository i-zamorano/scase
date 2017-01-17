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

    QString getModule();
    void setModule(const QString moduleName_);

    void setAction(const QString actionName);
    QString getActionName();

    void setFeedback(const QString feedbackName);
    QString getFeedbackName();

    // BrowserItem members
    bool hasAction();
    bool hasFeedback();

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
    QString *feedbackName;

    IPlugin *pluginDelegate;

};

#endif // BROWSERITEM_H
