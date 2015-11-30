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

#include "browserlevel.h"

BrowserLevel::BrowserLevel(QObject *parent) :
    QObject(parent)
{
    reset();
    isStop_ = false;
    container = NULL;
}

void BrowserLevel::insertItem(IBrowserItem *item) {
    insertItem(item, items.size());
}

void BrowserLevel::insertItem(IBrowserItem *item, int pos) {
    if (item != NULL) {
        if (pos < 0) {
            pos = 0;
        } else if (pos > items.size()) {
            pos = items.size();
        }
        items.insert(pos, item);
        item->setContainer(this);
    }
}

void BrowserLevel::removeItemAtPos(int pos) {
    if (!items.isEmpty()) {
        items.removeAt(pos);
    }
}

void BrowserLevel::reset() {
    currentPosition = 0;
}

void BrowserLevel::next() {
    currentPosition++;
}

bool BrowserLevel::atEnd() {
    return (currentPosition >= items.size());
}

BrowserItem *BrowserLevel::getCurrentItem() {
    if (atEnd() || (currentPosition < 0)) {
        return NULL;
    }
    return static_cast<BrowserItem *>(items.at(currentPosition));
}
