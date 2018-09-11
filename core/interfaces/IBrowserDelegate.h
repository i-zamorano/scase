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

#ifndef IBROWSERDELEGATE_H
#define IBROWSERDELEGATE_H

#include <QString>

#include "interfaces/IBrowserLevel.h"

class IBrowserDelegate {

public:
    virtual void addItemToLevel(IBrowserLevel *level, int pos, QString name, QString moduleName, QString actionName, bool isSpecial) = 0;
    virtual void goToLevel(IBrowserLevel *ilevel) = 0;
    virtual void actionDidFinish(IBrowserItem *item) = 0;
    virtual IBrowserLevel *addLevel() = 0;
    virtual void stopTansitions() = 0;
    virtual void test() = 0;
};

#define IBROWSER_DELEGATE_SID "net.adapar.SCASE1.IBrowserDelegate/1.0"

Q_DECLARE_INTERFACE(IBrowserDelegate, IBROWSER_DELEGATE_SID)

#endif // IBROWSERDELEGATE_H
