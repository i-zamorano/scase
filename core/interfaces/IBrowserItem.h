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

#ifndef IBROWSERITEM_H
#define IBROWSERITEM_H

// TODO: CLEAN UP INTERFACE (MAKE DELEGATES?)

#include <QtPlugin>

class IBrowserItem;

#include "IBrowserLevel.h"

class IBrowserItem {

public:
    virtual QString getPresentationData() = 0;
    virtual void setPresentationData(const QString presentationData_) = 0;

    virtual QString getModule() = 0;
    virtual void setModule(const QString moduleName_) = 0;

    virtual void setAction(const QString actionName) = 0;
    virtual QString getActionName() = 0;

    virtual void setFeedback(const QString feedbackName) = 0;
    virtual QString getFeedbackName() = 0;

    inline IBrowserLevel *getLevelBelow() { return levelBelow_; }
    inline void setLevelBelow(IBrowserLevel *levelBelow) { levelBelow_ = levelBelow; }
    inline bool hasLevelBelow() { return (levelBelow_ != NULL); }

    inline bool isSpecial() { return isSpecial_; }
    inline void setIsSpecial(bool value) { isSpecial_ = value; }

    inline void setContainer(IBrowserLevel *level) { container = level; }
    inline IBrowserLevel *getContainer() { return container; }

protected:
    bool isSpecial_;
    IBrowserLevel *container;
    IBrowserLevel *levelBelow_;

};

#define IBROWSER_ITEM_SID "net.adapar.SCASE1.IBrowserItem/1.0"

Q_DECLARE_INTERFACE(IBrowserItem, IBROWSER_ITEM_SID)

#endif // IBROWSERITEM_H
