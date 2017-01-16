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

#ifndef BROWSER_H
#define BROWSER_H

#include <QObject>
#include <QString>
#include <QDomElement>
#include <QLabel>
#include <QStack>
#include <QTimer>

#include "interfaces/IBrowserDelegate.h"
#include "interfaces/IPlugin.h"

#include "browserlevel.h"
#include "browserpresentationwidget.h"

#define SCASE1_MODULE_BROWSER "net.adapar.SCASE1.Browser/1.0"

enum BROWSER_NAVIGATION_STATUS {
    BROWSER_READ_FROM_TREE,
    BROWSER_PSEUDOITEM_UP_ONE_LEVEL
};

class Browser : public QObject, public IBrowserDelegate
{
    Q_OBJECT
    Q_INTERFACES(IBrowserDelegate)

public:
    explicit Browser(QObject *parent = 0);

    void setup();
    void setupPluginTree(IPlugin *plugin);
    void start(BrowserPresentationWidget *presentationDelegate_, int delay);

    virtual void addItemToLevel(IBrowserLevel *level, int pos, QString name, QString moduleName, QString actionName, bool isSpecial);

    void actionDidFinish(IBrowserItem *item = NULL);

    inline void setPauseDelay(int delay) { pauseDelay = delay; }

    void test();

signals:
    void nextItemSelected(bool isSpecial);
    void pluginSelected(IPlugin *plugin);
    void executeActionFromPlugin(QString moduleName, QString actionName);
    void ringBellSignal();
    void isBackToRoot();

public slots:
    void nextItem();
    void executeItem();
    void goToPreviousStop();
    void goToLevel(IBrowserLevel *ilevel);
    void stopTimer();
    void startTimer();
    void ring_bell();

private slots:
    void updatePresentationDelegate();

private:
    BROWSER_NAVIGATION_STATUS navigationStatus;

    BrowserLevel *rootLevel;

    BrowserItem *readDeploymentTree(const QString pluginName, const QString deploymentTree);
    BrowserItem *readDeploymentItem(const QString pluginName, QDomElement currentElement);

    BrowserLevel *currentLevel;
    BrowserItem *currentItem;

    QStack <BrowserLevel *> levelStack;
    BrowserPresentationWidget *presentationDelegate;

    int itemDelay;
    QTimer *timer;

    int pauseDelay;

    void setCurrentLevel(BrowserLevel *level);

    void goToPreviousLevel(bool waitForStop = false);
};

#endif // BROWSER_H
