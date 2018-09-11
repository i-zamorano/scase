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

#include "browser.h"

#include <QDomDocument>
#include <QDebug>

Browser::Browser(QObject *parent) :
    QObject(parent)
{
    setup();
}

void Browser::setup() {
    pauseDelay = 0;

    rootLevel = new BrowserLevel(this);
    rootLevel->setIsStop(true);

    BrowserItem *ringBellItem = new BrowserItem(this);
    ringBellItem->setModule(SCASE1_MODULE_BROWSER);
    ringBellItem->setAction("ring_bell");
    ringBellItem->setPresentationData("SONAR ALARMA");
    rootLevel->insertItem(ringBellItem);

    levelStack.clear();

    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(nextItem()));
}

void Browser::start(BrowserPresentationWidget *presentationDelegate_, int delay) {
    presentationDelegate = presentationDelegate_;
    timer->setInterval(delay);
    setCurrentLevel(rootLevel);
}

void Browser::stopTimer() {
    timer->stop();
}

void Browser::startTimer() {
    timer->start();
}

void Browser::nextItem() {
    currentLevel->next();
    currentItem = currentLevel->getCurrentItem();

    bool rewind = false;

    if (currentItem == NULL) {
        if (levelStack.size() > 1) {
            if (navigationStatus == BROWSER_READ_FROM_TREE) {
                navigationStatus = BROWSER_PSEUDOITEM_UP_ONE_LEVEL;
            } else if (navigationStatus == BROWSER_PSEUDOITEM_UP_ONE_LEVEL) {
                navigationStatus = BROWSER_READ_FROM_TREE;
                rewind = true;
            }
        } else {
            navigationStatus = BROWSER_READ_FROM_TREE;
            rewind = true;
        }
    }

    if (rewind) {
        currentLevel->reset();
        currentItem = currentLevel->getCurrentItem();
    }

    updatePresentationDelegate();
}

void Browser::executeItem() {
    stopTimer();

    if (navigationStatus == BROWSER_READ_FROM_TREE) {
        if (currentItem != NULL) {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
            qDebug() << "Browser:executeItem:" << currentItem->getPresentationData();
#endif
            if (currentItem->hasLevelBelow()) {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
                qDebug() << "Browser:executeItem:hasLevelBelow";
#endif
                if (currentItem->hasPluginDelegate()) {
                    emit pluginSelected(currentItem->getPluginDelegate());
                    currentItem->getLevelBelow()->setIsStop(true);
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
                    qDebug() << "Browser:executeItem:currentItem->getLevelBelow is a stop?" << ((currentItem->getLevelBelow()->isStop()) ? "yes":"no");
#endif
                }
                setCurrentLevel(currentItem->getLevelBelow());
            } else {
                if (currentItem->hasAction()) {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
                    qDebug() << "Browser:executeItem:hasAction:" << currentItem->getModule() << "," << currentItem->getActionName();
#endif
                    if (currentItem->getModule() == SCASE1_MODULE_BROWSER) {
                        QMetaObject::invokeMethod(this, currentItem->getActionName().toLatin1().constData());
                        actionDidFinish();
                    } else {
                        emit executeActionFromPlugin(currentItem->getModule(), currentItem->getActionName());
                    }
                }
            }
        }
    } else if (navigationStatus == BROWSER_PSEUDOITEM_UP_ONE_LEVEL) {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
        qDebug() << "Browser:executeItem:goToPreviousLevel";
#endif
        goToPreviousLevel();
    }
}

void Browser::goToPreviousStop() {
    goToPreviousLevel(true);
}

void Browser::goToPreviousLevel(bool waitForStop) {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "Browser:executeItem:goToPreviousLevel:waitForStop?" << (waitForStop ? "yes":"no");
    qDebug() << "Browser:executeItem:goToPreviousLevel:levelStack.size?" << levelStack.size();
#endif

    BrowserLevel *previousLevel;
    BrowserLevel *currentLevel;

    if (levelStack.isEmpty()) {
        previousLevel = rootLevel;
    } else {
        currentLevel = levelStack.pop();
        if (levelStack.isEmpty()) {
            previousLevel = currentLevel;
        } else {
            bool stopFound = !waitForStop;

            do {
                previousLevel = levelStack.pop();
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
                if (previousLevel != NULL) {
                    qDebug() << "Browser:executeItem:goToPreviousLevel:(INLOOP):previousLevel belongs to?" << ((previousLevel->getContainer() != NULL) ? previousLevel->getContainer()->getPresentationData() : "NULL");
                }
#endif
                if ((previousLevel != NULL) && waitForStop) {
                    stopFound = previousLevel->isStop();
                }
            } while (!stopFound && (previousLevel != NULL) && !levelStack.isEmpty());

            if (previousLevel == NULL) {
                previousLevel = currentLevel;
            }
        }
    }

    if (previousLevel == NULL) {
        previousLevel = rootLevel;
    }

#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "Browser:executeItem:goToPreviousLevel:(FOUND):previousLevel belongs to?" << ((previousLevel->getContainer() != NULL) ? previousLevel->getContainer()->getPresentationData() : "NULL");
#endif

    if (previousLevel == rootLevel) {
        emit isBackToRoot();
    }

    setCurrentLevel(previousLevel);
}

void Browser::goToLevel(IBrowserLevel *ilevel) {
    if (ilevel == NULL) {
        setCurrentLevel(rootLevel);
    } else {
        BrowserLevel *testLevel;
        BrowserLevel *level = static_cast<BrowserLevel *>(ilevel);
        QStack <BrowserLevel *>::const_iterator i;

        bool found = false;

        for (i = levelStack.begin(); i != levelStack.end(); ++i) {
            testLevel = (*i);
            if (testLevel == level) {
                found = true;
                break;
            }
        }

        if (found) {
            do {
                testLevel = levelStack.pop();
            } while (testLevel != level && !levelStack.isEmpty());
        }

        setCurrentLevel(level);
    }
}

void Browser::setCurrentLevel(BrowserLevel *level) {
    currentLevel = level;
    currentLevel->reset();
    currentItem = currentLevel->getCurrentItem();
    navigationStatus = BROWSER_READ_FROM_TREE;
    levelStack.push(currentLevel);
    presentationDelegate->setPresentationData(QString(""), false);
    QTimer::singleShot(pauseDelay, this, SLOT(updatePresentationDelegate()));
}

void Browser::updatePresentationDelegate() {
    if (presentationDelegate != NULL) {
        if (navigationStatus == BROWSER_READ_FROM_TREE) {
            if (currentItem == NULL) {
                presentationDelegate->setPresentationData(QString::fromUtf8("ERROR: No hay información disponible"), false);
            } else {
                presentationDelegate->setPresentationData(currentItem->getPresentationData(), currentItem->isSpecial());
                if (currentItem->hasFeedback()) {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
                    qDebug() << "Browser:updatePresentationDelegate:hasFeedback:" << currentItem->getModule() << "," << currentItem->getFeedbackName();
#endif
                    emit executeFeedbackFromPlugin(currentItem->getModule(), currentItem->getFeedbackName());
                }
            }
        } else if (navigationStatus == BROWSER_PSEUDOITEM_UP_ONE_LEVEL) {
            presentationDelegate->setPresentationData(QString::fromUtf8("VOLVER AL MENÚ ANTERIOR"), false);
        }
    }

    startTimer();
}

void Browser::actionDidFinish(IBrowserItem *item) {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    qDebug() << "Browser::actionDidFinish - rewinding";
#endif
    if ((item != NULL) && item->hasLevelBelow()) {
        goToLevel(item->getLevelBelow());
    } else {
        goToPreviousLevel();
    }
}

void Browser::setupPluginTree(IPlugin *plugin) {
    QString xml = plugin->getBrowserTree();
    if (rootLevel != NULL) {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
        if (xml.size() == 0) {
            qDebug() << "Plugin" << plugin->getName() << "has no browser tree for the browser";
        } else {
#endif
            qDebug() << "Plugin" << plugin->getName() << "has browser tree";
            BrowserItem *rootItem = readDeploymentTree(plugin->getName(), xml);
            if (rootItem != NULL) {
                plugin->setBrowserItemDelegate(rootItem);
                rootItem->setPluginDelegate(plugin);
                rootItem->setParent(rootLevel);
                rootLevel->insertItem(rootItem);
            }
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
        }
#endif
    }
}

BrowserItem *Browser::readDeploymentTree(const QString pluginName, const QString deploymentTree) {
    int errorLine, errorColumn;

    QString errorMsg;
    QDomDocument doc("deploymentTree");

    if (doc.setContent(deploymentTree, &errorMsg, &errorLine, &errorColumn)) {
        QDomElement rootElement = doc.documentElement();
        if (rootElement.tagName() == "item") {
            return readDeploymentItem(pluginName, rootElement);
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
        } else {
            qDebug() << "Browser::readDeploymentTree: root element of" << pluginName << "should have been 'item' and was" << rootElement.tagName();
#endif
        }
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
    } else {
        qDebug() << "errorMsg:" << errorMsg << "errorLine:" << errorLine << "errorColumn:" << errorColumn;
#endif
    }

    return NULL;
}

BrowserItem *Browser::readDeploymentItem(const QString pluginName, QDomElement currentElement) {
    BrowserItem *currentItem = new BrowserItem(this);

    currentItem->setModule(pluginName);
    currentItem->setPresentationData(currentElement.attribute("name", "-- FALTA INFORMACIÓN --"));

    QDomElement childElement = currentElement.firstChildElement();

    for (; !childElement.isNull(); childElement = childElement.nextSiblingElement()) {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
        qDebug() << "Browser::readDeploymentItem: tagname found" << childElement.tagName() << "in plugin" << pluginName;
#endif
        if (childElement.tagName() == "action") {
            currentItem->setAction(childElement.text());
        } else if (childElement.tagName() == "feedback") {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
            qDebug() << "Browser::readDeploymentItem: found feedback" << childElement.text() << "in plugin" << pluginName;
#endif
            currentItem->setFeedback(childElement.text());
        } else if (childElement.tagName() == "level") {
            BrowserLevel *newLevel = new BrowserLevel(currentItem);
            QDomElement levelElement = childElement.firstChildElement();
            while (!levelElement.isNull()) {
                if (levelElement.tagName() == "item") {
                    BrowserItem *newItem = readDeploymentItem(pluginName, levelElement);
                    if (newItem != NULL) {
                        newLevel->insertItem(newItem);
                        newItem->setParent(newLevel);
                    }
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
                } else {
                    qDebug() << "Browser::readDeploymentItem: level elements can contain items only, while traversing item" << currentItem->getPresentationData() << "in plugin" << pluginName;
#endif
                }
                levelElement = levelElement.nextSiblingElement();
            }
            currentItem->setLevelBelow(newLevel);
        } else {
#ifdef SCASE1_DEBUG_LEVEL_VERBOSE
            qDebug() << "Browser::readDeploymentItem: item elements can contain level, action or feedback only, while traversing item" << currentItem->getPresentationData() << "in plugin" << pluginName;
#endif
            return NULL;
        }
    }

    return currentItem;
}

void Browser::addItemToLevel(IBrowserLevel *levelDelegate, int pos, QString name, QString moduleName, QString actionName, bool isSpecial) {
    if (levelDelegate != NULL) {
        BrowserItem *item = new BrowserItem(this);
        BrowserLevel *level = static_cast<BrowserLevel *>(levelDelegate);

        item->setModule(moduleName);
        item->setPresentationData(name);
        item->setAction(actionName);
        item->setIsSpecial(isSpecial);

        level->insertItem(item, pos);
    }
}

IBrowserLevel *Browser::addLevel()
{
    currentItem = currentLevel->getCurrentItem();
    BrowserLevel *level = new BrowserLevel(static_cast<BrowserItem *>(currentItem));
    currentItem->setLevelBelow(level);

    return static_cast<BrowserLevel *>(level);
}

void Browser::stopTansitions() {

}

void Browser::ring_bell() {
    emit ringBellSignal();
}

void Browser::test() {
    BrowserItem *item = NULL;
    BrowserLevel *currentLevel = NULL;

    QStack <BrowserLevel *> testStack;
    testStack.clear();

    rootLevel->reset();
    testStack.push(rootLevel);

    while (!testStack.isEmpty()) {
        currentLevel = testStack.pop();
        item = currentLevel->getCurrentItem();
        currentLevel->next();
        if (item != NULL) {
            qDebug() << "item:" << item->getPresentationData();
            if (item->hasAction()) {
                qDebug() << "has action:" << item->getModule() << ":" << item->getActionName();
            } else if (item->hasLevelBelow()) {
                qDebug() << "has one level below, going down";
                testStack.push(currentLevel);
                currentLevel = item->getLevelBelow();
                currentLevel->reset();
            }
        }
        testStack.push(currentLevel);
        if (currentLevel->atEnd()) {
            if (!testStack.isEmpty()) {
                currentLevel = testStack.pop();
            }
        }
    }
}
