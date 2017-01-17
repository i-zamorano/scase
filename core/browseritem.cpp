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

#include "browseritem.h"

BrowserItem::BrowserItem(QObject *parent) :
    QObject(parent)
{
    presentationData = "";
    levelBelow_ = NULL;
    moduleName = NULL;
    actionName = NULL;
    pluginDelegate = NULL;
    container = NULL;
    isSpecial_ = false;
}

QString BrowserItem::getPresentationData() {
    return presentationData;
}

void BrowserItem::setPresentationData(const QString presentationData_) {
    presentationData = presentationData_;
}

BrowserLevel *BrowserItem::getLevelBelow() {
    return static_cast<BrowserLevel *>(levelBelow_);
}

void BrowserItem::setLevelBelow(BrowserLevel *levelBelow) {
    levelBelow->setContainer(this);
    IBrowserItem::setLevelBelow(levelBelow);
}

bool BrowserItem::hasAction() {
    return ((moduleName != NULL) && (actionName != NULL));
}

QString BrowserItem::getActionName() {
    return QString(*actionName);
}

void BrowserItem::setAction(const QString actionName_) {
    actionName = new QString(actionName_);
}

bool BrowserItem::hasFeedback() {
    return ((moduleName != NULL) && (feedbackName != NULL));
}

QString BrowserItem::getModule() {
    return QString(*moduleName);
}

void BrowserItem::setModule(const QString moduleName_)
{
    moduleName = new QString(moduleName_);
}

QString BrowserItem::getFeedbackName() {
    return QString(*feedbackName);
}

void BrowserItem::setFeedback(const QString feedbackName_) {
    feedbackName = new QString(feedbackName_);
}
