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
