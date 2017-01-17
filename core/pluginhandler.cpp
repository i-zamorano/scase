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

#include "pluginhandler.h"

#include <QDebug>
#include <QStack>

PluginHandler::PluginHandler(QObject *parent) :
    QObject(parent)
{
    directory.clear();
}

void PluginHandler::registerPlugin(const QString pluginName, IPlugin *plugin, Browser *browser, QStackedWidget *container) {
    if (directory.contains(pluginName)) {
        directory.remove(pluginName);
    }

    directory.insert(pluginName, plugin);

    plugin->setBrowserDelegate(browser);
    browser->setupPluginTree(plugin);

    QWidget *pluginWidget = plugin->getOutputWidget();
    if (pluginWidget == NULL) {
        qDebug() << "Plugin" << pluginName << "has no presentation widget";
    } else {
        container->addWidget(pluginWidget);
        pluginWidget->hide();
    }
}

void PluginHandler::invokeActionFromPlugin(const QString pluginName, const QString actionName) {
    if (directory.contains(pluginName)) {
        directory.value(pluginName)->invokeAction(actionName);
    }
}

void PluginHandler::invokeFeedbackFromPlugin(const QString pluginName, const QString feedbackName) {
    if (directory.contains(pluginName)) {
        directory.value(pluginName)->invokeFeedback(feedbackName);
    }
}

QWidget *PluginHandler::getOutputWidgetFromPlugin(const QString pluginName) {
    if (directory.contains(pluginName)) {
        return directory.value(pluginName)->getOutputWidget();
    }
    return NULL;
}
