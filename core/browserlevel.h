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

#ifndef BROWSERLEVEL_H
#define BROWSERLEVEL_H

#include <QObject>

#include <QList>

#include "interfaces/IBrowserItem.h"
#include "interfaces/IBrowserLevel.h"

class BrowserLevel;

#include "browseritem.h"

class BrowserLevel : public QObject, public IBrowserLevel
{
    Q_OBJECT
    Q_INTERFACES(IBrowserLevel)

public:
    explicit BrowserLevel(QObject *parent = 0);

    void insertItem(IBrowserItem *item, int pos);
    void insertItem(IBrowserItem *item);
    void removeItemAtPos(int pos);

    void reset();
    void next();
    bool atEnd();

    inline bool isStop() { return isStop_; }
    inline void setIsStop(bool value) { isStop_ = value; }

    BrowserItem *getCurrentItem();

signals:

public slots:

private:
    QList<IBrowserItem *>items;

    int currentPosition;

    bool isStop_;
};

#endif // BROWSERLEVEL_H
