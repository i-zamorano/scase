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

#ifndef IBROWSERPRESENTATIONDELEGATE_H
#define IBROWSERPRESENTATIONDELEGATE_H

#include <QString>
#include <QtPlugin>

class IBrowserPresentationDelegate {

public:
    virtual void setPresentationData(QString data, bool isSpecial) = 0;

};

#define IBROWSER_PRESENTATION_DELEGATE_SID "net.adapar.SCASE1.IBrowserPresentationDelegate/1.0"

Q_DECLARE_INTERFACE(IBrowserPresentationDelegate, IBROWSER_PRESENTATION_DELEGATE_SID)

#endif // IBROWSERPRESENTATIONDELEGATE_H
