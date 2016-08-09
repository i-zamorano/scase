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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QLabel>
#include <QStackedWidget>
#include <QSound>
#include <QResizeEvent>
#include <QSettings>
#include <QFile>

#include "interfaces/IPlugin.h"

#include "pluginhandler.h"

#include "browser.h"
#include "browserpresentationwidget.h"

#include "interactionwidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void pluginSelected(IPlugin *plugin);
    void logAction(QString action, QString message = "");
    void ringBell();

private:
    void keyPressEvent(QKeyEvent *event);

    void setup();
    void setupInterface();
    void setupPlugins();

    void hideCurrentPlugin();

    void openActionLogger();
    void closeActionLogger();
    QFile *actionLog;

    bool isActionLogActive;

    PluginHandler *pluginHandler;
    Browser *browser;

    QStackedWidget *zoneFeedback;
    InteractionWidget *zoneInteraction;
    BrowserPresentationWidget *zoneBrowser;

    QSound *bell;

    QSettings *settings;

    IPlugin *currentPlugin;
};

#endif // MAINWINDOW_H
